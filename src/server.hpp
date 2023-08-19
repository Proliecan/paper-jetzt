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
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;
using game::Game;
using game::Player;

// forward declarations
class Session;

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
};

class Session
    : public std::enable_shared_from_this<Session>
{
public:
    enum ProcessErrorCode
    {
        OK,
        ERROR
    };

    enum ServerPacketType
    {
        motd,
        error,
        game,
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

private:
    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };

    // player data
    Player* m_player;

public:
    Session(tcp::socket socket)
        : socket_(std::move(socket)),
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

    ProcessErrorCode process(string data);

private:
    void do_read();

    void do_write(string data);

    void sendPacket(ServerPacketType type, vector<string> args);

    string to_string(ServerPacketType type);

    // process client packets
    ProcessErrorCode processJoin(string /* username */, string /* password */);
    ProcessErrorCode processMove(string /* direction */);
    ProcessErrorCode processChat(string /* message */);
};