/*********************************************
 * ZmqDealer.h
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/

#ifndef ZMQDEALER_H_
#define ZMQDEALER_H_

class ZmqDealer
{
public:
	ZmqDealer();
	virtual ~ZmqDealer();

	void Run();

private:
	void* sock_dealer_;
	void* sock_pair_;
	void* ctx_;
};

#endif /* ZMQDEALER_H_ */
