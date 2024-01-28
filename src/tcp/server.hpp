#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace tcp
{
    class server
    {
    private:
        int endpoint{0};
        uint16_t port{0};
        int buffer_size{4096};

    private:    
        std::vector<std::string> log;
        int connection_count{0};
        int server_status{0};
        enum status_list : int
        {
            initialized = 1,
            runnning = 2,
            stopping = 3
        };

    private:
        void accept_connection();
        void concrete_connection(int client_socket);
        void log_message(const std::string& client_message);

    public:
        server() = default;
        ~server();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp