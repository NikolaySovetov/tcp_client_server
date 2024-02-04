#include "request_handler.hpp"

namespace tcp
{
    void message_logger::save_request(const char *client_message)
    {
        mt.lock();
        
        log.push_back(client_message);
        
        mt.unlock();
    }

    message_logger_file::message_logger_file() {
        file.open(file_name, std::ios::out); 
    }

    message_logger_file::~message_logger_file() {
        file.close(); 
    }

    void message_logger_file::save_request(const char *client_message)
    {
        file_mutex.lock();
        
        file << client_message;
        
        file_mutex.unlock();
    }



} // tcp