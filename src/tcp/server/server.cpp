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
#include <future>
#include <mutex>
#include <chrono>

namespace tcp
{
    std::mutex stop_mutex;
    std::mutex count_mutex;

    inline void server::add_connection()
    {
        count_mutex.lock();

        ++connection_count;

        count_mutex.unlock();
    }

    inline void server::delete_connection()
    {
        count_mutex.lock();

        --connection_count;

        count_mutex.unlock();
    }

    inline uint16_t server::get_connections()
    {
        std::lock_guard<std::mutex> lg(count_mutex);
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

                // add connection to common count
                add_connection();
            }
        }
    }

    void server::concrete_connection(int client_socket)
    {
        char buffer[buffer_size];
        int bytes_recv{};
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
                stop_mutex.lock();
                if (stop_flag)
                {
                    close_connection_flag = true;
                }
                stop_mutex.unlock();

                if (close_connection_flag)
                    break;
            }

            // Checks a bytes count of the "recv" and handle client message
            if (bytes_recv == -1 || bytes_recv == 0)
            {
                break;
            }
            else
            {
                // Async start "mlogger" and waits it
                ft = std::async([&]()
                                { mlogger.save_message(buffer); });

                // In this point CPU resources not used
                ft.wait();
            }
        }
        close(client_socket);
        delete_connection();
        std::cout << ". " << std::flush;
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
                stop_mutex.lock();
                stop_flag = true;
                stop_mutex.unlock();

                // Waits until the connections count becomes 0  
                while (get_connections())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
                break;
            }
        }

        std::cout << "\nThe server is stopped\n";
    }

} // tcp