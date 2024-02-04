add_library(tcp_server server.cpp)
target_include_directories(tcp_server PUBLIC .)

add_subdirectory(request_handler)
target_link_libraries(tcp_server PUBLIC request_handler)

