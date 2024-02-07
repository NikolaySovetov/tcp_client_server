#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <initializer_list>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>

namespace tcp
{
    class message_logger
    {
    private:
        const char* file_name {"log.txt"};
        std::vector<std::string> messages_pool;
        std::fstream file;

    public:
        message_logger();
        ~message_logger();
        void save_message(const char* client_message); 
        void save_message(const std::initializer_list<std::string>& client_message); 
    };

} // tcp