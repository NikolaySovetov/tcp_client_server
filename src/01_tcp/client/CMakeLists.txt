add_library(tcp_client client.cpp)
target_include_directories(tcp_client PUBLIC .)
target_link_libraries(tcp_client PRIVATE timestamp)
