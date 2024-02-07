add_library(tcp_client client.cpp)
target_include_directories(tcp_client PUBLIC .)

add_subdirectory(timestamp)
target_link_libraries(tcp_client PRIVATE timestamp)
