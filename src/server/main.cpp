#include <fstream>
#include <vector>
#include <thread>

#include "server.hpp"
#include "user.hpp"

using std::string;
using std::vector;

// forward declarations
void print_usage(char *argv[]);
UserDatabase *init_user_db(string db_file_path);

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 3 || argc > 4)
        {
            print_usage(argv);
            return 1;
        }

        bool verbose = false;
        short port;
        string user_file_path;

        int token_counter = 0;

        for (int i = 1; i < argc; ++i)
        {
            // check if flag
            if (argv[i][0] == '-')
            {
                if (strcmp(argv[i], "-v") == 0)
                {
                    verbose = true;
                }
            } else
            {
                // is token aka. Parameter
                switch (token_counter)
                {
                case 0:
                    port = std::atoi(argv[i]);
                    break;
                case 1:
                    user_file_path = argv[i];
                    break;
                default:
                    print_usage(argv);
                    return 1;
                }

                ++token_counter;
            }
        }

        if (verbose)
        {
            Logger::setVerbosity(Logger::verbose);
            Logger::ln("Verbose mode is on");
        }

        UserDatabase *user_db = init_user_db(user_file_path);

        Logger::ln("Starting server...", Logger::normal);
        boost::asio::io_context io_context;
        // server has to live on the heap to not cause segfauls daah
        new Server(io_context, port, user_db);

        io_context.run();
    }
    catch (std::exception &e)
    {
        // TODO: log exception
        Logger::ln("Exception: " + string(e.what()), Logger::normal, red);
    }

    return 0;
}

void print_usage(char *argv[])
{
    // TODO: use cerr
    Logger::ln("Usage: " + string(argv[0]) + colorize(" [-v]", color::yellow) + colorize(" <port>", color::red) + colorize(" <path_to_user_file>", color::cyan), Logger::normal);
}

UserDatabase *init_user_db(string db_file_path)
{
    // does the user file exist?
    std::ifstream user_file(db_file_path);
    if (!user_file.is_open())
    {
        // print info message and create user file
        Logger::ln("User file " + colorize(db_file_path, color::cyan) + " does not exist. Creating it...", Logger::normal);

        std::ofstream user_file(db_file_path);
    }
    // init user database on the heap
    UserDatabase *user_db = new UserDatabase(db_file_path);

    return user_db;
}