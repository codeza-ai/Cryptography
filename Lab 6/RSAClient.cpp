#include <cstring>
#include <iostream>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define PUB_P 7919
#define PUB_Q 1009

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

int gcd(int a, int b)
{
    if(b==0) return a;
    return gcd(b, a%b);
}

int find_coprime(int phi)
{
    for(int i=2; i<phi; i++){
        int gcd_phi = gcd(i, phi);
        if (gcd_phi == 1) return i;
    }
    return 1;
}

int generate_key(int& n)
{
    n = PUB_P * PUB_Q;
    int phi = (PUB_P-1)*(PUB_Q-1);
    return find_coprime(phi);
}

int encrypt (int pt, int key, int n)
{
    int res = 1;
    pt = pt % n;
    while (key>0){
      if(key & 1) res = (res * 1LL * pt) % n;
      pt = (pt * 1LL * pt) % n;
      key = key/2;
    }
    return res;
}
    
int main() {
    clientConnect();
    
    cout << ">: " << receiveData() << endl;
    sendData("Client Online!");
    
    int plaintext;
    cout << ">: Enter Plaintext: " << endl;
    cin >> plaintext;
    
    int n;
    int e = generate_key(n);
    cout << ">: Public key(e, n): (" << e << ", " << n << ")" << endl;
    
    int ciphertext = encrypt(plaintext, e, n);
    ciphertext = (ciphertext % n + n) % n;
    
    sendData(to_string(ciphertext));
    cout << ">: Encrypted Text Sent!" << endl;
    cout << ciphertext << endl;
    
    return 0;
}
