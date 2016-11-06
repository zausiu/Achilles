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
		char buffer[4096];
		std::string identity;
		std::string request_js;
		int len = zmq_recv(sock_, buffer, sizeof(buffer), 0);
		if (len < 0)
		{
			std::cerr << "zmq_recv error: " << zmq_strerror(errno) << std::endl;
			abort();
		}
		identity.assign(buffer, buffer + len);
		std::cout << "Received an identifier: [" << identity << "]\n";

		int more;
		size_t more_size;
		for (int i = 0; true; i++)
		{
			int rc = zmq_getsockopt(sock_, ZMQ_RCVMORE, &more, &more_size);
			if (rc != 0)
			{
				std::cerr << "zmq_getsockopt failed: " << zmq_strerror(errno) << "\n";
				abort();
			}

			if (!more)
			{
				break;
			}

			len = zmq_recv(sock_, buffer, sizeof(buffer), 0);
			if (len < 0)
			{
				std::cerr << "zmq_recv error: " << zmq_strerror(errno) << std::endl;
				abort();
			}
			buffer[len] = 0;

			std::cout << "Received request json: " << buffer << std::endl;
			request_js = buffer;
		}

		if (request_js.empty())
		{
			std::cerr << "Expected request json is not present.\n";
			continue;
		}

		std::string cmd;
		int start_line, line_count;
		std::string id;
		if (!ParseJsonTxt(request_js, cmd, start_line, line_count, id))
		{
			std::cerr << "Request json is bad-formated.\n";
			continue;
		}

		std::cout << "Request parameters: [" << cmd << "] " << start_line << " "
				  << line_count << " " << id << std::endl;

		MyPipe pipe(cmd, start_line, line_count);
		const auto& lines = pipe.GetResultLines();
		std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(std::cout));

		len = zmq_send(sock_, identity.data(), identity.size(), ZMQ_SNDMORE);
		if (len < 0)
		{
			std::cerr << "zmq_send error: " << zmq_strerror(errno) << std::endl;
			abort();
		}
//        len = zmq_send(sock_, "", 0, ZMQ_SNDMORE);
//        assert(len == 0);
        len = zmq_send(sock_, id.data(), id.size(), ZMQ_SNDMORE);
        assert(len >= 0);
        int sz = lines.size();
        for (int i = 0; i < sz; i++)
        {
        	const std::string& line = lines[i];
        	len = zmq_send(sock_, line.data(), line.size(), ZMQ_SNDMORE);
        	assert(len >= 0);
        }
        len = zmq_send(sock_, "", 0, 0);
        assert(len == 0);
	}
}

bool Router::ParseJsonTxt(const std::string& txt, std::string& cmd, int& start_line, int& line_count, std::string& id)
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
	id = jv_root["id"].asString();

	return true;
}
