#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "game.hpp"

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using std::string;
using game::Game;
using game::Player;

class Server
{
private:
    tcp::acceptor acceptor_;

public:
    Server(boost::asio::io_context &io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        cout << "Server listening on port " << colorize(std::to_string(port), cyan) << endl;

        // create new game
        new Game(10, 10);

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

private:
    tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];

    // player data
    Player* m_player;

public:
    Session(tcp::socket socket, Player* player)
        : socket_(std::move(socket)),
          m_player(player)
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

private:
    void do_read();

    void do_write(std::size_t length);
};