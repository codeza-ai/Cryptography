#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class HillCipher {
    vector<vector<int>> keyMatrix, inverseKeyMatrix;

    void getKeyMatrix(string key) {
        int keySize = sqrt(key.length());
        keyMatrix.resize(keySize, vector<int>(keySize));
        int k = 0;
        for (int i = 0; i < keySize; i++) {
            for (int j = 0; j < keySize; j++) {
                keyMatrix[i][j] = (key[k] - 'A') % 26;
                k++;
            }
        }
        calculateInverseKeyMatrix();
    }

    void calculateInverseKeyMatrix() {
        int keySize = keyMatrix.size();
        inverseKeyMatrix.resize(keySize, vector<int>(keySize));
        int determinant = 0;

        for (int i = 0; i < keySize; i++) {
            determinant = (determinant + keyMatrix[0][i] *
                           (keyMatrix[1][(i + 1) % 3] * keyMatrix[2][(i + 2) % 3] -
                            keyMatrix[1][(i + 2) % 3] * keyMatrix[2][(i + 1) % 3])) % 26;
        }

        determinant = (determinant + 26) % 26;
        int determinantInverse = -1;
        for (int i = 0; i < 26; i++) {
            if ((determinant * i) % 26 == 1) {
                determinantInverse = i;
                break;
            }
        }

        for (int i = 0; i < keySize; i++) {
            for (int j = 0; j < keySize; j++) {
                int cofactor = ((keyMatrix[(j + 1) % 3][(i + 1) % 3] * keyMatrix[(j + 2) % 3][(i + 2) % 3]) -
                                (keyMatrix[(j + 1) % 3][(i + 2) % 3] * keyMatrix[(j + 2) % 3][(i + 1) % 3])) % 26;
                inverseKeyMatrix[i][j] = (determinantInverse * cofactor) % 26;
                if (inverseKeyMatrix[i][j] < 0) {
                    inverseKeyMatrix[i][j] += 26;
                }
            }
        }
    }

    vector<int> textToVector(string text, int size) {
        vector<int> vec(size);
        for (int i = 0; i < size; i++) {
            vec[i] = text[i] - 'a';
        }
        return vec;
    }

    string vectorToText(vector<int> vec) {
        string text = "";
        for (int val : vec) {
            text += char((val % 26) + 'a');
        }
        return text;
    }

    string processText(string text, vector<vector<int>> &matrix) {
        string result = "";
        int keySize = matrix.size();
        for (int i = 0; i < text.length(); i += keySize) {
            vector<int> vec = textToVector(text.substr(i, keySize), keySize);
            vector<int> resVec(keySize, 0);
            for (int row = 0; row < keySize; row++) {
                for (int col = 0; col < keySize; col++) {
                    resVec[row] += matrix[row][col] * vec[col];
                }
            }
            result += vectorToText(resVec);
        }
        return result;
    }

public:
    string encrypt(string text, string key) {
        for (char &c : text) {
            c = tolower(c);
        }
        getKeyMatrix(key);
        while (text.length() % keyMatrix.size() != 0) {
            text += 'x';
        }
        string result = processText(text, keyMatrix);
        for (char &c : result) {
            c = toupper(c);
        }
        return result;
    }

    string decrypt(string text, string key) {
        for (char &c : text) {
            c = tolower(c);
        }
        getKeyMatrix(key);
        string result = processText(text, inverseKeyMatrix);
        for (char &c : result) {
            c = toupper(c);
        }
        return result;
    }
};

int main() {
    HillCipher cipher;
    string plaintext = "attackatdawn";
    string key = "GYBNQKURP";

    string encrypted = cipher.encrypt(plaintext, key);
    cout << "Encrypted: " << encrypted << endl;

    string decrypted = cipher.decrypt(encrypted, key);
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}
