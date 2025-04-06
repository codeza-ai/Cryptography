#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 4096

// Function to generate an RSA key pair
RSA* generateRSAKeyPair() {
    return RSA_generate_key(2048, RSA_F4, NULL, NULL);
}

// Function to sign the hash of the document
std::string signDocument(RSA* rsa, const std::string& document) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)document.c_str(), document.size(), hash);

    unsigned char* signature = new unsigned char[RSA_size(rsa)];
    unsigned int sigLen;
    RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, signature, &sigLen, rsa);

    std::string signedData(reinterpret_cast<char*>(signature), sigLen);
    delete[] signature;
    return signedData;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Generate RSA Key Pair
    RSA* rsa = generateRSAKeyPair();

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed!" << std::endl;
        return -1;
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed!" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;

    // Accept client connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed!" << std::endl;
        return -1;
    }

    // Receive document from client
    read(new_socket, buffer, BUFFER_SIZE);
    std::string document(buffer);

    // Generate digital signature
    std::string signature = signDocument(rsa, document);

    // Send signature back to client
    send(new_socket, signature.c_str(), signature.size(), 0);

    // Cleanup
    close(new_socket);
    close(server_fd);
    RSA_free(rsa);
    return 0;
}
