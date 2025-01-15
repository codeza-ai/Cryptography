#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
// #include "./sdes.cpp" // Include SDES implementation

using namespace std;

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Creating socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Server is listening on port " << PORT << "...\n";

    // Accepting client connections
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << "\n";

    // Create SDES object for decryption
    SDES sdes_obj;
    sdes_obj.key_generation(); // Generate keys for encryption/decryption

    // Communication with client
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_read == 0) {
            cout << "Client disconnected\n";
            break;
        } else if (bytes_read < 0) {
            perror("Read failed");
            break;
        }

        // Print encrypted message received
        buffer[bytes_read] = '\0';
        string encrypted_message(buffer);
        cout << "Encrypted message received: " << encrypted_message << "\n";

        // Convert encrypted message to vector<int>
        vector<int> ciphertext;
        for (char c : encrypted_message) {
            ciphertext.push_back(c - '0');
        }

        // Decrypt the message
        vector<int> decrypted_message = sdes_obj.decryption(ciphertext);

        // Convert decrypted message to string
        string plaintext_message;
        for (int bit : decrypted_message) {
            plaintext_message += (bit + '0');
        }

        // Print the decrypted message
        cout << "Decrypted message: " << plaintext_message << "\n";

        // Sending response to client
        string message = "Message received and decrypted";
        send(client_fd, message.c_str(), message.size(), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
