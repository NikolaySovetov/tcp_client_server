#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

int main(int argc, char* argv[]) {

    // Create an endpoint for communication (socket)
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        std::cerr << "Can't create a socket\n";
        return EXIT_FAILURE;
    }

    // Bind the socket to a IP / port
    std::string str_port = "3000"; //argv[1];
    uint16_t port = std::stoi(str_port);

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening, reinterpret_cast<sockaddr*>(&hint), sizeof(hint)) == -1) {
        std::cerr << "Can't bind to IP/port\n";
        return EXIT_FAILURE;
    }

    // Mark the socket for listening in
    if (listen(listening, SOMAXCONN) == -1) {
        std::cerr << "Can't listen\n";
        return EXIT_FAILURE;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, reinterpret_cast<sockaddr*>(&client), &clientSize);
    if (clientSocket == -1) {
        std::cerr << "Problem with client connection\n";
        return EXIT_FAILURE;
    }

    // Close the listening socket
    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo(reinterpret_cast<sockaddr*>(&client), sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result) {
        std::cout << host << " connected on " << svc << std::endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
    }

    // While receiving - display message
    char buffer[4096];
    
    while(true) {
        // Clear buffer
        memset(buffer, 0, 4096);

        // Wait for a message
        int bytesRecv = recv(clientSocket, buffer, 4096, 0);
        
        if (bytesRecv == -1) {
            std::cerr << "The client disconnected\n";
            break;
        }    

        // Display the message
        std::cout << "Received: " << buffer << std::endl;
        
        // echo to client
        send(clientSocket, buffer, bytesRecv + 1, 0);    
    }

    // Close socket
    close(clientSocket);


    return EXIT_SUCCESS;
}