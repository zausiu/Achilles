/*********************************************
 * ZmqOffice.cpp
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/
#include <iostream>
#include <algorithm>
#include <iterator>
#include <list>
#include <stdlib.h>
#include <assert.h>
#include <zmq.h>
#include "ZmqOffice.h"

static const char* const magic_gate = "inproc://gate";

ZmqOffice::ZmqOffice()
{
	ctx_ = zmq_ctx_new();
	assert(ctx_ != NULL);

	sock_dealer_ = zmq_socket(ctx_, ZMQ_DEALER);
	assert(sock_dealer_ != NULL);
	const std::string& id("yambs");
	int ret = zmq_setsockopt(sock_dealer_, ZMQ_IDENTITY, id.data(), id.size());
	assert(ret == 0);
	ret = zmq_connect(sock_dealer_, "tcp://127.0.0.1:20160");
	if (ret != 0)
	{
		std::cerr << "zmq_connect error: " << zmq_strerror(errno) << std::endl;
		abort();
	}

	sock_pair0_ = zmq_socket(ctx_, ZMQ_PAIR);
	sock_pair1_ = zmq_socket(ctx_, ZMQ_PAIR);
	assert(sock_pair0_ != NULL);
	assert(sock_pair1_ != NULL);
	ret = zmq_connect(sock_pair0_, magic_gate);
	assert(ret == 0);
	ret = zmq_bind(sock_pair1_, magic_gate);
	assert(ret == 0);
}

ZmqOffice::~ZmqOffice()
{
	zmq_ctx_term(ctx_);
	zmq_close(sock_pair0_);
	zmq_close(sock_pair1_);
	zmq_close(sock_dealer_);
}

void ZmqOffice::Run()
{
	char buff[4096*100];
	zmq_pollitem_t items[2];

	items[0].socket = sock_pair1_;
	items[0].events = ZMQ_POLLIN;

	items[1].socket = sock_dealer_;
	items[1].events = ZMQ_POLLIN;

	while(true)
	{
		int len;
		int ret = zmq_poll(items, 2, -1);
		if (items[0].revents & ZMQ_POLLIN)
		{
			len = zmq_recv(items[0].socket, buff, sizeof(buff), 0);
			assert(len >= 0);
			len = zmq_send(sock_dealer_, buff, len, 0);
			std::cout << "send text on sock_dealer: " << buff << std::endl;
			assert(len >= 0);
		}

		if (items[1].revents & ZMQ_POLLIN)
		{
			std::list<std::string> resps;
			std::string id;

			int more;
			size_t more_size = sizeof(more);
			for (int i = 0; true; i++)
			{
				int rc = zmq_recv(sock_dealer_, buff, sizeof(buff), 0);
				assert (rc != -1);
				buff[rc] = 0;
				rc = zmq_getsockopt(sock_dealer_, ZMQ_RCVMORE, &more, &more_size);
				assert (rc == 0);

				if (i == 0)
				{
					id = buff;
				}
				else
				{
					resps.push_back(buff);
				}

				if (!more)
					break;
			}

			OnResponse(id, resps);
		}
	}
}

int ZmqOffice::Send(const std::string& req)
{
	int ret = zmq_send(sock_pair0_, req.data(), req.size(), 0);
	if (ret < 0)
	{
		std::cerr << "zmq_send failed: " << zmq_strerror(errno) << "\n";
	}

    return ret;
}

void ZmqOffice::OnResponse(const std::string& id, const std::list<std::string>& resps)
{
	std::cout << "Received responses: \n";
	std::cout << "id is: " << id << "\n";
	std::copy(resps.begin(), resps.end(), std::ostream_iterator<std::string>(std::cout));
	std::cout << "##################################\n\n";
}
