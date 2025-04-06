#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char *greeting = "Hello from server";

    // Create socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        return 1;
    }

    // Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // localhost
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // Start listening
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    // Accept incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        return 1;
    }

    read(new_socket, buffer, 1024);
    std::cout << "Message from client: " << buffer << std::endl;

    send(new_socket, greeting, strlen(greeting), 0);
    std::cout << "Greeting sent to client\n";

    close(new_socket);
    close(server_fd);
    return 0;
}
