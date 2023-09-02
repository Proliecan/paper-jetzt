#include <fstream>

#include "server.hpp"
#include "user.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: " << argv[0]
                      << colorize(" <port>", color::red)
                      << colorize(" <path_to_user_file>", color::cyan)
                      << std::endl;
            return 1;
        }

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
