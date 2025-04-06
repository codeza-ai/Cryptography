#include <bits/stdc++.h>

using namespace std;

class PlayfairCipher {
    vector<vector<char>> matrix;
    unordered_map<char, pair<int, int>> map;

public:
    PlayfairCipher(string key) {
        matrix = vector<vector<char>>(5, vector<char>(5));
        unordered_set<char> used;
        int index = 0;

        for (char c : key) {
            if (c == 'J') c = 'I';
            if (used.find(c) == used.end()) {
                used.insert(c);
                matrix[index / 5][index % 5] = c;
                map[c] = {index / 5, index % 5};
                index++;
            }
        }

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

    string preprocess(string text) {
        string result = "";
        for (char c : text) {
            if (c >= 'a' && c <= 'z') c = toupper(c);
            if (c == 'J') c = 'I';
            if (c >= 'A' && c <= 'Z') result += c;
        }

        string processed = "";
        for (int i = 0; i < result.length(); i++) {
            processed += result[i];
            if (i + 1 < result.length() && result[i] == result[i + 1]) {
                processed += 'X';
            }
        }
        if (processed.length() % 2 != 0) {
            processed += 'X';
        }
        return processed;
    }

    string encrypt(string text) {
        text = preprocess(text);
        string result = "";

        for (int i = 0; i < text.length(); i += 2) {
            char a = text[i];
            char b = text[i + 1];
            pair<int, int> posA = map[a];
            pair<int, int> posB = map[b];

            if (posA.first == posB.first) {
                result += matrix[posA.first][(posA.second + 1) % 5];
                result += matrix[posB.first][(posB.second + 1) % 5];
            } else if (posA.second == posB.second) {
                result += matrix[(posA.first + 1) % 5][posA.second];
                result += matrix[(posB.first + 1) % 5][posB.second];
            } else {
                result += matrix[posA.first][posB.second];
                result += matrix[posB.first][posA.second];
            }
        }
        return result;
    }

    string decrypt(string text) {
        string result = "";

        for (int i = 0; i < text.length(); i += 2) {
            char a = text[i];
            char b = text[i + 1];
            pair<int, int> posA = map[a];
            pair<int, int> posB = map[b];

            if (posA.first == posB.first) {
                result += matrix[posA.first][(posA.second - 1 + 5) % 5];
                result += matrix[posB.first][(posB.second - 1 + 5) % 5];
            } else if (posA.second == posB.second) {
                result += matrix[(posA.first - 1 + 5) % 5][posA.second];
                result += matrix[(posB.first - 1 + 5) % 5][posB.second];
            } else {
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
