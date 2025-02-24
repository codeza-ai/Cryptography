#include <cstring>
#include <iostream>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <cstdio>
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

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {  // If exponent is odd
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

long long get_random_k(long long p) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dist(1, p - 2);

    return dist(gen);
}

int main() {
    clientConnect();
    
    cout << ">: " << receiveData() << endl;
    sendData("Client Online!");
    
    string public_key = receiveData();
    long long p, g, y;
    sscanf(public_key.c_str(), "(%lld, %lld, %lld)", &p, &g, &y);
    cout << p << " " << g << " " << y << endl;
    
    long long PlainText;
    cout << ">: Please enter the message to be encrypted and sent: " << endl;
    cin >> PlainText;
    
    long long k = get_random_k(p);
    cout << ">: random k: " << k << endl;
    
    long long c1 = power_mod(g, k, p);
    long long c2 = power_mod(PlainText, 1, p) * power_mod(y, k, p);
    if(c2>=p) c2=power_mod(c2, 1, p);
    cout << c1 << " " << c2 << endl;
    
    string CipherText = "("+to_string(c1)+", "+to_string(c2)+")";
    sendData(CipherText);
    cout << ">: Ciphertext sent!" << endl;
    return 0;
}
