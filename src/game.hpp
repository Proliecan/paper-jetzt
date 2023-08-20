#pragma once

#include <string>
#include <vector>

#include "helpers.hpp"

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
        Game(int width, int height)
            : width(width),
              height(height),
              players(new vector<Player>){};

        int getWidth() { return width; };
        int getWeight() { return height; };
        vector<Player> *getPlayers() { return players; };

        void addPlayer(Player p) { players->push_back(p); };
    };
}