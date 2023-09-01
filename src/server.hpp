#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <boost/asio.hpp>

#include "game.hpp"

using boost::asio::ip::tcp;
using game::Game;
using game::Player;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

// forward declarations
class Session;

enum ProcessErrorCode
{
    OK,
    ERROR
};

enum ServerPacketType
{
    motd,
    error,
    game_pkg,
    pos,
    player,
    tick,
    die,
    message,
    win,
    lose
};

// enum ClientPacketType // not used rn
// {
//     join,
//     move,
//     chat
// };

class Server
{
private:
    tcp::acceptor acceptor_;
    vector<std::shared_ptr<Session>> sessions_;

public:
    Server(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          sessions_()
    {
        cout << "Server listening on port " << colorize(std::to_string(port), cyan) << endl;

        do_accept();
    }

    ~Server()
    {
        cout << "Server shutting down" << endl;
    }

private:
    void do_accept();

public:
    void sendPacketToAll(ServerPacketType type, vector<string> args);
    static string to_string(ServerPacketType type);

};

class Session
    : public std::enable_shared_from_this<Session>
{
private:
    tcp::socket socket_;
    Server *m_server;

    enum
    {
        max_length = 1024
    };

    // player data
    Player *m_player;

public:
    Session(tcp::socket socket, Server *m_server)
        : socket_(std::move(socket)),
          m_server(m_server),
          m_player(nullptr)
    {
        cout << colorize("New session with", color::yellow) << " " << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl;
    }

    ~Session()
    {
        cout << colorize("Ending session with ", color::yellow) << colorize(socket_.remote_endpoint().address().to_string(), cyan) << endl;
    }

    void start()
    {
        do_read();
    }

    void sendPacket(ServerPacketType type, vector<string> args);

private:
    void do_read();

    void do_write(string data);

    ProcessErrorCode process(string data);


    // process client packets
    ProcessErrorCode processJoin(string username, string password);
    ProcessErrorCode processMove(string /* direction */);
    ProcessErrorCode processChat(string /* message */);

    ProcessErrorCode checkCredentials(string /* username */, string /* password */);

    // utility functions
    bool hasJoined();
};