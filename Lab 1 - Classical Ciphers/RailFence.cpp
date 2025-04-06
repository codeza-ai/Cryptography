#include <iostream>
#include <string>

using namespace std;

class RailFenceCipher {
public:
    string encrypt(string s) {
        string encrypted = "";
        int l = s.length();

        for (int i = 0; i < l; i += 2) {
            encrypted += s[i];
        }
        for (int i = 1; i < l; i += 2) {
            encrypted += s[i];
        }
        return encrypted;
    }

    string decrypt(string s) {
        int l = s.length();
        string decrypted = "";
        int mid = (l + 1) / 2;

        for (int i = 0; i < mid; ++i) {
            decrypted += s[i];
            if (i + mid < l) {
                decrypted += s[i + mid];
            }
        }
        return decrypted;
    }
};

int main() {
    string plaintext;
    cin >> plaintext;

    RailFenceCipher obj = RailFenceCipher();
    string encrypted = obj.encrypt(plaintext);
    cout << "Encrypted text: " << encrypted << endl;
    string decrypted = obj.decrypt(encrypted);
    cout << "Decrypted text: " << decrypted << endl;

    return 0;
}
