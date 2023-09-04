#include "bot.hpp"

#include <iostream>

using std::cout;
using std::endl;

using namespace bot;

// main function
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: bot <port> <username> <password>\n";
        return 1;
    }

    boost::asio::io_context io_context;

    bot::Bot bot(io_context, std::atoi(argv[1]), argv[2], argv[3]);
    

    bot.connect();
    bot.join();
    bot.do_read();

    io_context.run();
}