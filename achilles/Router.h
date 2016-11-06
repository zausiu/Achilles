/*********************************************
 * Router.h
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/

#ifndef ROUTER_H_
#define ROUTER_H_

class Router
{
public:
	Router();
	virtual ~Router();

	void Run();

private:
	bool ParseJsonTxt(const std::string& txt, std::string& cmd, int& start_line, int& line_count);

private:
	void* ctx_;
	void* sock_;
};

#endif /* ROUTER_H_ */
