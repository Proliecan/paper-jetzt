#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "../helpers.hpp"

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
              trace(new vector<position>)
        {
            // Print player info
            color c = nearestColorFromHexCode(i_color);
            cout << colorize("Player " + i_name + " joined the game", c) << endl;
            cout << colorize("Color: " + i_color, c) << endl;
        };

        string getName() { return m_name; };
        string getColor() { return m_color; };
        position getPos() { return pos; };
        vector<position> *getTrace() { return trace; };
    };

    class Game
    {
    private:
        int width;
        int height;
        vector<Player> *players;

    public:
        Game(int width, int height, vector<Player> *players)
            : width(width),
              height(height),
              players(players){};

        Game(vector<Player> *players)
            : width(10*players->size()),
              height(10*players->size()),
              players(players){};

        int getWidth() { return width; };
        int getWeight() { return height; };
        vector<Player> *getPlayers() { return players; };

        bool isRunning() { return true; };

    public:
        void start();
    };

}