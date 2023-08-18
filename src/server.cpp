#include "server.hpp"

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                // create new player
                Player *player = new Player("Player", "red", {0, 0});

                std::make_shared<Session>(std::move(socket), player)->start();
            }

            do_accept();
        });
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                                if (!ec)
                                {
                                    // strip endline from data
                                    string data_str = stripEndline(data_);

                                    cout << colorize(">", color::red) << " "
                                         << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                         << data_str << endl;

                                    // process packet
                                    /* ProcessErrorCode ec = */ process(data_str);

                                    do_write(length);
                                }
                            });
}

void Session::do_write(std::size_t length)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     // strip endline from data
                                     string data_str = stripEndline(data_);

                                     cout << colorize("<", color::green) << " "
                                          << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                          << data_str << endl
                                          << endl;
                                     do_read();
                                 }

                                 // clear data buffer
                                 memset(data_, 0, max_length);
                             });
}

Session::ProcessErrorCode Session::process(string data){
    // split packet type and arguments
    // <ptype> | <arg1> <arg2> ...
    stringstream stream(data);
    string ptype;
    vector<string> args;

    std::getline(stream, ptype, '|');
    ptype = strip(ptype);

    string arg;
    while (std::getline(stream, arg, ' '))
    {
        // drop empty args
        if (arg == "")
        {
            continue;
        }
        args.push_back(strip(arg));
    }

    // temporarily log packet type and args
    cout << colorize("Packet type: ", color::yellow) << ptype << endl;
    cout << colorize("Packet args: ", color::yellow);
    for (string arg : args)
    {
        cout << "\"" << arg << "\" ";
    }
    cout << endl;

    return OK;

}
