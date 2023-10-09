#include "game.hpp"
using namespace game;

#pragma region Map-bitmasking

Player *Game::getPlayerAtPosition(int x, int y)
{
    short unsigned int value = (*map)[y][x];

    short unsigned int playerId = (value & 0b0000000011111111) - 1; // -1 because 0 is empty
    return &(*players)[playerId];
}

bool Game::isPositionFree(int x, int y)
{
    short unsigned int value = (*map)[y][x];
    return (value & 0b0000000011111111) == 0;
}

bool Game::isPositionTerritory(int x, int y)
{
    short unsigned int value = (*map)[y][x];
    return (value & 0b0000001000000000) > 0;
}

bool Game::isPositionHead(int x, int y)
{
    short unsigned int value = (*map)[y][x];
    return (value & 0b0000000100000000) > 0;
}

void Game::setPositionPlayer(int x, int y, Player *player)
{
    // determine player id
    short unsigned int playerId = 0;
    for (unsigned int i = 0; i < players->size(); i++)
    {
        if (&(*players)[i] == player)
        {
            playerId = i + 1; // +1 because 0 is empty
            break;
        }
    }

    // get value from map
    short unsigned int value = (*map)[y][x];
    //combine player id with value
    value = (value & 0b1111111100000000) | playerId;

    // set value in map
    (*map)[y][x] = value;
}

void Game::setPositionIsHead(int x, int y, bool isHead)
{
    // get value from map
    short unsigned int value = (*map)[y][x];
    // set bit 8 to isHead
    value = (value & 0b1111111011111111) | (isHead << 8);

    // set value in map
    (*map)[y][x] = value;
}

void Game::setPositionIsTerritory(int x, int y, bool isTerritory)
{
    // get value from map
    short unsigned int value = (*map)[y][x];
    // set bit 9 to isTerritory
    value = (value & 0b1111110111111111) | (isTerritory << 9);

    // set value in map
    (*map)[y][x] = value;
}

#pragma endregion

void Game::start()
{
    cout << colorize("Game started", color::green) + " " + colorize("[" + std::to_string(width) + "x" + std::to_string(height) + "]", color::blue) << endl;

    // Initialize player positions
    // for (Player &player : *players)
    // {
        cout << colorize("Player initialization not implemented", color::red) << endl;
    // }

    // Start game loop
    gameLoop();
}

void Game::gameLoop()
{
    while (true)
    {
        // send game state to clients
        // send all players the position of all players
        for (Player &player : *players)
        {
            if (!player.getIsAlive())
            {
                continue;
            }
            cout << colorize("Pos packet not implemented", color::red) << endl;
        }

        // sleep for 1 second
        sleep(sleepTime);

        // vector for died players
        vector<Player *> diedPlayers;

        // make moves
        for (Player &player : *players)
        {
            if (!player.getIsAlive())
            {
                continue;
            }
            // move player
            cout << colorize("player movement not implemented", color::red) << endl;

            // check for collision
            

            // update player position
        }

        // send game tick
        m_server->sendPacketToAllPlayers(ServerPacketType::tick, {});

        // check for died players
        if (diedPlayers.size() > 0)
        {
            // send die packet
            vector<string> diedPlayerNames;
            for (Player *player : diedPlayers)
            {
                diedPlayerNames.push_back(player->getName());
            }
            m_server->sendPacketToAllPlayers(ServerPacketType::die, diedPlayerNames);
        }
    }
}
