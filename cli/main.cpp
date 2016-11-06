/*********************************************
 * Achilles
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/

#include <iostream>
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include "ZmqOffice.h"
#include "json/json.h"

int main(int argc, char** argv)
{
	ZmqOffice zmq_office;
	boost::shared_ptr<boost::thread> thread = boost::make_shared<boost::thread>(
			boost::bind(&ZmqOffice::Run, &zmq_office)
	);

	Json::Value jv;
	jv["id"] = 1000;
//	jv["cmd"] = "cat /etc/passwd";
	jv["cmd"] = "ls -la";
	jv["start_line"] = 1;
	jv["line_count"] = 4;
	while (true)
	{
		zmq_office.Send(jv.toStyledString());
		sleep(2);
	}

	return 0;
}
