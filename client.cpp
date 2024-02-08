#include "timestamp.hpp"
#include "client.hpp"

#include "client.hpp"
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

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "User input is invalid\n"
                  << "use format: ./client \"UserName\" \"port\" \"timeout\"\n";
        return -1;
    }

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "Can't create a socket\n";
        return -1;
    }

    // Ceate a hint structure for the server we'a connectiong with
    std::string ip_address = "127.0.0.1";

    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(std::stoi(argv[2]));
    //hint.sin_port = htons(2003);
    inet_pton(AF_INET, ip_address.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connect_res = connect(sock, reinterpret_cast<sockaddr *>(&hint), sizeof(hint));
    if (connect_res == -1)
    {
        std::cout << "Can't connect to the server\n";
        return -1;
    }

    // While loop
    size_t buffer_size{4096};
    char buffer[buffer_size];
    int send_res{};
    std::string user_input(buffer_size, '\0');
    std::future<void> ft;
    size_t span = std::stoi(argv[3]);

    while (true)
    {
        user_input = '[';
        user_input += get_timestamp();
        user_input += "] ";
        user_input += argv[1];
        user_input += "  ";
        user_input += argv[3];

        // std::cout << user_input << '\n';

        ft = std::async(std::launch::async, [&]()
                        { send_res = send(sock, user_input.c_str(), sizeof(user_input) + 1, 0); });

        std::this_thread::sleep_for(std::chrono::milliseconds(span * 1000));

        if (send_res == -1 || send_res == 0)
        {
            break;
        }
    }
    close(sock);

    return 0;
}