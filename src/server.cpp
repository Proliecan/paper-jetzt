#include "server.hpp"

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket))->start();
            }

            do_accept();
        });
}

void Session::do_read()
{
    auto self(shared_from_this());
    char *data_ = new char[max_length];
    // clear data
    memset(data_, 0, max_length);

    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self, data_](boost::system::error_code ec, std::size_t /* length */)
                            {
                                if (!ec)
                                {
                                    // data to string
                                    string data_str = string(data_);
                                    // strip endline from data
                                    data_str = stripEndline(data_str);
                                    
                                    delete[] data_;
                                    

                                    cout << colorize(">", color::red) << " "
                                         << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                         << data_str << endl;

                                    // process packet
                                    ProcessErrorCode ec = process(data_str);
                                    if (ec == ERROR)
                                    {
                                        cout << colorize("Packet processing failed on session with ", color::red)
                                             << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl
                                             << colorize("On packet: ", color::red) << data_str << endl;
                                    }

                                    do_read();
                                }
                            });
}

void Session::do_write(string data)
{
    auto self(shared_from_this());
    // calc length of data
    std::size_t length = data.length();
    boost::asio::async_write(socket_, boost::asio::buffer(data.c_str(), length),
                             [this, self, data](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     // strip endline from data
                                     string data_str = stripEndline(data);

                                     cout << colorize("<", color::green) << " "
                                          << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                          << data_str << endl
                                          << endl;
                                 }
                             });

}

Session::ProcessErrorCode Session::process(string data)
{
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

    // process packet
    if (ptype == "join")
    {
        // check for 2 args
        if (args.size() != 2)
        {
            this->sendPacket(error, {"ARG_ERR", "Packet type \"join\" requires 2 arguments"});
            return ERROR;
        }

        // todo: check credentials and create player object
        return OK;
    }
    else if (ptype == "move")
    {
        // check for one arg that has to be one of up, right, down or left
        if (args.size() != 1 || (args[0] != "up" && args[0] != "right" && args[0] != "down" && args[0] != "left"))
        {
            this->sendPacket(error, {"ARG_ERR", "Packet type \"move\" requires 1 argument that has to be one of up, right, down or left"});
            return ERROR;
        }

        // todo: register move for next tick
        return OK;
    }
    else if (ptype == "chat")
    {
        // check for 1 arg
        if (args.size() != 1)
        {
            this->sendPacket(error, {"ARG_ERR", "Packet type \"chat\" requires 1 argument"});
            return ERROR;
        }

        // todo: send message to all players
        return OK;
    }

    this->sendPacket(error, {"UNKNOWN_TYPE", "Unknown packet type"});
    return ERROR;
}

void Session::sendPacket(ServerPacketType type, vector<string> args)
{
    // construct packet
    string packet = to_string(type) + "|";
    for (string arg : args)
    {
        packet += arg + " ";
    }
    packet += "\n";

    // send packet
    do_write(packet);
}

string Session::to_string(ServerPacketType type)
{
    switch (type)
    {
    case motd:
        return "motd";
    case error:
        return "error";
    case game:
        return "game";
    case pos:
        return "pos";
    case player:
        return "player";
    case tick:
        return "tick";
    case die:
        return "die";
    case message:
        return "message";
    case win:
        return "win";
    case lose:
        return "lose";
    default:
        return "unknown";
    }
}
