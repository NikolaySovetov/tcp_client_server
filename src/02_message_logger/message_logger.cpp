#include "message_logger.hpp"

message_logger::message_logger(const char* file_name)
{
    file.open(file_name, std::ios::out /* | std::ios::app */);
}

message_logger::~message_logger()
{
    file.close();
}

void message_logger::save_message(const char *client_message)
{
    file_mutex.lock();

    file << client_message;
    file << '\n';

    file_mutex.unlock();
}
