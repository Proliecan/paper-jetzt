#include "bot.hpp"
#include "../helpers.hpp"

using namespace bot;
using namespace helpers;

// main function
int main(int argc, char *argv[])
{
    if (argc < 4 || argc > 5)
    {
        // log (this should be in stderr TODO)
        Logger::ln("Usage: bot [-v] <port> <username> <password>\n", Logger::normal, red);
        return 1;
    }

    boost::asio::io_context io_context;

    // create bot
    bot::Bot *bot;
    if (std::string(argv[1]) == "-v")
    {
        bot = new bot::Bot(io_context, std::atoi(argv[2]), argv[3], argv[4]);
    } else {
        bot = new bot::Bot(io_context, std::atoi(argv[1]), argv[2], argv[3]);
    }

    // verbose mode
    if (std::string(argv[1]) == "-v")
    {
        Logger::setVerbosity(Logger::verbose);
    }

    bot->connect();
    bot->join();
    bot->do_read();

    io_context.run();
}