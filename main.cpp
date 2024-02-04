#include "server.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

    tcp::server s;
    s.init(argc, argv);
    s.run();

 
/*     tcp::message_logger_file mlf;
    mlf.save_request("Hello\n");
 */

    return 0;
}