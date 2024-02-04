#include "server.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <list>
#include <thread>

namespace tcp
{
    void server::add_connection(int client_socket)
    {
        sockets_mutex.lock();

        client_sockets.push_back(client_socket);

        sockets_mutex.unlock();
    }

    void server::close_connection(int client_socket)
    {
        sockets_mutex.lock();

        if (client_socket)
        {
            close(client_socket);
        }

        sockets_mutex.unlock();
    }

    void server::close_all_connections()
    {
        sockets_mutex.lock();

        for (const auto sock : client_sockets)
        {
            close(sock);
/*             if (sock)
            {
                ;
            }
 */
        }
        sockets_mutex.unlock();
    }

    void server::accept_connection()
    {
        // Accept a call
        while (true)
        {
            struct sockaddr_in client;
            socklen_t clientSize;
            int client_socket = accept(endpoint, reinterpret_cast<sockaddr *>(&client), &clientSize);

            if (client_socket > 0)
            {
                add_connection(client_socket);

                std::thread th(&server::concrete_connection, this, client_socket);
                th.detach();
            }
        }
    }

    void server::concrete_connection(int client_socket)
    {
        // While receiving - display message
        char buffer[buffer_size];

        while (true)
        {
            // Clear buffer
            memset(buffer, 0, buffer_size);

            // Wait for a message
            int bytes_recv = recv(client_socket, buffer, buffer_size, 0);

            if (bytes_recv == -1 || bytes_recv == 0)
            {
                break;
            }
            else
            {
                mlogger.save_request(buffer);
            }
        }
        close_connection(client_socket);
    }

    server::~server()
    {
        if (endpoint > 0)
        {
            close(endpoint);
        }
    }

    void server::init(int argc, char *argv[])
    {

        // Validation user input
        if (argc != 1)
        {
            std::cerr << "User input is invalid\n"
                      << "use format: ./server 3000 (3000 - number of port)\n";
            return;
        }

        // Create an endpoint(socket) for communication
        endpoint = socket(AF_INET, SOCK_STREAM, 0);
        if (endpoint == -1)
        {
            std::cerr << "Can't create a socket\n";
            return;
        }

        // To describe the socket for work with IP
        // port = std::stoi(argv[1]);
        port = std::stoi("2000");

        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        // Bind the socket to IP address
        if (bind(endpoint, reinterpret_cast<sockaddr *>(&hint), sizeof(hint)) == -1)
        {
            std::cerr << "Can't bind to IP/port\n";
            return;
        }

        init_flag = true;
    }

    void server::run()
    {
        if (!init_flag)
        {
            return;
        }

        std::cout << "Server runing. Current port: " << port << '\n';

        // Mark the socket for listening in
        if (listen(endpoint, SOMAXCONN) == -1)
        {
            std::cerr << "Can't listen\n";
            return;
        }

        bool multithread_handler;
        {
        }

        std::thread th1(&server::accept_connection, this);
        th1.detach();

        while (true)
        {
            std::string command;
            std::cout << "Enter \"exit\" to stop server: ";
            std::cin >> command;

            if (command == "exit")
            {
                break;
            }
        }

        std::cout << "\nServer was stopped\n";
    }

} // tcp