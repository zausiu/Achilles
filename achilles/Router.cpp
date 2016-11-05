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
#include <zmq.h>
#include "Router.h"

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
		char buff[4096];
		int len = zmq_recv(sock_, buff, sizeof(buff), 0);
		std::cout << "Received request from client: " << buff << std::endl;
	}
}
