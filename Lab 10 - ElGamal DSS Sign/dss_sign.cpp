#include <iostream>
#include <fstream>
#include <cryptopp/dsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

using namespace CryptoPP;
using namespace std;

string getFileContent(const string& filename) {
    ifstream file(filename);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

int main() {
    AutoSeededRandomPool rng;

    // Generate DSA Keys
    DSA::PrivateKey privateKey;
    DSA::PublicKey publicKey;

    privateKey.GenerateRandomWithKeySize(rng, 1024);
    privateKey.MakePublicKey(publicKey);

    // Read message
    string message = getFileContent("message.txt");

    // Hash and Sign
    string signature;
    SHA256 hash;
    StringSource ss(message, true,
        new SignerFilter(rng, DSA::Signer(privateKey), new StringSink(signature)));

    cout << "DSS Signature created successfully.\n";

    // Verify
    bool result = false;
    try {
        StringSource ss2(message + signature, true,
            new SignatureVerificationFilter(DSA::Verifier(publicKey),
                new ArraySink((byte*)&result, sizeof(result))));
    } catch (const Exception& e) {
        cerr << "Verification failed: " << e.what() << endl;
    }

    cout << "Verification: " << (result ? "Success" : "Failure") << endl;
    return 0;
}
