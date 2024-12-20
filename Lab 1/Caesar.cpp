#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class CaesarCipher{
public:
    string encrypt(string text, int key){
        string result = "";
        for (int i=0;i<text.length();i++){
            if (isupper(text[i])){
                result += char(int(text[i]+key-65)%26 +65);
            }else{
                result += char(int(text[i]+key-97)%26 +97);
            }
        }
        return result;
    }
    string decrypt(string text, int key){
        string result = "";
        for (int i=0;i<text.length();i++){
            if (isupper(text[i])){
                result += char(int(text[i]-key-65)%26 +65);
            }else{
                result += char(int(text[i]-key-97)%26 +97);
            }
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
    cout<< "Decrypted: " << cipher.decrypt(encrypted, key) << endl;
}