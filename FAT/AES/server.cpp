#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;

#define PORT 8080

int main() {
    byte key[AES::DEFAULT_KEYLENGTH] = "thisisasecretkey"; // 16-byte AES key
    ECB_Mode<AES>::Decryption decryptor;
    decryptor.SetKey(key, sizeof(key));

    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    cout << "Server is listening on port " << PORT << "...\n";

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    int valread = read(new_socket, buffer, 1024);

    // Decrypt
    string decrypted;
    try {
        StringSource ss(reinterpret_cast<const byte*>(buffer), valread, true,
            new StreamTransformationFilter(decryptor, new StringSink(decrypted)));
    } catch (const Exception& e) {
        cerr << "Decryption failed: " << e.what() << endl;
        close(new_socket);
        return 1;
    }

    cout << "Decrypted message: " << decrypted << endl;

    close(new_socket);
    return 0;
}
