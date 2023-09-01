#include "server.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: " << argv[0] << colorize(" <port>", color::red) << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        // server has to live on the heap to not cause segfauls daah
        new Server(io_context, std::atoi(argv[1]));

        io_context.run();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
