#include "server.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>

namespace tcp
{
    namespace context
    {
        namespace mutexes
        {
            std::mutex count_connections;
            std::mutex stop_server;
        }
    }

    inline uint16_t server::has_connections()
    {
        std::lock_guard<std::mutex> lg(context::mutexes::stop_server);
        return connection_count;
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
                // Start new connection in new thread
                std::thread th(&server::concrete_connection, this, client_socket);
                th.detach();

                context::mutexes::count_connections.lock();
                ++connection_count;
                context::mutexes::count_connections.unlock();
            }
        }
    }

    void server::concrete_connection(int client_socket)
    {
        char buffer[buffer_size];
        int bytes_recv{0};
        bool close_connection_flag{false};
        std::future<void> ft;
        std::chrono::milliseconds span{250};

        while (true)
        {
            // Clear buffer
            memset(buffer, 0, buffer_size);

            // Async wait for a message
            ft = std::async([&]()
                            { bytes_recv = recv(client_socket, buffer, buffer_size, 0); });

            // Wait "recv"and checks whether "stop_flag" is active by "span" timeout
            while (ft.wait_for(span) == std::future_status::timeout)
            {
                context::mutexes::stop_server.lock();
                if (stop_flag)
                {
                    close_connection_flag = true;
                }
                context::mutexes::stop_server.unlock();
            }

            // Checks a bytes count of the "recv" and handle client message
            if (bytes_recv == -1 || bytes_recv == 0 || close_connection_flag)
            {
                break;
            }
            else
            {
                ft = std::async([&]()
                                { mlogger.save_message(buffer); });
                ft.wait();
            }
        }
        close(client_socket);

        if (close_connection_flag)
        {
            std::cout << ". " << std::flush;
        }

        context::mutexes::count_connections.lock();
        --connection_count;
        context::mutexes::count_connections.unlock();
    }

    server::server(const char *file_name) : mlogger{file_name}
    {
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
        if (argc != 2)
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
        port = std::stoi(argv[1]);

        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, connection_address, &hint.sin_addr);

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

        std::thread th1(&server::accept_connection, this);
        th1.detach();

        while (true)
        {
            std::string command;
            std::cout << "Enter \"exit\" to stop server: ";
            std::cin >> command;

            if (command == "exit")
            {
                context::mutexes::stop_server.lock();
                stop_flag = true;
                context::mutexes::stop_server.unlock();

                // Waits until the connections count becomes 0
                while (has_connections())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
                break;
            }
        }
        std::cout << "\nThe server is stopped\n";
    }

} // tcp