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

namespace game
{
    class Player
    {
    private:
        string m_name;
        string m_color;

        struct position
        {
            int x;
            int y;
        };

        position pos;
        vector<position> *trace;

    public:
        Player(string i_name, string i_color)
            : m_name(i_name),
              m_color(i_color),
              trace(new vector<position>){};

        string getName() { return m_name; };
        string getColor() { return m_color; };
        position getPos() { return pos; };
        vector<position> *getTrace() { return trace; };
    };

    class Game
    {
    private:
        unsigned int width;
        unsigned int height;
        vector<Player> *players;

    public:
        Game(unsigned int width, unsigned int height, vector<Player> *players)
            : width(width),
              height(height),
              players(players){};

        Game(vector<Player> *players)
            : width(10 * players->size()),
              height(10 * players->size()),
              players(players){};

        unsigned int getWidth() { return width; };
        unsigned int getHeight() { return height; };
        vector<Player> *getPlayers() { return players; };

        bool isRunning() { return true; };

    public:
        void start();
        void gameLoop();
    };

}