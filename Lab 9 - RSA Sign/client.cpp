#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 4096

// Function to verify the document signature
bool verifySignature(RSA* rsa, const std::string& document, const std::string& signature) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)document.c_str(), document.size(), hash);

    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH,
                            (unsigned char*)signature.c_str(), signature.size(), rsa);
    return result == 1;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // Document to be verified
    std::string document = "This is a test document.";
    send(sock, document.c_str(), document.size(), 0);

    // Receive the signature from the server
    read(sock, buffer, BUFFER_SIZE);
    std::string signature(buffer);

    // Load public key for verification
    RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);  // This should be replaced with the actual public key
    bool isValid = verifySignature(rsa, document, signature);

    if (isValid)
        std::cout << "Signature is valid!" << std::endl;
    else
        std::cout << "Signature verification failed!" << std::endl;

    close(sock);
    RSA_free(rsa);
    return 0;
}
