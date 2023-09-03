#include "game.hpp"
using namespace game;

void Game::start(){
    cout << "Game started" << endl;
    cout << "Width: " << width << endl;
    cout << "Height: " << height << endl;
    cout << "Players: " << endl;
    for (unsigned int i = 0; i < players->size(); i++)
    {
        cout << "  " << players->at(i).getName() << endl;
    }   
}