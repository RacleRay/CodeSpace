/*************************************************************************
    > File Name: asio.1.cpp
    > Author: racle
    > Mail: racleray@qq.com
    > Created Time:
 ************************************************************************/

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

int main() {
    boost::asio::io_service     io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.wait();
    std::cout << "Hello, world!" << std::endl;

    return 0;
}
