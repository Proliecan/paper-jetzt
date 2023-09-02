#include "server.hpp"

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                // create new session
                // on heap
                Session *session = new Session(std::move(socket), this);
                sessions_.push_back(session);

                // start session
                session->start();
            }

            do_accept();
        });
}

string Server::to_string(ServerPacketType type)
{
    switch (type)
    {
    case motd:
        return "motd";
    case error:
        return "error";
    case game_pkg:
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

void Server::sendPacketToAll(ServerPacketType type, vector<string> args)
{
    // send packet to all sessions
    for (Session *session : sessions_)
    {
        session->sendPacket(type, args);
    }
}

void Session::do_read()
{
    char *data_ = new char[max_length];
    // clear data
    memset(data_, 0, max_length);

    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, data_](boost::system::error_code ec, std::size_t /* length */)
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
    // calc length of data
    std::size_t length = data.length();
    boost::asio::async_write(socket_, boost::asio::buffer(data.c_str(), length),
                             [this, data](boost::system::error_code ec, std::size_t /*length*/)
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

ProcessErrorCode Session::process(string data)
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

        processJoin(args[0], args[1]);
        return OK;
    }
    else if (ptype == "move")
    {
        // check if player has joined
        if (!hasJoined())
        {
            this->sendPacket(error, {"NOT_JOINED", "You have not joined the game yet"});
            return ERROR;
        }
        // check for one arg that has to be one of up, right, down or left
        if (args.size() != 1 || (args[0] != "up" && args[0] != "right" && args[0] != "down" && args[0] != "left"))
        {
            this->sendPacket(error, {"ARG_ERR", "Packet type \"move\" requires 1 argument that has to be one of up, right, down or left"});
            return ERROR;
        }

        processMove(args[0]);
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

        // check if player has joined
        if (!hasJoined())
        {
            this->sendPacket(error, {"NOT_JOINED", "You have not joined the game yet"});
            return ERROR;
        }

        processChat(args[0]);
        return OK;
    }

    this->sendPacket(error, {"UNKNOWN_TYPE", "Unknown packet type"});
    return ERROR;
}

void Session::sendPacket(ServerPacketType type, vector<string> args)
{
    // construct packet
    string packet = Server::to_string(type) + "|";
    for (string arg : args)
    {
        packet += arg + " ";
    }
    packet += "\n";

    // send packet
    do_write(packet);
}

bool Session::hasJoined()
{
    return m_player != nullptr;
}

ProcessErrorCode Session::checkCredentials(string username, string password)
{
    if (m_server->m_user_db->userExists(username))
    {

        // check password
        if (m_server->m_user_db->checkPassword(username, password))
        {
            // print debug message
            cout << colorize("Existing user", color::yellow) << " " << colorize(username, cyan) << " "
                 << colorize("joined", color::yellow) << endl;
            return OK;
        }
        else
        {
            return ERROR;
        }
    }

    // add user
    m_server->m_user_db->addUser(username, password);

    // print debug message
    cout << colorize("New user", color::yellow) << " " << colorize(username, cyan) << " "
         << colorize("joined", color::yellow) << endl;

    return OK;
}

ProcessErrorCode Session::processJoin(string username, string password)
{
    if (hasJoined())
    {
        // send error packet
        this->sendPacket(error, {"ALREADY_JOINED", "You have already joined"});
        return ERROR;
    }

    // if there is already another session with this username, send error packet
    for (Session *session : m_server->sessions_)
    {
        if (session->hasJoined() && session->m_player->getName() == username)
        {
            this->sendPacket(error, {"ALREADY_JOINED", "You joined with this username on another session"});
            return ERROR;
        }
    }

    // check credentials
    ProcessErrorCode ec = checkCredentials(username, password);

    if (ec == ERROR)
    {
        // send error packet
        this->sendPacket(error, {"AUTH_ERR", "Invalid username or password"});
        return ERROR;
    }

    // create player
    m_player = new Player(username, helpers::randomHexColor());

    return OK;
}

ProcessErrorCode Session::processMove(string /* direction */)
{
    // send error packet
    this->sendPacket(error, {"NOT_IMPLEMENTED", "Packet type \"move\" is not implemented yet"});
    return ERROR;
}

ProcessErrorCode Session::processChat(string message)
{
    m_server->sendPacketToAll(ServerPacketType::message, {message});
    return ERROR;
}