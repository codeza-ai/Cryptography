#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "MD5.h"

using namespace std;

#define PORT 8080

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_sock, 1) == -1) {
        perror("Listen failed");
        return 1;
    }

    cout << "Server listening on port " << PORT << "...\n";

    int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock == -1) {
        perror("Accept failed");
        return 1;
    }

    char buffer[1024] = {0};
    recv(client_sock, buffer, sizeof(buffer), 0);
    string received_data(buffer);

    size_t delimiter = received_data.find("|");
    if (delimiter == string::npos) {
        send(client_sock, "Invalid format", 14, 0);
        close(client_sock);
        close(server_sock);
        return 1;
    }

    string received_message = received_data.substr(0, delimiter);
    cout << "Received message:  "<< received_message<<endl;
    string received_hash = received_data.substr(delimiter + 1);
    cout << "Received hash: "<< received_hash<<endl;

    MD5 md5;
    string computed_hash = md5.hash(received_message);
    cout << "Computed hash: "<<computed_hash<<endl;

    if (computed_hash == received_hash) {
        send(client_sock, "Valid", 5, 0);
        cout << "Hash matches! Message is valid.\n";
    } else {
        send(client_sock, "Invalid", 7, 0);
        cout << "Hash mismatch! Message is invalid.\n";
    }

    close(client_sock);
    close(server_sock);
    return 0;
}
