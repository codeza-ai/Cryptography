#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class RotorCipher {
private:
    vector<int> rotor1;
    vector<int> rotor2;
    vector<int> rotor3;

    int rotor1Position;
    int rotor2Position;
    int rotor3Position;

    void resetRotors() {
        rotor1 = {1, 5, 13, 6, 10, 25, 2, 12, 24, 0, 11, 9, 3, 4, 18, 7, 21, 8, 17, 20, 23, 19, 14, 15, 22, 16};
        rotor2 = {5, 10, 7, 14, 2, 8, 12, 11, 13, 4, 6, 15, 0, 17, 18, 21, 25, 24, 23, 16, 3, 20, 9, 19, 22, 1};
        rotor3 = {2, 5, 8, 11, 14, 17, 20, 23, 0, 1, 4, 7, 10, 13, 16, 19, 22, 25, 24, 21, 18, 15, 12, 9, 6, 3};
        rotate(rotor1.begin(), rotor1.begin() + rotor1Position, rotor1.end());
        rotate(rotor2.begin(), rotor2.begin() + rotor2Position, rotor2.end());
        rotate(rotor3.begin(), rotor3.begin() + rotor3Position, rotor3.end());
    }

    int findIndex(const vector<int>& rotor, int value) {
        return find(rotor.begin(), rotor.end(), value) - rotor.begin();
    }

public:
    RotorCipher(int P1, int P2, int P3) {
        rotor1Position = P1;
        rotor2Position = P2;
        rotor3Position = P3;
        resetRotors();
    }

    string encrypt(string message) {
        resetRotors(); // Ensure rotors are in initial position
        string encrypted = "";
        int n = message.length();

        for (int i = 0; i < n; i++) {
            int letter = message[i] - 'A';

            // Apply rotor transformations
            int encryptedLetter = rotor3[letter];
            encryptedLetter = rotor2[encryptedLetter];
            encryptedLetter = rotor1[encryptedLetter];

            encrypted += (char)(encryptedLetter + 'A');

            // Rotate rotors
            rotate(rotor1.begin(), rotor1.begin() + 1, rotor1.end());
            if ((i + 1) % 2 == 0) {
                rotate(rotor2.begin(), rotor2.begin() + 1, rotor2.end());
            }
            if ((i + 1) % 4 == 0) {
                rotate(rotor3.begin(), rotor3.begin() + 1, rotor3.end());
            }
        }

        return encrypted;
    }

    string decrypt(string cipherText) {
        resetRotors(); // Ensure rotors are in initial position
        string decrypted = "";
        int n = cipherText.length();

        for (int i = 0; i < n; i++) {
            int letter = cipherText[i] - 'A';

            // Reverse rotor transformations
            int decryptedLetter = findIndex(rotor1, letter);
            decryptedLetter = findIndex(rotor2, decryptedLetter);
            decryptedLetter = findIndex(rotor3, decryptedLetter);

            decrypted += (char)(decryptedLetter + 'A');

            // Rotate rotors
            rotate(rotor1.begin(), rotor1.begin() + 1, rotor1.end());
            if ((i + 1) % 2 == 0) {
                rotate(rotor2.begin(), rotor2.begin() + 1, rotor2.end());
            }
            if ((i + 1) % 4 == 0) {
                rotate(rotor3.begin(), rotor3.begin() + 1, rotor3.end());
            }
        }

        return decrypted;
    }
};

int main() {
    RotorCipher obj = RotorCipher(7, 11, 3);
    string message = "DARSHAN";
    string encrypted = obj.encrypt(message);

    cout << "Encrypted message: " << encrypted << endl;

    string decrypted = obj.decrypt(encrypted);
    cout << "Decrypted message: " << decrypted << endl;

    return 0;
}
