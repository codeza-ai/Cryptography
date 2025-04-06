#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

#define PUB_P 7919
#define PUB_Q 1009

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

int extended_euclidean(int num, int phi, int& x, int& y) {
    if (phi == 0) {
        x = 1, y = 0;
        return num;
    }
    int x1, y1;
    int d = extended_euclidean(phi, num % phi, x1, y1);
    y = x1 - (num / phi) * y1;
    x = y1;
    return d;
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
    int e = find_coprime(phi);
    int inv, y;
    int d = extended_euclidean(e, phi, inv, y);
    if(inv<0) inv = (inv%phi + phi) % phi;
    return inv;
}

int decrypt (int ct, int key, int n)
{
    int res = 1;
    ct = ct % n;
    while (key>0){
      if(key & 1) res = (res * 1LL * ct) % n;
      ct = (ct * 1LL * ct) % n;
      key = key/2;
    }
    return res;
}

int main() {
    serverConnect();

    sendData("Server Online!");
    cout << ">: " << receiveData() << endl;
    
    int n;
    int d = generate_key(n);
    cout << ">: Private key(d, n): (" << d << ", " << n << ")" << endl;
    
    string input_ciphertext = receiveData();
    int ciphertext = stoi(input_ciphertext);
    cout << ">: received text: " << ciphertext<<endl;
    
    int plaintext = decrypt(ciphertext, d, n);
    cout << ">: Decrypted Text: " << plaintext << endl;
    
    cout << ">: Server Exiting!!" << endl << endl;
    close(serverSocket);
    
    return 0;
}
