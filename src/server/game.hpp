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
    private:
        string m_name;
        string m_color;

        move nextMove;

        bool isAlive = true;

    public:
        Player(string i_name, string i_color)
            : m_name(i_name),
              m_color(i_color){};

        string getName() { return m_name; };
        string getColor() { return m_color; };
        void setNextMove(move i_move) { nextMove = i_move; };
        move getNextMove() { return nextMove; };
        bool getIsAlive() { return isAlive; };
        void setIsAlive(bool i_isAlive) { isAlive = i_isAlive; };
    };

    class Game
    {
    private:
        int width;
        int height;
        vector<Player> *players;
        Server *m_server;
        unsigned int sleepTime = 1;

        // bit layout of value:
        // bits 0-7: player id      // max players: 255
        // bit 8: is head
        // bit 9: is territory
        // bit 10: is wall          // unused
        // bit 11: is food          // unused
        // bit 12-15: not used      // unused
        vector<vector<short int>> *map;

    public:
        Game(unsigned int width, unsigned int height, vector<Player> *players, Server *m_server)
            : width(width),
              height(height),
              players(players),
              m_server(m_server),
              map(new vector<vector<short int>>(width, vector<short int>(height, 0))){};

        Game(vector<Player> *players, Server *m_server)
            : width(10 * players->size()),
              height(10 * players->size()),
              players(players),
              m_server(m_server),
              map(new vector<vector<short int>>(width, vector<short int>(height, 0))){};

        unsigned int getWidth() { return width; };
        unsigned int getHeight() { return height; };
        bool isRunning() { return true; };
        vector<Player> *getPlayers() { return players; };

        vector<vector<short int>> *getMap() { return map; };

    public:
        void start();
        void gameLoop();

    private:
        Player *getPlayerAtPosition(int x, int y);
        bool isPositionFree(int x, int y);
        bool isPositionTerritory(int x, int y);
        bool isPositionHead(int x, int y);

        void setPositionPlayer(int x, int y, Player *player);
        void setPositionIsHead(int x, int y, bool isHead);
        void setPositionIsTerritory(int x, int y, bool isTerritory);
    };

}