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

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cout << "Can't create a socket\n";
        return -1;
    }

    // Ceate a hint structure for the server we'a connectiong with
    int port = 2003;
    std::string ipAddress = "127.0.0.1";

    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket
    int connectRes = connect(sock, reinterpret_cast<sockaddr *>(&hint), sizeof(hint));
    if (connectRes == -1)
    {
        std::cout << "Can't connect to the server\n";
        return -1;
    }

    // While loop
    char buffer[4096];
    int sendRes{};
    std::string userInput;
    std::future<void> ft;
    size_t count{0};

    while (true)
    {
        /*         userInput = get_timestamp();
                ft = std::async(std::launch::deferred, [&]()
                                {
                                    sendRes = send(sock, userInput.c_str(), sizeof(userInput) + 1, 0);
                                    std::cout << userInput << '\n'; });

        */
        userInput = get_timestamp();
        
        ++count;
        userInput += " : num ";
        userInput += std::to_string(count);
        
        sendRes = send(sock, userInput.c_str(), sizeof(userInput) + 1, 0);
        std::cout << userInput << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        if (sendRes == -1 || sendRes == 0)
        {
            break;
        }
    }
    close(sock);

    return 0;
}