#pragma once
#include <message_logger.hpp>
#include <cstdint>
#include <mutex>

namespace tcp
{
    class server
    {
    private:
        int endpoint{0};
        uint16_t port{0};
        uint16_t buffer_size{4096};
        uint16_t connection_count{0};

    private:
        message_logger mlogger;
        std::mutex stop_mutex;
        std::mutex count_mutex;
        bool init_flag;
        bool stop_flag;

    private:
        inline void increase_count_connection();
        inline void decrease_count_connection();
        inline uint16_t has_connections(); 
        void accept_connection();
        void concrete_connection(int client_socket);

    public:
        server() = default;
        ~server();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp