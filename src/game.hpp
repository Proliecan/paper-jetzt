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
        Player(string i_name, string i_color, position start_pos)
            : m_name(i_name),
              m_color(i_color),
              pos(start_pos),
              trace(new vector<position>)
        {
            trace->push_back(start_pos);

            // Print player info
            color c = nearestColorFromHexCode(i_color);
            cout << colorize("Player " + i_name + " joined the game", c) << endl;
            cout << colorize("Color: " + i_color, c) << endl;
            cout << colorize("Position: (" + std::to_string(start_pos.x) + ", " + std::to_string(start_pos.y) + ")", c) << endl;
        };

        string get_name() { return m_name; };
        string get_color() { return m_color; };
        position get_pos() { return pos; };
        vector<position> *get_trace() { return trace; };
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

        int get_width() { return width; };
        int get_height() { return height; };
        vector<Player> *get_players() { return players; };

        void add_player(Player p) { players->push_back(p); };
    };
}