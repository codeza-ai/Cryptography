#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <iostream>
#include <string>
#include <cryptopp/md5.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

using namespace std;
using namespace CryptoPP;

string hashMD5(const string& input) {
    string digest;
    Weak::MD5 md5;
    StringSource ss(input, true,
        new HashFilter(md5, new HexEncoder(new StringSink(digest)))
    );
    return digest;
}


string hashSHA1(const string& input) {
    string digest;
    SHA1 sha1;
    StringSource ss(input, true,
        new HashFilter(sha1, new HexEncoder(new StringSink(digest)))
    );
    return digest;
}

string hashSHA256(const string& input) {
    string digest;
    SHA256 sha256;
    StringSource ss(input, true,
        new HashFilter(sha256, new HexEncoder(new StringSink(digest)))
    );
    return digest;
}

string hashSHA512(const string& input) {
    string digest;
    SHA512 sha512;
    StringSource ss(input, true,
        new HashFilter(sha512, new HexEncoder(new StringSink(digest)))
    );
    return digest;
}

int main() {
    string input;
    cout << "Enter text to hash: ";
    getline(cin, input);

    cout << "\n--- Hash Results ---" << endl;
    cout << "MD5     : " << hashMD5(input) << endl;
    cout << "SHA1    : " << hashSHA1(input) << endl;
    cout << "SHA256  : " << hashSHA256(input) << endl;
    cout << "SHA512  : " << hashSHA512(input) << endl;

    return 0;
}
