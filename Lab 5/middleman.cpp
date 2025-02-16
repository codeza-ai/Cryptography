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

int user1Socket, user2Socket, ownSocket;

void serverConnect() {
    ownSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    int opt=1;
    setsockopt(ownSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
   
    bind(ownSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(ownSocket, 5);
    
    cout<<"Mallory waiting for Alice..."<<endl;
    user1Socket = accept(ownSocket, nullptr, nullptr);
    cout<<"Alice connected!"<<endl;
}

void clientConnect()
{
    user2Socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(connect(user2Socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))){
      cout<<"Waiting to connect to user2..."<<endl;
      sleep(1);
    }
    cout<<"Connected to user2!"<<endl;
}

void sendData(string message, int Sock) {
    char buffer[100] = { 0 };
    copy(message.begin(), message.end(), buffer);
    send(Sock, buffer, sizeof(buffer), 0);
}

string receiveData(int Sock){
    char buffer[100] = { 0 };
    recv(Sock, buffer, sizeof(buffer), 0);
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
    clientConnect(); //connect to Bob
    serverConnect(); //connect to Alice
    
    sendData("User2 Online! (sent by Mallory to Alice)", user1Socket);
    cout << ">: " << receiveData(user1Socket) << endl;
    
    cout<< ">: "<<receiveData(user2Socket) <<endl;
    sendData("User1 Online! (sent by Mallory to Bob)", user2Socket);
    
    int MIMPK;
    cout<<">: Enter private key: ";
    cin>>MIMPK;
    
    int U1K = stoi(receiveData(user1Socket));
    cout<<">: Key received from User1(Alice): "<<U1K<<endl;
    
    int publicVal = createPublicVal(MIMPK);
    sendData(to_string(publicVal), user1Socket);
    cout<<">: Public key sent!(to Alice)"<<endl;
    sendData(to_string(publicVal), user2Socket); //sending fake key to bob
    cout<<">: public key sent!(to Bob)"<<endl;
    
    string user2Key = receiveData(user2Socket);
    int U2K=stoi(user2Key);
    cout<<">: Key received from User2(Bob): "<<U2K<<endl;
    
    int commonkeyU1 = createKey(U1K, MIMPK);
    cout<<">: Common Key: (with Alice)"<<commonkeyU1<<endl;
    
    int commonkeyU2 = createKey(U2K, MIMPK);
    cout<<">: Common Key: (with Bob)"<<commonkeyU2<<endl;
    return 0;
}
