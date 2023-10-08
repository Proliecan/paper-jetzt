#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "../helpers.hpp"
#include "server.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace helpers;

// forward declare
class Server;

namespace game
{

    enum move
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Player
    {
    public:
        struct position
        {
            int x;
            int y;
        };

    private:
        string m_name;
        string m_color;

        position pos;
        vector<position> *trace;
        move nextMove;

    public:
        Player(string i_name, string i_color)
            : m_name(i_name),
              m_color(i_color),
              trace(new vector<position>){};

        string getName() { return m_name; };
        string getColor() { return m_color; };
        position getPos() { return pos; };
        vector<position> *getTrace() { return trace; };
        void setPos(position i_pos) { pos = i_pos; };
        void setNextMove(move i_move) { nextMove = i_move; };
        move getNextMove() { return nextMove; };
    };

    class Game
    {
    private:
        unsigned int width;
        unsigned int height;
        vector<Player> *players;
        Server *m_server;
        unsigned int sleepTime = 1;

    public:
        Game(unsigned int width, unsigned int height, vector<Player> *players, Server *m_server)
            : width(width),
              height(height),
              players(players),
              m_server(m_server) {};

        Game(vector<Player> *players, Server *m_server)
            : width(10 * players->size()),
              height(10 * players->size()),
              players(players),
              m_server(m_server) {};

        unsigned int getWidth() { return width; };
        unsigned int getHeight() { return height; };
        vector<Player> *getPlayers() { return players; };

        bool isRunning() { return true; };

    public:
        void start();
        void gameLoop();
    };

}