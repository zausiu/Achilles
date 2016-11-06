/*********************************************
 * MyPipe.h
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/

#ifndef MYPIPE_H_
#define MYPIPE_H_

#include <vector>
#include <string>

class MyPipe
{
public:
	MyPipe(const std::string cmd, int start_line, int line_count);
	virtual ~MyPipe();
	const std::vector<std::string>& GetResultLines()const;

private:
	void Plenish();

private:
	std::string cmd_;
	int start_line_;
	int line_count_;
	std::vector<std::string> lines_;
};

#endif /* MYPIPE_H_ */
