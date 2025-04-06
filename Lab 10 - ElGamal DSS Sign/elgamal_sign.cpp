#include <iostream>
#include <fstream>
#include <cryptopp/elgamal.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <cryptopp/dsa.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;
using namespace std;

string getFileContent(const string& filename) {
    ifstream file(filename);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;
}

int main() {
    AutoSeededRandomPool rng;

    // Generate ElGamal Keys
    ElGamalKeys::Signer signer;
    ElGamalKeys::Verifier verifier;
    InvertibleElGamalFunction privKey;
    ElGamalFunction pubKey;

    privKey.Initialize(rng, 512);
    pubKey = ElGamalFunction(privKey);

    signer.AccessKey().Initialize(privKey.GetGroupParameters(), privKey.GetSubgroupOrder(), privKey.GetSubgroupGenerator(), privKey.GetPrivateExponent());
    verifier.AccessKey().Initialize(pubKey.GetGroupParameters(), pubKey.GetSubgroupOrder(), pubKey.GetSubgroupGenerator(), pubKey.GetPublicElement());

    // Read file
    string message = getFileContent("message.txt");

    // Hash the message
    SHA256 hash;
    string digest;
    StringSource ss(message, true, new HashFilter(hash, new StringSink(digest)));

    // Sign the digest
    string signature;
    StringSource ss2(digest, true, new SignerFilter(rng, signer, new StringSink(signature)));

    cout << "Signature created successfully.\n";

    // Verify signature
    bool result = false;
    try {
        StringSource ss3(digest + signature, true,
            new SignatureVerificationFilter(verifier, new ArraySink((byte*)&result, sizeof(result))));
    } catch (const Exception& e) {
        cerr << "Verification failed: " << e.what() << endl;
    }

    cout << "Verification: " << (result ? "Success" : "Failure") << endl;
    return 0;
}
