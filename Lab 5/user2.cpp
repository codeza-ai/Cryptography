#include <cstring>
#include <iostream>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

#define P 23
#define G 9

int clientSocket, serverSocket;

void serverConnect() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    clientSocket = accept(serverSocket, nullptr, nullptr);
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

int createPublicVal(int privKey){
  return (int)(pow(G, privKey))%P;
}

int createKey(int user1Key, int privKey){
  return (int)(pow(user1Key, privKey))%P;
}

int main() { 
    serverConnect();
    
    sendData("User2 Online!(Sent by Bob)");
    cout << ">: " << receiveData() << endl;
    
    int U2PK;
    cout<<">: Enter private key: ";
    cin>>U2PK;
    
    int publicVal = createPublicVal(U2PK);
    
    sendData(to_string(publicVal));
    cout<<">: Public key sent!"<<endl;
    
    string user1Key = receiveData();
    int U1K=stoi(user1Key);
    cout<<">: Key received from User1: (Mallory!)"<<U1K<<endl;
    
    int commonkey = createKey(U1K, U2PK);
    cout<<">: Common Key: (with mallory)"<<commonkey<<endl;
    return 0;
}
