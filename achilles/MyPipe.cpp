/*********************************************
 * MyPipe.cpp
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/
#include <stdio.h>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "MyPipe.h"

MyPipe::MyPipe(const std::string cmd, int start_line, int line_count)
{
	cmd_ = cmd;
	start_line_ = start_line;
	line_count_ = line_count;
	lines_.reserve(line_count);

	Plenish();
}

MyPipe::~MyPipe()
{
}

const std::vector<std::string>& MyPipe::GetResultLines()const
{
	return lines_;
}

void MyPipe::Plenish()
{
	// replace the cmd string to any external command u wish
//	static const std::string cmd("cat /etc/passwd");
	char line[4096*100];

    boost::shared_ptr<FILE> pipe(popen(cmd_.c_str(), "r"), pclose);
    if (!pipe)
    {
    	throw std::runtime_error("popen() failed!");
    }
    int end_line = start_line_ + line_count_;
    for (int i = 0; i < end_line && !feof(pipe.get()); i++)
    {
        if (fgets(line, sizeof(line), pipe.get()) != NULL)
        {
        	if ( i < start_line_ )
        		continue;

        	lines_.push_back(line);
        }
    }
}
