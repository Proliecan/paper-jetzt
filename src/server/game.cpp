#include "game.hpp"
using namespace game;

#pragma region Map-bitmasking

Game::fieldState *Game::getFieldState(int x, int y)
{
    return &(*map)[y][x];
}

void Game::setFieldState(int x, int y, fieldState state)
{
    (*map)[y][x] = state;
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
        while (!(getFieldState(x, y)->player == nullptr))
        {
            x = rand() % width;
            y = rand() % height;
        }
        setFieldState(x, y, {false, true, &player});
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
                if (getFieldState(x, y)->isHead)
                {
                    Player *player = getFieldState(x, y)->player;

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

    // previous field is now tail
    setFieldState(pos->x, pos->y, {false, false, player});

    // get next move
    move nextMove = player->getNextMove();

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
    setFieldState(pos->x, pos->y, {false, true, player});
}

Game::position *Game::getPlayerPosition(Player *player)
{
    // iterate over map and find player
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (getFieldState(x, y)->player == player)
            {
                return new position{x, y};
            }
        }
    }

    return nullptr;
}