#include "request_handler.hpp"

namespace tcp
{
    message_logger::message_logger()
    {
        //file.open(file_name, std::ios::out /* | std::ios::app */);
    }

    message_logger::~message_logger()
    {
        file.open(file_name, std::ios::out | std::ios::app);

        for (const auto str : messages_pool)
        {
            file << str << '\n';
        }

        file.close();
    }

    std::mutex file_mutex;

    void message_logger::save_message(const char *client_message)
    {
        file_mutex.lock();

        messages_pool.push_back(client_message);

        file_mutex.unlock();
    }

    /*     void message_logger::save_message(const char *client_message)
        {
            file_mutex.lock();
            file.open("log.txt", std::ios::out | std::ios::app);
            file << client_message << '\n';
            file.close();
            file_mutex.unlock();
        }
     */

    void message_logger::save_message(const std::initializer_list<std::string> &client_message)
    {
        file_mutex.lock();
        // file.open("log.txt", std::ios::out /* | std::ios::app */);
        for (const std::string &word : client_message)
        {
            file << word << std::flush;
        }
        // file.close();
        file_mutex.unlock();
    }

} // tcp