/*********************************************
 * Achilles
 * Author: kamuszhou@tencent.com kamuszhou@qq.com
 * website: blog.ykyi.net
 * Created on: Nov 6, 2016
 * Praise Be to the Lord. BUG-FREE CODE !
 ********************************************/

#include <iostream>
#include <algorithm>
#include <iterator>
#include "Router.h"

int main(int argc, char** argv)
{
//	MyPipe pipe(6);
//	const auto& lines = pipe.GetResultLines();
//
//    std::copy(lines.begin(), lines.end(),
//              std::ostream_iterator<std::string>(std::cout));
	Router router;
	router.Run();

	return 0;
}
