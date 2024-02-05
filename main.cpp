#include "server.hpp"
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

int main(int argc, char *argv[])
{
    tcp::server s;
    s.init(argc, argv);
    s.run();

    return 0;
}