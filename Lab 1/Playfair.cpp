#include <bits/stdc++.h>

using namespace std;

class PlayfairCipher {
    vector<vector<char>> matrix;              // 5x5 matrix for Playfair Cipher
    unordered_map<char, pair<int, int>> map;  // To store positions of characters in the matrix

public:
    // Constructor to initialize the 5x5 matrix using the key
    PlayfairCipher(string key) {
        matrix = vector<vector<char>>(5, vector<char>(5));
        unordered_set<char> used;
        int index = 0;

        // Fill the matrix with the key, avoiding duplicates and treating J as I
        for (char c : key) {
            if (c == 'J') c = 'I'; // Treat 'J' as 'I'
            if (used.find(c) == used.end()) {
                used.insert(c);
                matrix[index / 5][index % 5] = c;
                map[c] = {index / 5, index % 5};
                index++;
            }
        }

        // Fill the remaining slots with the rest of the alphabet (excluding 'J')
        for (char c = 'A'; c <= 'Z'; c++) {
            if (c == 'J') continue;
            if (used.find(c) == used.end()) {
                used.insert(c);
                matrix[index / 5][index % 5] = c;
                map[c] = {index / 5, index % 5};
                index++;
            }
        }
    }

    // Helper method to preprocess the input text for encryption
    string preprocess(string text) {
        string result = "";
        for (char c : text) {
            if (c >= 'a' && c <= 'z') c = toupper(c); // Convert to uppercase
            if (c == 'J') c = 'I';                   // Treat 'J' as 'I'
            if (c >= 'A' && c <= 'Z') result += c;   // Ignore non-alphabetic characters
        }

        // Add padding for consecutive same letters and odd-length text
        string processed = "";
        for (int i = 0; i < result.length(); i++) {
            processed += result[i];
            if (i + 1 < result.length() && result[i] == result[i + 1]) {
                processed += 'X'; // Padding for repeated letters
            }
        }
        if (processed.length() % 2 != 0) {
            processed += 'X'; // Padding for odd length
        }
        return processed;
    }

    // Encryption method
    string encrypt(string text) {
        text = preprocess(text);
        string result = "";

        for (int i = 0; i < text.length(); i += 2) {
            char a = text[i];
            char b = text[i + 1];
            pair<int, int> posA = map[a];
            pair<int, int> posB = map[b];

            if (posA.first == posB.first) {
                // Same row
                result += matrix[posA.first][(posA.second + 1) % 5];
                result += matrix[posB.first][(posB.second + 1) % 5];
            } else if (posA.second == posB.second) {
                // Same column
                result += matrix[(posA.first + 1) % 5][posA.second];
                result += matrix[(posB.first + 1) % 5][posB.second];
            } else {
                // Rectangle swap
                result += matrix[posA.first][posB.second];
                result += matrix[posB.first][posA.second];
            }
        }
        return result;
    }

    // Decryption method
    string decrypt(string text) {
        string result = "";

        for (int i = 0; i < text.length(); i += 2) {
            char a = text[i];
            char b = text[i + 1];
            pair<int, int> posA = map[a];
            pair<int, int> posB = map[b];

            if (posA.first == posB.first) {
                // Same row
                result += matrix[posA.first][(posA.second - 1 + 5) % 5];
                result += matrix[posB.first][(posB.second - 1 + 5) % 5];
            } else if (posA.second == posB.second) {
                // Same column
                result += matrix[(posA.first - 1 + 5) % 5][posA.second];
                result += matrix[(posB.first - 1 + 5) % 5][posB.second];
            } else {
                // Rectangle swap
                result += matrix[posA.first][posB.second];
                result += matrix[posB.first][posA.second];
            }
        }
        return result;
    }
};

int main() {
    string plaintext;
    cout << "Enter the plaintext: ";
    cin >> plaintext;

    PlayfairCipher cipher("MONARCHY");
    string encrypted = cipher.encrypt(plaintext);
    cout << "Encrypted text: " << encrypted << endl;

    string decrypted = cipher.decrypt(encrypted);
    cout << "Decrypted text: " << decrypted << endl;

    return 0;
}
