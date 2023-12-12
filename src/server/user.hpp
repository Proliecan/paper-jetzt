#pragma once

#include <string>
#include <map>

using string = std::string;

namespace server{
    class User
    {
        string username_;
        string password_;
    };

    class UserDatabase
    {
        string filepath_;
        std::map<string, string> *users_;

    public:
        UserDatabase(string filepath)
            : filepath_(filepath)
        {
            loadFromFile(filepath);
        };
        ~UserDatabase(){};

        void loadFromFile(string filepath);
        void addUser(string username, string password);
        bool userExists(string username);
        bool checkPassword(string username, string password);
        string hashPassword(string password);
    };
};