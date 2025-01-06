#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class CaesarCipher {
public:
    string encrypt(string text, int key) {
        string result = "";
        for (char &c : text) {
            c = tolower(c);
        }
        for (int i = 0; i < text.length(); i++) {
            if (isalpha(text[i])) {
                result += char(int(text[i] + key - 'a') % 26 + 'a');
            } else {
                result += text[i];
            }
        }
        for (char &c : result) {
            c = toupper(c);
        }
        return result;
    }

    string decrypt(string text, int key) {
        string result = "";
        for (char &c : text) {
            c = tolower(c);
        }
        for (int i = 0; i < text.length(); i++) {
            if (isalpha(text[i])) {
                result += char((int(text[i] - key - 'a' + 26) % 26) + 'a');
            } else {
                result += text[i];
            }
        }
        for (char &c : result) {
            c = toupper(c);
        }
        return result;
    }
};

int main() {
    CaesarCipher cipher;
    string text = "ATTACKATONCE";
    int key = 3;

    string encrypted = cipher.encrypt(text, key);
    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: " << cipher.decrypt(encrypted, key) << endl;

    return 0;
}
