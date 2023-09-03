#include "bot.hpp"
#include "../helpers.hpp"

using namespace bot;
using namespace helpers;

void Bot::run()
{
    connect();
    send("join|" + name + "|" + password + "\n");
    send("ready\n");
}

void Bot::connect()
{
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("localhost", std::to_string(port));

    boost::asio::connect(socket, endpoints);
}

void Bot::send(string message)
{
    std::size_t length = message.length();
    boost::asio::async_write(socket, boost::asio::buffer(message.c_str(), length),
                             [&](boost::system::error_code ec, std::size_t /*length*/)
                             {
        if (!ec)
        {
            // log
            std::cout << "Bot " << name << " sent message: " << message << std::endl;
        }
        else
        {
            std::cerr << "Error: " << ec.message() << "\n";
        } });
}

void Bot::receive()
{
    // asynchronous read
    char *data = new char[1024];
    socket.async_read_some(boost::asio::buffer(data, 1024), [&](boost::system::error_code ec, std::size_t length)
                           {
        if (!ec)
        {
            // process message
            string message(data, length);
            process(message);

            // receive next message
            receive();
        }
        else
        {
            std::cerr << "Error: " << ec.message() << "\n";
        } });
}

void Bot::process(string message)
{
    // log
    std::cout << "Bot " << name << " received message: " << message << std::endl;
    return;
}

// forward declarations
void print_usage(char *argv[]);

// main function
int main(int argc, char *argv[])
{
    try
    {
        if (argc != 4)
        {
            print_usage(argv);
            return 1;
        }

        string name = argv[1];
        string password = argv[2];
        short port = std::atoi(argv[3]);

        // log
        std::cout << "Starting bot " << name << "..." << std::endl;

        boost::asio::io_context io_context;

        Bot bot(io_context, name, password, port);
        bot.run();
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
              << colorize(" <name>", color::red)
              << colorize(" <password>", color::cyan)
              << colorize(" <port>", color::green)
              << std::endl;
}