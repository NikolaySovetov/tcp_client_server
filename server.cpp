#include "server.hpp"

int main(int argc, char *argv[])
{
    tcp::server s;
    s.init(argc, argv);
    s.run();
    
    return 0;
}