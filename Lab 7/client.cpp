#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "MD5.h"

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return 1;
    }
    string message;
    cout << "Enter the message to send: ";
    cin>> message;
    MD5 md5;
    string hash = md5.hash(message);
    cout << "Hash value of the message: "<<hash<<endl;
    string data_to_send = message + "|" + hash; // Format: "message|hash"

    send(sock, data_to_send.c_str(), data_to_send.size(), 0);
    cout << "Sent message and hash to server.\n";

    char response[1024] = {0};
    recv(sock, response, sizeof(response), 0);
    cout << "Server response: " << response << endl;

    close(sock);
    return 0;
}
