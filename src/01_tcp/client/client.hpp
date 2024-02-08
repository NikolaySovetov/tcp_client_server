#pragma once

namespace tcp
{
    class client
    {
    private:
        int sock{0};
        int port{0};
        int buffer_size{4096};
        int timeout{0};
        const char *ip_address{"127.0.0.1"};
        char *name{};

    private:
        bool init_flag;
        bool stop_flag;

    public:
        client() = default;
        ~client();
        void init(int argc, char *argv[]);
        void run();
    };

} // tcp