#include "client.hpp"
#include "timestamp.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <future>
#include <chrono>
#include <thread>

namespace tcp
{
    client::~client()
    {
        close(sock);
    }

    void client::init(int argc, char **argv)
    {
        if (argc != 4)
        {
            std::cerr << "User input is invalid\n"
                      << "use format: ./client \"UserName\" \"port\" \"timeout\"\n";
            init_flag = false;
            return;
        }

        // Create a socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            std::cout << "Can't create a socket\n";
            init_flag = false;
            return;
        }

        // Ceate a hint structure for the server we'a connectiong with
        struct sockaddr_in hint;
        hint.sin_family = AF_INET;
        //hint.sin_port = htons(std::stoi(argv[2]));
        hint.sin_port = htons(3000);
        inet_pton(AF_INET, ip_address, &hint.sin_addr);

        // Connect to the server on the socket
        int connect_res = connect(sock, reinterpret_cast<sockaddr *>(&hint), sizeof(hint));
        if (connect_res == -1)
        {
            std::cout << "Can't connect to the server\n";
            init_flag = false;
            return;
        }

        name = argv[1];
        timeout = std::stoi(argv[3]);
        init_flag = true;
    }

    void client::run()
    {
        if (!init_flag)
        {
            return;
        }

        // While loop
        char buffer[buffer_size];
        int send_res{};
        std::string user_input(buffer_size, '\0');
        std::future<void> ft;

        while (true)
        {
            user_input = '[';
            user_input += get_timestamp();
            user_input += "] ";
            user_input += name;
            user_input += "  ";
            user_input += std::to_string(timeout);

            std::cout << user_input << '\n';

            ft = std::async(std::launch::async, [&]()
                            { send_res = send(sock, user_input.c_str(), sizeof(user_input) + 1, 0); });

            std::this_thread::sleep_for(std::chrono::milliseconds(timeout * 1000));

            std::cout << "send_res = " << send_res << '\n';

            if (send_res == -1 || send_res == 0)
            {
                break;
            }
        }
    }

} // tcp
