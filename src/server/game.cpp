#include "game.hpp"
using namespace game;

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
