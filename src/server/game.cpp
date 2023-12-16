#include "game.hpp"
using namespace game;

Game::fieldState *Game::getFieldState(int x, int y)
{
    return &(*map)[y][x];
}

void Game::setFieldState(int x, int y, fieldState state)
{
    (*map)[y][x] = state;
}

void Game::purgePlayers(span<Player *const> players)
{
    // iterate over known positions of player and over whole map and remove players from map
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (Player *p : players)
            {
                if (getFieldState(x, y)->player == p)
                {
                    setFieldState(x, y, {false, false, nullptr});
                }
            }
        }
    }
}

void Game::start()
{
    Logger::ln("Game started", Logger::normal, green);
    Logger::ln("[" + std::to_string(width) + "x" + std::to_string(height) + "]", Logger::normal, blue);

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
        // send pos packets
        vector<headPosition> headPositions = getAllHeadPositions();
        for (headPosition headPosition : headPositions)
        {
            m_server->sendPacketToAllPlayers(ServerPacketType::pos, {headPosition.player->getName(), std::to_string(headPosition.pos->x), std::to_string(headPosition.pos->y)});
        }

        // sleep for 1 second
        sleep(sleepTime);

        // move players
        vector<headPosition> newHeadPositions;
        for (headPosition headPosition : headPositions)
        {
            // calculate new position based on next move
            newHeadPositions.push_back(getNewHeadPosition(headPosition));
        }
        // for all new positions
        for (headPosition newHeadPosition : newHeadPositions)
        {
            // check for collision by looking up in map and comparing all new positions
            collisionType collision = checkCollision(newHeadPosition);
            // if collision push player to diedPlayers vector
            if (collision != collisionType::NONE)
            {
                diedPlayers.push_back(newHeadPosition.player);
                continue;
            }
            // set new position in map
            setFieldState(newHeadPosition.pos->x, newHeadPosition.pos->y, {false, true, newHeadPosition.player});
            // old position is now tail
            // TODO: use hashmaps for head positions
            headPosition oldHeadPosition;
            for (headPosition headPosition : headPositions)
            {
                if (headPosition.player == newHeadPosition.player)
                {
                    oldHeadPosition = headPosition;
                }
            }
            setFieldState(oldHeadPosition.pos->x, oldHeadPosition.pos->y, {false, false, newHeadPosition.player});
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
            purgePlayers(diedPlayers);

            // empty diedPlayers vector
            diedPlayers.clear();
        }
    }
}

Game::headPosition Game::getNewHeadPosition(Game::headPosition headPosition)
{
    // get Info
    move nextMove = headPosition.player->getNextMove();
    position *pos = new Game::position{headPosition.pos->x, headPosition.pos->y};

    // calculate new position
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

    // return new position
    return {headPosition.player, pos};
}

vector<Game::headPosition> Game::getAllHeadPositions()
{
    vector<headPosition> headPositions;

    // TODO: optimize this
    // iterate over map and find player
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            if (getFieldState(x, y)->isHead)
            {
                Player *player = getFieldState(x, y)->player;

                if (player != nullptr)
                {
                    headPositions.push_back({player, new position{x, y}});
                }
            }
        }
    }

    return headPositions;
}

Game::collisionType Game::checkCollision(headPosition newHeadPosition)
{
    collisionType collision = collisionType::NONE;
    // check for out of bounds
    if (newHeadPosition.pos->x < 0 || newHeadPosition.pos->x >= width || newHeadPosition.pos->y < 0 || newHeadPosition.pos->y >= height)
    {
        collision = collisionType::OUT_OF_BOUNDS;
    }
    return collision;
}