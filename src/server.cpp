#include "server.hpp"

string colorize(string text, colors color)
{
    return "\033[" + std::to_string(color) + "m" + text + "\033[0m";
}
string strip_endline(string text)
{
    return text.substr(0, text.length() - 1);
}

void server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<session>(std::move(socket))->start();
            }

            do_accept();
        });
}

void session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    // strip endline from data
                                    string data_str = strip_endline(data_);

                                    cout << colorize(">", colors::red) << " "
                                         << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                         << data_str << endl;
                                    do_write(length);
                                }
                            });
}

void session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     // strip endline from data
                                     string data_str = strip_endline(data_);

                                     cout << colorize("<", colors::green) << " "
                                          << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                          << data_str << endl
                                          << endl;
                                     do_read();
                                 }

                                 // clear data buffer
                                 memset(data_, 0, max_length);
                             });
}
