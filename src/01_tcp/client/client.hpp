#pragma once
#include <stddef.h>

namespace tcp
{
    class client
    {
    private:
        int sock{0};
        int port{0};
        size_t buffer_size{4096};
        size_t timeout{0};
        const char *ip_address{"127.0.0.1"};
        char *name{};
        bool init_flag;

    public:
        client() = default;
        ~client();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp