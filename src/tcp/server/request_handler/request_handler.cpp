#include "request_handler.hpp"

namespace tcp
{
    message_logger::message_logger() {
        file.open(file_name, std::ios::out); 
    }

    message_logger::~message_logger() {
        file.close(); 
    }

    void message_logger::save_message(const char *client_message)
    {
        file_mutex.lock();
        
        file << client_message;
        
        file_mutex.unlock();
    }
    
} // tcp