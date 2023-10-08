#include "game.hpp"
using namespace game;

void Game::start()
{
    cout << colorize("Game started", color::green) + " " + colorize("[" + std::to_string(width) + "x" + std::to_string(height) + "]", color::blue) << endl;

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

void Game::gameLoop()
{
    while (true)
    {
        // send game state to clients
        // send all players the position of all players
        for (Player &player : *players)
        {
            m_server->sendPacketToAllPlayers(ServerPacketType::pos, {player.getName(), std::to_string(player.getPos().x), std::to_string(player.getPos().y)});
        }

        // sleep for 1 second
        sleep(sleepTime);

        // make moves
        for (Player &player : *players)
        {
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
            // if collision, remove player
            // else, update player position
            player.setPos(newPos);
        }

        // send game tick
        m_server->sendPacketToAllPlayers(ServerPacketType::tick, {});
    }
}