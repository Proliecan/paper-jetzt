#include "game.hpp"
using server::game::Game;
using server::Server;

void server::game::Game::start()
{
    Logger::ln("Game started", Logger::normal, green);
    Logger::ln("[" + std::to_string(width) + "x" + std::to_string(height) + "]", Logger::normal, blue);

    // Initialize player positions
    for (Player &player : *players)
    {
        Player::position pos;
        pos.x = rand() % width;
        pos.y = rand() % height;
        player.setPos(pos);
    }

    // Start game loop
    gameLoop();
}

void server::game::Game::gameLoop()
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
            m_server->sendPacketToAllPlayers(ServerPacketType::pos, {player.getName(), std::to_string(player.getPos().x), std::to_string(player.getPos().y)});
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
            Player::position newPos = player.getPos();
            switch (player.getNextMove())
            {
            case move::UP:
                newPos.y--;
                break;
            case move::DOWN:
                newPos.y++;
                break;
            case move::LEFT:
                newPos.x--;
                break;
            case move::RIGHT:
                newPos.x++;
                break;
            default:
                break;
            }

            // check for collision
            if (newPos.x < 0 || newPos.x >= width || newPos.y < 0 || newPos.y >= height)
            {
                // out of bounds
                // kill player
                diedPlayers.push_back(&player);
                player.setIsAlive(false);

                continue;
            }

            // update player position
            player.setPos(newPos);
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
