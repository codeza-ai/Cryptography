#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;

#define PORT 8080

int main() {
    byte key[AES::DEFAULT_KEYLENGTH] = "thisisasecretkey"; // 16-byte AES key
    ECB_Mode<AES>::Encryption encryptor;
    encryptor.SetKey(key, sizeof(key));

    string message;
    cout << "Enter message to encrypt and send: ";
    getline(cin, message);

    string encrypted;
    try {
        StringSource ss(message, true,
            new StreamTransformationFilter(encryptor, new StringSink(encrypted)));
    } catch (const Exception& e) {
        cerr << "Encryption error: " << e.what() << endl;
        return 1;
    }

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed\n";
        return 1;
    }

    send(sock, encrypted.c_str(), encrypted.length(), 0);
    cout << "Encrypted message sent to server.\n";

    close(sock);
    return 0;
}
