#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <mutex>


namespace tcp
{
    class message_logger
    {
    private:
        std::mutex mt;
        std::vector<std::string> log;

    public:
        void save_request(const char* client_message);
    };

    class message_logger_file
    {
    private:
        const char* file_name {"./log.txt"};
        std::fstream file;
        std::mutex file_mutex;

    public:
        message_logger_file();
        ~message_logger_file();
        void save_request(const char* client_message);
    };

} // tcp