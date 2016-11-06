/*********************************************
 * Router.cpp
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <zmq.h>
#include "json/json.h"
#include "Router.h"
#include "MyPipe.h"

Router::Router() 
{
	ctx_ = zmq_ctx_new();
	assert(ctx_ != NULL);

	sock_ = zmq_socket(ctx_, ZMQ_ROUTER);
	assert(sock_ != NULL);
	int ret = zmq_bind(sock_, "tcp://*:20160");
	if (ret != 0)
	{
		std::cerr << "zmq_bind error: " << zmq_strerror(errno) << std::endl;
		abort();
	}
}

Router::~Router() 
{
	zmq_ctx_term(ctx_);
	zmq_close(sock_);
}

void Router::Run()
{
	while(true)
	{
		char identifier[1024];
		char buffer[4096];
		std::string request_js;
		int len = zmq_recv(sock_, identifier, sizeof(identifier), 0);
		if (len < 0)
		{
			std::cerr << "zmq_recv error: " << zmq_strerror(errno) << std::endl;
			abort();
		}
		std::cout << "Received an identifier: " << identifier << std::endl;

		int more;
		size_t more_size;
		for (int i = 0; true; i++)
		{
			int rc = zmq_getsockopt(sock_, ZMQ_RCVMORE, &more, &more_size);
			assert(rc == 0);

			if (!more)
			{
				if (i < 2)
				{
					std::cerr << "Expected delimiter or request json doesn't exist.\n";
				}
				break;
			}

			len = zmq_recv(sock_, buffer, sizeof(buffer), 0);
			if (len < 0)
			{
				std::cerr << "zmq_recv error: " << zmq_strerror(errno) << std::endl;
				abort();
			}

			if (i == 0)
			{
				assert(len == 0);
			}
			else if (i == 1)
			{
				std::cout << "Received request json: " << buffer << std::endl;
				request_js = buffer;
			}
		}

		if (request_js.empty())
		{
			std::cerr << "Expected request json is not present.\n";
			continue;
		}

		std::string cmd;
		int start_line, line_count;
		if (!ParseJsonTxt(request_js, cmd, start_line, line_count))
		{
			std::cerr << "Request json is bad-formated.\n";
			continue;
		}

		std::cout << "Request parameters: " << cmd << " " << start_line << " " << line_count << std::endl;

		MyPipe pipe(cmd, start_line, line_count);
		const auto& lines = pipe.GetResultLines();
		std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(std::cout));

		len = zmq_send(sock_, identifier, sizeof(identifier), ZMQ_SNDMORE);
		if (len < 0)
		{
			std::cerr << "zmq_send error: " << zmq_strerror(errno) << std::endl;
			abort();
		}
		int flags = 0;
		if (!lines.empty())
		{
			flags = ZMQ_SNDMORE;
		}
        len = zmq_send(sock_, "", 0, flags);
        assert(len == 0);
        int sz = lines.size();
        for (int i = 1; i <= sz; i++)
        {
        	flags = ZMQ_SNDMORE;
        	if (i == sz)
        	{
        		flags = 0;
        	}
        	const std::string& line = lines[i];
        	len = zmq_send(sock_, line.data(), line.size(), flags);
        	assert(len >= 0);
        }
	}
}

bool Router::ParseJsonTxt(const std::string& txt, std::string& cmd, int& start_line, int& line_count)
{
	Json::Reader reader;
	Json::Value jv_root;
	if (!reader.parse(txt, jv_root, false))
	{
		return false;
	}

	cmd = jv_root["cmd"].asString();
	start_line = jv_root["start_line"].asInt();
	line_count = jv_root["line_count"].asInt();

	return true;
}
