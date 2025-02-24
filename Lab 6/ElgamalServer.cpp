#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <vector>
#include <set>
#include <cmath>
#include <random>
#include <string>
#include <cstdio>

using namespace std;

#define PRIME_P 23

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

set<long long> prime_factors(long long n) {
    set<long long> factors;
    while(n%2==0){
      factors.insert(2);
      n/=2;
    }
    for(long long i=3; i<=sqrt(n); i+=2){
        while(n%i==0) {
          factors.insert(i);
          n/=i;
        }
    }
    if(n>1) factors.insert(n);
    return factors;
}
    
long long find_generator() {
    long long phi = PRIME_P-1;
    set<long long> factors = prime_factors(phi);
    for(long long g = 2; g<PRIME_P; g++){
        bool is_generator = true;
        for(int q:factors){
            if(power_mod(g, phi/q, PRIME_P) == 1){
                is_generator = false;
                break;
            }
        }
        if (is_generator) return g;
    }
    return -1;
}

long long get_random_d() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dist(1, PRIME_P - 2);

    return dist(gen);
}

long long generate_pub_key(long long& g, long long& d) {
    d = get_random_d();
    cout << ">: random d: " << d << endl;
    g = find_generator();
    long long y = power_mod(g,d,PRIME_P);
    return y;
}

int main() {
    serverConnect();

    sendData("Server Online!");
    cout << ">: " << receiveData() << endl;
    
    long long g, d;
    long long y = generate_pub_key(g, d); 
    string public_key = "("+to_string(PRIME_P)+", "+to_string(g)+", "+to_string(y)+")";
    sendData(public_key);
    cout << ">: Public key sent! (p,g,y): " << public_key << endl;
    
    string CipherText = receiveData();
    cout << ">: Message received from client! (c1, c2): " << CipherText << endl;
    long long c1, c2;
    sscanf(CipherText.c_str(), "(%lld, %lld)", &c1, &c2);
    cout << c1 << " " << c2 << endl;
    long long s = power_mod(c1, d, PRIME_P);
    long long s_inv = power_mod(s, PRIME_P-2, PRIME_P); //fermat's theorem
    
    long long decrypted_msg = power_mod(c2, 1, PRIME_P)*power_mod(s_inv, 1, PRIME_P);
    if (decrypted_msg > PRIME_P) decrypted_msg = power_mod(decrypted_msg, 1, PRIME_P);
    
    cout << ">: Decrypted Message: " << decrypted_msg << endl;
    
    cout << ">: Server Exiting!!" << endl << endl;
    close(serverSocket);
    
    return 0;
}
