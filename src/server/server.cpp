#include "server.hpp"

Server::Server(boost::asio::io_context &io_context, short port, UserDatabase *user_db)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      sessions_(),
      m_user_db(user_db)
{
    cout << "Server listening on port " << colorize(std::to_string(port), cyan) << endl;

    do_accept();
}

Server::~Server()
{
    cout << "Server shutting down" << endl;
}

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

void Server::sendPacketToAllPlayers(ServerPacketType type, vector<string> args)
{
    // send packet to all players
    for (Session *session : sessions_)
    {
        if (session->hasJoined())
        {
            session->sendPacket(type, args);
        }
    }
}

void Server::startGame()
{
    // create players vector
    vector<Player> *players = new vector<Player>;
    for (Session *session : sessions_)
    {
        if (session->hasJoined())
        {
            players->push_back(*session->getPlayer());
        }
    }

    // create game
    Game *game = new Game(players, this);

    // set game
    m_game = game;

    // send game packet to all sessions
    vector<string> args;
    args.push_back(std::to_string(game->getWidth()));
    args.push_back(std::to_string(game->getHeight()));
    sendPacketToAllPlayers(game_pkg, args);

    // start game
    game->start();
}

bool Server::isGameRunning()
{
    if (m_game == nullptr)
        return false;
    return m_game->isRunning();
};

bool Server::isPlayerLoggedIn(string username)
{
    for (Session *session : sessions_)
    {
        if (session->hasJoined() && session->getPlayer()->getName() == username)
        {
            return true;
        }
    }
    return false;
}

unsigned int Server::getNumPlayersLoggedIn()
{
    int num = 0;
    for (Session *session : sessions_)
    {
        if (session->hasJoined())
        {
            num++;
        }
    }
    return num;
}

void Server::eraseSession(Session *session)
{
    // find session
    for (unsigned int i = 0; i < sessions_.size(); i++)
    {
        if (sessions_[i] == session)
        {
            // erase session
            sessions_.erase(sessions_.begin() + i);
            return;
        }
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
                                    process(data_str);

                                    do_read();
                                }
                                else
                                {
                                    // if error is eof
                                    if (ec == boost::asio::error::eof)
                                    {
                                        // delete session
                                        delete this;
                                        return;
                                    }
                                    // log the error information
                                    cout << colorize("Error on session with ", color::red)
                                         << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl
                                         << colorize("Error: ", color::red) << ec.message() << endl;
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

                                     // check existence of endpoint
                                     try
                                     {
                                         socket_.remote_endpoint();
                                         cout << colorize("<", color::green) << " "
                                              << colorize(socket_.remote_endpoint().address().to_string(), cyan) << ": "
                                              << data_str << endl;
                                     }
                                     catch (boost::system::system_error &e)
                                     {
                                         // delete session
                                         std::cerr << colorize("Sending data failed: ", color::red) << e.what() << endl;
                                         return;
                                     }
                                 }
                             });
}

Session::Session(tcp::socket socket, Server *m_server)
    : socket_(std::move(socket)),
      m_server(m_server),
      m_player(nullptr)
{
    cout << colorize("New session with", color::yellow) << " " << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl;
}

Session::
    ~Session()
{

    // check socket endpoint
    try
    {
        socket_.remote_endpoint();
        cout << colorize("Ending session with ", color::yellow) << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl;
    }
    catch (const boost::system::system_error &e)
    {
        cout << colorize("Ending session with ", color::yellow) << colorize("unknown", cyan) << endl;
    }

    socket_.close();
    // remove myself from server sessions list
    m_server->eraseSession(this);
    delete m_player;
}

void Session::start()
{
    do_read();
}

Player *Session::getPlayer()
{
    return m_player;
}

ProcessErrorCode Session::process(string data)
{
    // split packet type and arguments
    // <ptype> | <arg1> | <arg2> ... // ignore whitespace
    stringstream stream(data);
    string ptype;
    vector<string> args;

    // get packet type
    getline(stream, ptype, '|');

    // get arguments
    string arg;
    while (getline(stream, arg, '|'))
    {
        args.push_back(arg);
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

        processChat(m_player, args[0]);
        return OK;
    }

    this->sendPacket(error, {"UNKNOWN_TYPE", "Unknown packet type"});
    return ERROR;
}

void Session::sendPacket(ServerPacketType type, vector<string> args)
{
    // construct packet
    string packet = Server::to_string(type);
    for (string arg : args)
    {
        packet += "|" + arg;
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
            cout << "Existing user"
                 << " " << colorize(username, cyan)
                 << endl;
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
    cout << "New user"
         << " " << colorize(username, cyan)
         << endl;

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
    if (m_server->isPlayerLoggedIn(username))
    {
        this->sendPacket(error, {"ALREADY_JOINED", "You joined with this username on another session"});
        return ERROR;
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

    // if there are 2 or more players and no game is running, one should start
    if (m_server->getNumPlayersLoggedIn() >= 2 && !m_server->isGameRunning())
    {
        // start game in new thread
        std::thread t(&Server::startGame, m_server);
        t.detach();
    }

    return OK;
}

ProcessErrorCode Session::processMove(string /* direction */)
{
    // send error packet
    this->sendPacket(error, {"NOT_IMPLEMENTED", "Packet type \"move\" is not implemented yet"});
    return ERROR;
}

ProcessErrorCode Session::processChat(Player *player, string message)
{
    m_server->sendPacketToAll(ServerPacketType::message, {player->getName(), message});
    return ERROR;
}