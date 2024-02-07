#include "server.hpp"
#include "request_handler.hpp"
#include <iostream>


int main(int argc, char *argv[])
{
    tcp::server s;
    s.init(argc, argv);
    s.run();

    
/*     tcp::message_logger logger;

    for (int i {110}; i < 115; ++i) {
        logger.save_message({"text (", std::to_string(i), ")\n"});
    }
 */
    return 0;
}