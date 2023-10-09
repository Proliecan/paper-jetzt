#include "game.hpp"
using namespace game;

#pragma region Map-bitmasking

Player *Game::getPlayerAtPosition(int x, int y)
{
    short unsigned int value = (*map)[y][x];

    short unsigned int playerVal = (value & 0b0000000011111111); // -1 because 0 is empty

    // check if playerVal is 0
    if (playerVal == 0)
    {
        return nullptr;
    }

    return &(*players)[playerVal - 1];
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
    // combine player id with value
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
    for (Player &player : *players)
    {
        // set player position
        int x = rand() % width;
        int y = rand() % height;
        while (!isPositionFree(x, y))
        {
            x = rand() % width;
            y = rand() % height;
        }
        setPositionPlayer(x, y, &player);
        setPositionIsHead(x, y, true);
        player.setIsAlive(true);
    }

    // Start game loop
    gameLoop();
}

void Game::gameLoop()
{
    while (true)
    {
        // iterate over map and send head positions to all players
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (isPositionHead(x, y))
                {
                    Player *player = getPlayerAtPosition(x, y);

                    if (player != nullptr)
                    {
                        // send head position to player
                        m_server->sendPacketToAllPlayers(ServerPacketType::pos, {player->getName(), std::to_string(x), std::to_string(y)});
                    }
                }
            }
        }

        // sleep for 1 second
        sleep(sleepTime);

        // make moves
        for (Player &player : *players)
        {
            if (!player.getIsAlive())
            {
                continue;
            }

            // move player
            movePlayer(&player);
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

            // remove died players from map

            // empty diedPlayers vector
            diedPlayers.clear();
        }
    }
}

void Game::movePlayer(Player *player)
{
    position *pos = getPlayerPosition(player);

    if (pos == nullptr)
    {
        return;
    }

    // get next move
    move nextMove = player->getNextMove();

    // set current position to tail
    setPositionIsHead(pos->x, pos->y, false);

    // move player
    switch (nextMove)
    {
    case UP:
        pos->y--;
        break;
    case DOWN:
        pos->y++;
        break;
    case LEFT:
        pos->x--;
        break;
    case RIGHT:
        pos->x++;
        break;

    default:
        break;
    }

    // collision tests
    // Out of bounds
    if (pos->x < 0 || pos->x >= width || pos->y < 0 || pos->y >= height)
    {
        player->setIsAlive(false);
        diedPlayers.push_back(player);
        return;
    }

    // set new head position
    setPositionPlayer(pos->x, pos->y, player);
    setPositionIsHead(pos->x, pos->y, true);
    setPositionIsTerritory(pos->x, pos->y, false);
}

Game::position *Game::getPlayerPosition(Player *player)
{
    // iterate over map and find player
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            if (getPlayerAtPosition(x, y) == player)
            {
                return new position{x, y};
            }
        }
    }

    return nullptr;
}