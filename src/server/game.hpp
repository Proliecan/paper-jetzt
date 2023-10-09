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

        struct position
        {
            int x;
            int y;
        };
        struct fieldState
        {
            bool isTerritory = false;
            bool isHead = false;
            Player *player = nullptr;

            // constructor
            fieldState(bool i_isTerritory, bool i_isHead, Player *i_player)
                : isTerritory(i_isTerritory),
                  isHead(i_isHead),
                  player(i_player){};
        };
        struct field
        {
            position pos;
            fieldState state;
        };

        vector<vector<fieldState>> *map;

        /// @brief vector of players that died in the last round
        vector<Player *> diedPlayers;

    public:
        Game(unsigned int width, unsigned int height, vector<Player> *players, Server *m_server)
            : width(width),
              height(height),
              players(players),
              m_server(m_server),
              map(new vector<vector<fieldState>>(width, vector<fieldState>(height, fieldState(false, false, nullptr)))){};

        Game(vector<Player> *players, Server *m_server)
            : width(10 * players->size()),
              height(10 * players->size()),
              players(players),
              m_server(m_server),
              map(new vector<vector<fieldState>>(width, vector<fieldState>(height, fieldState(false, false, nullptr)))){};

        unsigned int getWidth() { return width; };
        unsigned int getHeight() { return height; };
        bool isRunning() { return true; };
        vector<Player> *getPlayers() { return players; };

        vector<vector<fieldState>> *getMap() { return map; };

    public:
        void start();
        void gameLoop();

    private:
        fieldState *getFieldState(int x, int y);

        void setFieldState(int x, int y, fieldState state);

        void movePlayer(Player *player);
        position *getPlayerPosition(Player *player);
    };

}