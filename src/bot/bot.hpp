#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string;

namespace bot
{
    class Bot
    {
    private:
        // socket
        boost::asio::io_context &io_context;
        tcp::socket socket;

        string name;
        string password;
        short port = 1234;

    public:
        Bot(boost::asio::io_context &io_context, string name, string password, short port)
            : io_context(io_context),
              socket(io_context),
              name(name),
              password(password),
              port(port){};
        ~Bot()
        {
            socket.close();
        };

        void run();

    private:
        void connect();
        void send(string message);
        void receive();
        void process(string message);
    };
}