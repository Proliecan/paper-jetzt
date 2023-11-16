#include "bot.hpp"
#include "../helpers.hpp"

using namespace bot;
using namespace helpers;

// main function
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        // log (this should be in stderr TODO)
        Logger::ln("Usage: bot <port> <username> <password>\n", red);
        return 1;
    }

    boost::asio::io_context io_context;

    bot::Bot bot(io_context, std::atoi(argv[1]), argv[2], argv[3]);
    

    bot.connect();
    bot.join();
    bot.do_read();

    io_context.run();
}