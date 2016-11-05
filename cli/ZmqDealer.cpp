/*********************************************
 * ZmqDealer.cpp
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <zmq.h>
#include "ZmqDealer.h"

ZmqDealer::ZmqDealer()
{
	ctx_ = zmq_ctx_new();
	assert(ctx_ != NULL);

	sock_dealer_ = zmq_socket(ctx_, ZMQ_DEALER);
	assert(sock_dealer_ != NULL);
	int ret = zmq_connect(sock_dealer_, "tcp://127.0.0.1:20160");
	if (ret != 0)
	{
		std::cerr << "zmq_connect error: " << zmq_strerror(errno) << std::endl;
		abort();
	}

	sock_pair_ = zmq_socket(ctx_, ZMQ_PAIR);
	assert(sock_pair_ != NULL);
}

ZmqDealer::~ZmqDealer()
{
	zmq_ctx_term(ctx_);
	zmq_close(sock_pair_);
	zmq_close(sock_dealer_);
}

void ZmqDealer::Run()
{
	int more;
	size_t more_size = sizeof (more);
	do {
	    /* Create an empty ØMQ message to hold the message part */
	    zmq_msg_t part;
	    int rc = zmq_msg_init(&part);
	    assert(rc == 0);
	    /* Block until a message is available to be received from socket */
	    rc = zmq_msg_recv(&part, sock_dealer_, 0);
	    assert (rc != -1);
	    /* Determine if more message parts are to follow */
	    rc = zmq_getsockopt(sock_dealer_, ZMQ_RCVMORE, &more, &more_size);
	    assert (rc == 0);
	    zmq_msg_close (&part);
	}while (more);
}
