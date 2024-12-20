#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ViegenereCipher{
    string key;
public:
    ViegenereCipher(string key){
        for (char &c : key) {
            c = tolower(c);
        }
        this->key = key;
    }
    string encrypt(string text){
        char base = 'a';
        int l = text.length();
        int k = key.length();

        string encrypted = "";
        for(int i = 0; i < l; i++){
            char c = tolower(text[i]);
            encrypted += (char)((c + key[i % k] - 2 * base) % 26 + base);
        }
        return encrypted;
    }
    string decrypt(string text){
        char base = 'a';
        int l = text.length();
        int k = key.length();

        string decrypted = "";
        for(int i = 0; i < l; i++){
            char c = tolower(text[i]);
            decrypted += (char)((c - key[i % k] + 26) % 26 + base);
        }
        return decrypted;
    }

};

int main() {
    string text;
    cin>> text;
    ViegenereCipher obj  = ViegenereCipher("deceptive");
    string e = obj.encrypt(text);
    cout<<"Encrypted : "<< e << endl;
    string d = obj.decrypt(e);
    cout<<"Decrypted : "<<d <<endl;

    return 0;
}