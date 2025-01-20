#include <cstring>
#include <iostream>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "DES.h"
using namespace std;

int clientSocket;


void clientConnect()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
}

void sendData(string message) {
    char buffer[100] = { 0 };
    copy(message.begin(), message.end(), buffer);
    send(clientSocket, buffer, sizeof(buffer), 0);
}

string receiveData(){
    char buffer[100] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    string message = string(buffer);
    return message;
}

int main() {
    clientConnect();
    DES obj;

    cout << ">: " << receiveData() << endl;
    sendData("Client Online!");

    string plainText;
    cout << ">: Enter the 8 letter message to encrypt: ";
    getline(cin, plainText);
    plainText=obj.atobConv(plainText);
    cout<<"Plaintext: "<<obj.BinToHex(plainText)<<endl;

    string key;
    cout << ">: Enter the 8 letter key: ";
    cin >> key;
    key=atobConv(key);
    cout<<"Key: "<<obj.BinToHex(key)<<endl;
    
    vector<string> sixteenKeys = obj.generateKeys(key);

    string cipherText = obj.DES_Encryption(plainText, initialPermutation, sixteenKeys, expnasionPermutation, permutationFunction, substitutionBoxes, IPInverse);
    
    cipherText = obj.BinToHex(cipherText);
    sendData(cipherText);
    cout << ">: Encrypted message sent to Server!!" << endl;

    key = obj.BinToHex(key);
    sendData(key);
    cout << ">: 64-Bit Key sent to Server!" << endl;

    cout << ">: Client Exiting!!!" << endl << endl;
    close(clientSocket);

    return 0;
}
