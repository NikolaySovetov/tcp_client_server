add_library(tcp_server server.cpp)
target_include_directories(tcp_server PUBLIC .)
target_link_libraries(tcp_server PUBLIC message_logger)

