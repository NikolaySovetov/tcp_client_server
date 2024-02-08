#pragma once
#include <message_logger.hpp>
#include <cstdint>

namespace tcp
{
    class server
    {
    private:
        const char* connection_address{"0.0.0.0"}; 
        int endpoint{0};
        int port{0};
        uint16_t buffer_size{4096};
        uint16_t connection_count{0};

    private:
        message_logger mlogger;
        bool init_flag;
        bool stop_flag;

    private:
        inline uint16_t has_connections(); 
        void accept_connection();
        void concrete_connection(int client_socket);

    public:
        server(const char* file_name = "log.txt");
        ~server();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp