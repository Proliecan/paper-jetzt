#include <fstream>
#include <vector>
#include <thread>

#include "server.hpp"
#include "user.hpp"
#include "bot.hpp"

using std::string;
using std::vector;
using namespace bot;

// forward declarations
void print_usage(char *argv[]);
UserDatabase *init_user_db(char *argv[]);
Bot *init_bot(string name, string password, short port);
vector<Bot *> init_bots(vector<string> names, vector<string> passwords, short port);

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            print_usage(argv);
            return 1;
        }

        UserDatabase *user_db = init_user_db(argv);

        cout << "Starting server..." << endl;
        boost::asio::io_context io_context;
        // server has to live on the heap to not cause segfauls daah
        new Server(io_context, std::atoi(argv[1]), user_db);

        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

void print_usage(char *argv[])
{
    std::cerr << "Usage: " << argv[0]
              << colorize(" <port>", color::red)
              << colorize(" <path_to_user_file>", color::cyan)
              << std::endl;
}

UserDatabase *init_user_db(char *argv[])
{
    // does the user file exist?
    std::ifstream user_file(argv[2]);
    if (!user_file.is_open())
    {
        // print info message and create user file
        std::cout << "User file " << colorize(argv[2], color::cyan) << " does not exist. Creating it..." << std::endl;
        std::ofstream user_file(argv[2]);
    }
    // init user database on the heap
    UserDatabase *user_db = new UserDatabase(argv[2]);

    return user_db;
}