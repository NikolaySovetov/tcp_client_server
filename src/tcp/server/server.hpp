#pragma once
#include <request_handler.hpp>
#include <cstdint>
#include <deque>
#include <mutex>

namespace tcp
{
    class server
    {
    private:
        int endpoint{0};
        uint16_t port{0};
        int buffer_size{4096};

    private:
        message_logger_file mlogger;
        std::mutex sockets_mutex;
        std::deque<int> client_sockets;
        bool init_flag;

    private:
        void add_connection(int client_socket);
        void close_connection(int client_socket);
        void close_all_connections();
        void accept_connection();
        void concrete_connection(int client_socket);

    public:
        server() = default;
        ~server();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp