#include "user.hpp"
#include "../helpers.hpp"

#include <fstream>

using string = std::string;
using namespace helpers;
using server::UserDatabase;

void UserDatabase::loadFromFile(string filepath)
{
    Logger::ln("Loading users from " + colorize(filepath, color::cyan), Logger::normal);
    std::ifstream user_file(filepath);
    // assert file is open
    if (!user_file.is_open())
    {
        throw std::runtime_error("Could not open user file");
    }

    // new user map
    users_ = new std::map<string, string>();

    // read lines from file with format: username$passwordHash
    string line;
    while (std::getline(user_file, line))
    {
        // ignore empty lines
        if (line.empty())
        {
            continue;
        }

        // find position of $ in line
        size_t pos = line.find('$');
        // assert that $ is found
        if (pos == string::npos)
        {
            throw std::runtime_error("Invalid user file format");
        }
        // extract username and password hash
        string username = line.substr(0, pos);
        string password_hash = line.substr(pos + 1);
        // add user to database map
        users_->insert({username, password_hash});
    }

    // log number of users loaded
    Logger::ln("Loaded " + std::to_string(users_->size()) + " users", Logger::normal);
}

void UserDatabase::addUser(string username, string password)
{
    // add user to map
    users_->insert({username, hashPassword(password)});

    // append user to file
    std::ofstream user_file(filepath_, std::ios_base::app);
    user_file << username << "$" << hashPassword(password) << std::endl;
}

bool UserDatabase::userExists(string username)
{
    // lookup user
    auto it = users_->find(username);
    // return true if user exists
    return it != users_->end();
}

bool UserDatabase::checkPassword(string username, string password)
{
    string password_hash = users_->at(username);
    return password_hash == hashPassword(password);
}

string UserDatabase::hashPassword(string password) { return password; }