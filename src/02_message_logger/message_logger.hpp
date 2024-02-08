#pragma once
#include <fstream>
#include <mutex>

class message_logger
{
private:
    std::fstream file;
    std::mutex file_mutex;

public:
    message_logger(const char* file_name);
    ~message_logger();
    void save_message(const char *client_message);
};
