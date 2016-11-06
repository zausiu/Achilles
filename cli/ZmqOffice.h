/*********************************************
 * ZmqOffice.h
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/

#ifndef ZMQDEALER_H_
#define ZMQOFFICE_H_

class ZmqOffice
{
public:
	ZmqOffice();
	virtual ~ZmqOffice();

	void Run();
	int Send(const std::string& req);

private:
	void OnResponse(const std::string& id, const std::list<std::string>& resps);

private:
	void* sock_dealer_;
	void* sock_pair0_;
	void* sock_pair1_;
	void* ctx_;
};

#endif /* ZMQDEALER_H_ */
