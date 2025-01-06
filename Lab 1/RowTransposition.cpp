#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class RowTransposition {
    vector<vector<char>> matrix;
    vector<int> sequence;

    void createSequence(int length) {
        sequence.resize(length);
        for (int i = 0; i < length; ++i) {
            sequence[i] = i; // Simple sequence: 0, 1, 2, ..., length-1
        }
        random_shuffle(sequence.begin(), sequence.end()); // Shuffle for randomness
    }

public:
    RowTransposition() {}

    string encrypt(string text) {
        int l = ceil(sqrt(text.length()));
        createSequence(l); // Generate random column sequence

        // Create matrix and fill with characters
        matrix.resize(l, vector<char>(l, 'x'));
        int index = 0;
        for (int r = 0; r < l; r++) {
            for (int c = 0; c < l; c++) {
                if (index < text.length()) {
                    matrix[r][c] = text[index];
                    index++;
                }
            }
        }

        // Read columns in sequence order
        string encrypted = "";
        for (int col : sequence) {
            for (int r = 0; r < l; r++) {
                encrypted += matrix[r][col];
            }
        }
        return encrypted;
    }

    string decrypt(string text) {
        int l = ceil(sqrt(text.length()));
        matrix.resize(l, vector<char>(l, 'x'));
        vector<int> reverseSeq(sequence.size());
        for (int i = 0; i < sequence.size(); ++i) {
            reverseSeq[sequence[i]] = i; // Reverse the sequence mapping
        }

        // Fill the matrix column-wise using reverse sequence
        int index = 0;
        for (int col : reverseSeq) {
            for (int r = 0; r < l; r++) {
                if (index < text.length()) {
                    matrix[r][col] = text[index];
                    index++;
                }
            }
        }

        // Read row-wise for plaintext
        string decrypted = "";
        for (int r = 0; r < l; r++) {
            for (int c = 0; c < l; c++) {
                if (matrix[r][c] != 'x') {
                    decrypted += matrix[r][c];
                }
            }
        }
        return decrypted;
    }
};

int main() {
    string plaintext;
    cout << "Enter plain text: ";
    cin >> plaintext;

    RowTransposition cipher;
    string encrypted = cipher.encrypt(plaintext);
    cout << "Encrypted text: " << encrypted << endl;

    string decrypted = cipher.decrypt(encrypted);
    cout << "Decrypted text: " << decrypted << endl;

    return 0;
}
