#pragma once

#include "../helpers.hpp"

#include <string>
#include <vector>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using string = std::string;

namespace bot
{
    class Bot
    {
    public:
        short port;
        string username;
        string password;
        tcp::socket sock;

        Bot(boost::asio::io_context &io_context, short port, string username, string password)
            : port(port),
              username(username),
              password(password),
              sock(io_context){};

        void connect();
        void join();
        void do_read();
        void do_write(string msg);

    private:
        string read_buffer;
        string write_buffer;
    };
}
