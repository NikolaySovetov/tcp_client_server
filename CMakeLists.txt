cmake_minimum_required(VERSION 3.16)
project(tcp_connection VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/tcp/server)
add_executable(main main.cpp)
target_link_libraries(main PRIVATE tcp_server)

target_link_libraries(main PRIVATE request_handler)
 
