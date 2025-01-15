#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
// #include "./sdes.cpp" // Import SDES implementation

using namespace std;

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Converting IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connecting to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    cout << "Connected to server at " << SERVER_IP << " on port " << PORT << "\n";

    // Communication with server
    while (true) {
        cout << "Enter an 8-bit binary string to send (or type 'exit' to quit): ";
        cin.getline(buffer, BUFFER_SIZE);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Convert input to vector<int>
        vector<int> plaintext(8);
        for (int i = 0; i < 8; i++) {
            plaintext[i] = buffer[i] - '0';
        }

        // Encrypt the plaintext
        vector<int> ciphertext = sdes_obj.encryption(plaintext);

        // Convert ciphertext vector to string
        string encrypted_message;
        for (int bit : ciphertext) {
            encrypted_message += (bit + '0');
        }

        // Send encrypted message to server
        send(sockfd, encrypted_message.c_str(), encrypted_message.size(), 0);

        // Receiving response from server
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            cout << "Server closed connection or error occurred\n";
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Server: " << buffer << "\n";
    }

    close(sockfd);
    return 0;
}