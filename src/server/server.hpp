#pragma once

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string;
using std::stringstream;
using std::vector;

// forward declarations
class Session;

#include "user.hpp"

#include "game.hpp"
namespace game
{
    class Player;
    class Game;
}
using game::Game;
using game::Player;

#include "../helpers.hpp"
using namespace helpers;

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
    vector<Session *> sessions_;
    // running game
    Game *m_game;

public:
    UserDatabase *m_user_db;

public:
    Server(boost::asio::io_context &io_context, short port, UserDatabase *user_db);

    ~Server();

private:
    void do_accept();

public:
    void sendPacketToAll(ServerPacketType type, vector<string> args);
    void sendPacketToAllPlayers(ServerPacketType type, vector<string> args);
    static string to_string(ServerPacketType type);

    bool isPlayerLoggedIn(string username);
    unsigned int getNumPlayersLoggedIn();
    void eraseSession(Session *session);

    void startGame();
    bool isGameRunning();
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
    Session(tcp::socket socket, Server *m_server);

    ~Session();

    void start();

    void sendPacket(ServerPacketType type, vector<string> args);

    // utility functions
    bool hasJoined();
    Player *getPlayer();

private:
    void do_read();

    void do_write(string data);

    ProcessErrorCode process(string data);

    // process client packets
    ProcessErrorCode processJoin(string username, string password);
    ProcessErrorCode processMove(string direction);
    ProcessErrorCode processChat(Player *player, string message);

    ProcessErrorCode checkCredentials(string username, string password);
};