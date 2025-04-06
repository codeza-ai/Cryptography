#include <cstring>
#include <iostream>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define P 23
#define G 9

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

int createPublicVal(int privKey){
  return (int)(pow(G, privKey))%P;
}

int createKey(int user2Key, int privKey){
  return (int)(pow(user2Key, privKey))%P;
}

int main() { 
    clientConnect();

    cout << ">: " << receiveData() << endl;
    sendData("User1 Online! (Sent by Alice)");
    
    int U1PK;
    cout<<">: Enter private key: ";
    cin>>U1PK;
    
    int publicVal = createPublicVal(U1PK);
    
    sendData(to_string(publicVal));
    cout<<">: Public key sent!"<<endl;
    
    string user2Key = receiveData();
    int U2K=stoi(user2Key);
    cout<<">: Key received from User2: (Mallory!)"<<U2K<<endl;
    
    int commonkey = createKey(U2K, U1PK);
    cout<<">: Common Key: (with Mallory)"<<commonkey<<endl;
    return 0;
}
