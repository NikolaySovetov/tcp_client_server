#pragma once
#include <fstream>
#include <mutex>

class message_logger
{
private:
    const char *file_name{"log.txt"};
    std::fstream file;
    std::mutex file_mutex;

public:
    message_logger();
    ~message_logger();
    void save_message(const char *client_message);
};
