#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "sha512.h"

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for client
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << PORT << endl;

    // Accept connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive data
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        string message(buffer, valread);
        cout << "Received: " << message << endl;

        // Compute SHA-512
        SHA512 sha512;
        string hash = sha512.compute(message);
        cout << "Computed SHA-512: " << hash << endl;

        // Send back hash
        send(new_socket, hash.c_str(), hash.length(), 0);
    }

    // Cleanup
    close(new_socket);
    close(server_fd);

    return 0;
}
