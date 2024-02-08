#include <client.hpp>

int main(int argc, char *argv[])
{
    tcp::client cl;
    cl.init(argc, argv);
    cl.run();

    return 0;
}