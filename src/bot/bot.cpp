#include "bot.hpp"
#include "../helpers.hpp"

#include <string>
#include <boost/asio.hpp>

using namespace helpers;

namespace bot
{
    void Bot::connect()
    {
        tcp::resolver resolver = tcp::resolver(sock.get_executor());
        boost::asio::connect(sock, resolver.resolve("localhost", std::to_string(port)));
    }

    void Bot::join()
    {
        do_write("join|" + username + "|" + password);
    }

    void Bot::do_read()
    {
        char *data_ = new char[1024];
        memset(data_, 0, 1024);

        sock.async_read_some(boost::asio::buffer(data_, 1024),
                             [this, data_](boost::system::error_code ec, std::size_t /* length */)
                             {
                                if (!ec)
                                {
                                // string from data_
                                string data = data_;
                                // delete data_
                                delete[] data_;

                                // strip endline if it exists
                                if (data.back() == '\n')
                                {
                                    data.pop_back();
                                }

                                // log
                                Logger::ln(data, red);
                                }
                                do_read();
                             });
    }

    void Bot::do_write(string msg)
    {
        // if message doesn't end with \n, add it
        if (msg.back() != '\n')
        {
            msg += "\n";
        }

        boost::asio::async_write(sock, boost::asio::buffer(msg),
                                 [this, msg](boost::system::error_code ec, std::size_t /* length */)
                                 {
                                    if (!ec)
                                    {
                                        // log
                                        Logger::ln("< " + msg, green);
                                    }
                                 });
    }
}