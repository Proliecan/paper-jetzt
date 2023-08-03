#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using std::string;

// forward declaration
class session;
enum colors
{
    red = 31,
    green = 32,
    blue = 34,
    yellow = 33,
    magenta = 35,
    cyan = 36,
    white = 37,
    red_bg = 41,
    green_bg = 42,
    blue_bg = 44,
    yellow_bg = 43,
    magenta_bg = 45,
    cyan_bg = 46,
    white_bg = 47,
    bold = 1,

    reset = 0
};
string colorize(string text, colors color);


class server
{
private:
    tcp::acceptor acceptor_;

public:
    server(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        cout << "Server listening on port " << colorize(std::to_string(port), cyan) << endl;
        do_accept();
    }

    ~server()
    {
        cout << "Server shutting down" << endl;
    }

private:
    void do_accept();
};

class session
    : public std::enable_shared_from_this<session>
{

private:
    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];

public:
    session(tcp::socket socket)
        : socket_(std::move(socket))
    {
        cout << colorize("New session with", colors::yellow) << " " << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl;
    }

    ~session()
    {
        cout << colorize("Ending session with ", colors::yellow) << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl;
    }

    void start()
    {
        do_read();
    }

private:
    void do_read();

    void do_write(std::size_t length);
};