#pragma once
#include <cstdint>
#include <vector>
#include <deque>
#include <mutex>

namespace tcp
{
    /*     enum class status_list : int
        {
            initialized = 1,
            runnning = 2,
            stopping = 3,
            stopped = 4
        }stat_list;
     */
    class connection_context
    {
    private:
        std::mutex mt;
        int status{0};

    public:
        void set_status(int new_status);
        void get_status(int& current_status);
    };

    class server
    {
    private:
        int endpoint{0};
        uint16_t port{0};
        int buffer_size{4096};

    private:
        enum status_list : int
        {
            initialized = 1,
            runnning = 2,
            stopping = 3,
            stopped = 4
        };

        int server_status{0};
        std::deque<connection_context> connections;
        std::deque<std::string> message_list;

    private:
        void accept_connection();
        void concrete_connection(int client_socket);
        void save_message(const std::string &client_message);

    public:
        server() = default;
        ~server();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp