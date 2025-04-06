#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "sha512.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert address
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    cout << "Enter the message to send: ";
    string message;
    cin>>message;
    send(sock, message.c_str(), message.length(), 0);
    cout << "Sent: " << message << endl;

    // Receive hash
    int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
        string receivedHash(buffer, valread);
        cout << "Received SHA-512: " << receivedHash << endl;

        // Compute hash locally
        SHA512 sha512;
        string localHash = sha512.compute(message);
        cout << "Local SHA-512: " << localHash << endl;

        // Compare hashes
        if (receivedHash == localHash) {
            cout << "Integrity check PASSED!" << endl;
        } else {
            cout << "Integrity check FAILED!" << endl;
        }
    }

    // Cleanup
    close(sock);

    return 0;
}
// https://www.perplexity.ai/search/i-am-working-on-a-project-in-w-ylxTuSnaQo.6MuNwVZd8qw