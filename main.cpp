#include <iostream>
#include "../sge/logger.h"
#include <boost/asio.hpp>
#include <boost/exception/diagnostic_information.hpp>

using namespace std;
using namespace boost::asio;
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/use_future.hpp>

using boost::asio::ip::udp;

#define CATCHES \
catch (boost::exception &e) \
{ \
    LOG(LogLevel::error) << "boost exception: " << boost::diagnostic_information(e); \
} \
catch (std::system_error &e) \
{ \
    LOG(LogLevel::error) << "system exception: " << e.what(); \
} \
catch (std::exception &e) \
{ \
    LOG(LogLevel::error) << "std exception: " << e.what(); \
} \
catch ( ... ) \
{ \
    LOG(LogLevel::error) << "Unknown exception"; \
}

void get_daytime(boost::asio::io_service& io_service)
{
    try
    {
        udp::resolver resolver(io_service);
        udp::endpoint ep(ip::address::from_string("127.0.0.1"), 2013);

        std::future<udp::resolver::iterator> iter =
                resolver.async_resolve(ep, use_future);

        udp::socket socket(io_service, udp::v4());

        char send_buf[] = { 0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0 };
        std::future<std::size_t> send_length =
                socket.async_send_to(buffer(send_buf),
                                     *iter.get(),
                                     use_future);

        size_t t = send_length.get();
        LOG(info) << t;

        std::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        std::future<std::size_t> recv_length =
                socket.async_receive_from(
                    buffer(recv_buf),
                    sender_endpoint,
                    use_future);

        t = recv_length.get();
        LOG(info) << t << recv_buf.data();
    }
    CATCHES
}

int main(int, char*[])
{
    try
    {
        io_service io_service;
        io_service::work work(io_service);
        std::thread thread([&io_service](){ io_service.run(); });

        get_daytime(io_service);

        io_service.stop();
        thread.join();
    }
    CATCHES

    return 0;
}
