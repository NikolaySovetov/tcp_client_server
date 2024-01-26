#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>

namespace tcp{

class server {
private:
    int endpoint {0};
    uint16_t port {0};

    struct status{
        bool initialized {0};
        bool running {0};
        int connection_count {0}; 
    }server_status;

private:
    void accept_connection() {

        // Accept a call 
        while(true) {
            
            struct sockaddr_in client;
            socklen_t clientSize;
            int clientSocket = accept(endpoint, reinterpret_cast<sockaddr*>(&client), &clientSize);
            
            if (clientSocket > 0) {
                concrete_connection(clientSocket);
            }
        }

    }

    void concrete_connection(int clientSocket) {
        // While receiving - display message
        char buffer[4096];
        while(true) {
            // Clear buffer
            memset(buffer, 0, 4096);
            // Wait for a message
            int bytesRecv = recv(clientSocket, buffer, 4096, 0);
            if (bytesRecv == -1) {
                std::cerr << "The client disconnected\n";
                break;
            }    
            // Display the message
            std::cout << "Received: " << buffer << std::endl;
            // echo to client
            send(clientSocket, buffer, bytesRecv + 1, 0);    
        }
    }
public:
    server() = default;
    ~server() {
        if (endpoint > 0) {
            close(endpoint);
        }
    }
    void init(int argc, char* argv[]) {

        // Validation user input
        if (argc != 2) {
            std::cerr << "User input is invalid\n"
                      << "use format: ./server 3000 (3000 - number of port)\n";
            return;
        }    

        // Create an endpoint(socket) for communication
        endpoint = socket(AF_INET, SOCK_STREAM, 0);
        if (endpoint == -1) {
            std::cerr << "Can't create a socket\n";
            return;
        }

        // To describe the socket for work with IP
        port = std::stoi(argv[1]);
        struct sockaddr_in hint{
            .sin_family = AF_INET,
            .sin_port = htons(port)
        };
        inet_pton(AF_INET, INADDR_ANY, &hint.sin_addr);

        // Bind the socket to IP address
        if (bind(endpoint, reinterpret_cast<sockaddr*>(&hint), sizeof(hint)) == -1) {
            std::cerr << "Can't bind to IP/port\n";
            return;
        }

        server_status.initialized = true;
    }    
    void run() {
        
        if (!server_status.initialized) {
            return;
        }

        std::cout << "Server runing. Current port: " << port << '\n';

        // Mark the socket for listening in
        if (listen(endpoint, SOMAXCONN) == -1) {
            std::cerr << "Can't listen\n";
            return;
        }

        std::thread th1 (accept_connection);
        th1.detach();
            
        while (true) {
            std::string command;
            std::cout << "Enter \"exit\" to stop server: ";
            std::cin >> command;
            if (command == "exit") {
                break;
            } 
        }

        std::cout << "\nServer was stopped\n";
    }
};    

}   // tcp