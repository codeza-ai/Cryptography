#include <iostream>
#include <string>
#include <vector>

using namespace std;

class HillCipher{
    vector<vector<int>> keyMatrix;
    void getKeyMatrix(string key){
        int keySize = sqrt(key.length());

        keyMatrix.resize(keySize);
        for (int i=0;i<keySize;i++){
            keyMatrix[i].resize(keySize);
        }
        int k = 0;
        for (int i=0;i<keySize;i++){
            for (int j=0;j<keySize;j++){
                keyMatrix[i][j] = (key[k])%65;
                k++;
            }
        }
    }
public:
    string encrypt(string text, string key){
        string result = "";
        
        getKeyMatrix(key);
        

        return result;
    }
    string decrypt(string text, string key){
        string result = "";
        return result;
    }
};

int main() {
    String plaintext = "Attack at dawn";
    String key = "HILL";

    return 0;
}