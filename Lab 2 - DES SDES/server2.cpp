#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include "DES.h"

using namespace std;

int clientSocket, serverSocket;

void serverConnect() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    clientSocket = accept(serverSocket, nullptr, nullptr);
}

void sendData(string message) {
    char buffer[100] = {0};
    copy(message.begin(), message.end(), buffer);
    send(clientSocket, buffer, sizeof(buffer), 0);
}

string receiveData() {
    char buffer[100] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    string message = string(buffer);
    return message;
}

int main() {
    serverConnect();

    sendData("Server Online!");
    cout << ">: " << receiveData() << endl;

    string encryptedMsg = receiveData();
    cout << ">: Encrypted message received from Client : " << encryptedMsg << endl;

    string key = receiveData();
    cout << ">: 64-Bit Key received from Client : " << key << endl;

    DES des; // Create an object of DES class

    key = des.HexToBin(key);
    vector<string> sixteenKeys = des.generateKeys(key);

    encryptedMsg = des.HexToBin(encryptedMsg);
    string decipherText = des.DES_Decryption(encryptedMsg, sixteenKeys, key);
    decipherText = des.BinToHex(decipherText);
    cout << ">: Decrypted Message : " << decipherText << endl;

    cout << ">: Server Exiting!!" << endl << endl;
    close(serverSocket);

    return 0;
}
