cmake_minimum_required(VERSION 3.16)
project(tcp_connection VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/tcp/server)
add_executable(server_exec server_exec.cpp)
target_link_libraries(server_exec PRIVATE tcp_server)
target_link_libraries(server_exec PRIVATE request_handler)

add_subdirectory(src/tcp/client)
add_executable(client_exec client_exec.cpp)
target_link_libraries(client_exec PRIVATE tcp_client)
target_link_libraries(client_exec PRIVATE timestamp)


