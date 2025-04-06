#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <stdexcept>
#include <stdint.h>

using namespace std;

class AES {
private:
    vector<uint8_t> key; // Original encryption key
    vector<uint8_t> expandedKey; // Expanded key for rounds
    array<array<uint8_t, 4>, 4> state; // State matrix
    

    static const int Nb = 4; // Number of columns in the state
    static const int Nk = 4; // Number of 32-bit words in the key
    static const int Nr = 10; // Number of rounds for AES-128

    static const uint8_t sBox[256]; // Substitute Byte S-Box
    static const uint8_t invSBox[256];
        // Add the 256 values for the AES inverse S-Box here
    static const uint8_t rCon[10]; // Round constants for key schedule

    // Helper to get index for expanded key
    uint8_t getExpandedKeyByte(int round, int row, int col) const {
        return expandedKey[round * Nb * 4 + col * Nb + row];
    }
    uint8_t gfMul(uint8_t a, uint8_t b)
    {
        uint8_t result = 0;
        while (b > 0)
        {
            if (b & 1)
            {
                result ^= a; // Add (XOR) if the least significant bit of b is 1
            }
            a = (a << 1) ^ ((a & 0x80) ? 0x1b : 0x00); // Shift left and reduce by AES modulus if needed
            b >>= 1;                                   // Shift b to the right
        }
        return result;
    }

public:
    AES(const vector<uint8_t>& keyInput) : key(keyInput) {
        if (key.size() != Nk * 4) {
            throw invalid_argument("Key must be 128 bits (16 bytes) for AES-128.");
        }
        expandedKey.resize(Nb * (Nr + 1) * 4);
        initializeState();
        keyExpansion();
    }

    void initializeState() {
        state.fill({0});
    }

    void keyExpansion() {
        int i = 0;
        while (i < Nk) {
            expandedKey[i * 4 + 0] = key[i * 4 + 0];
            expandedKey[i * 4 + 1] = key[i * 4 + 1];
            expandedKey[i * 4 + 2] = key[i * 4 + 2];
            expandedKey[i * 4 + 3] = key[i * 4 + 3];
            i++;
        }
        while (i < Nb * (Nr + 1)) {
            uint8_t temp[4];
            temp[0] = expandedKey[(i - 1) * 4 + 0];
            temp[1] = expandedKey[(i - 1) * 4 + 1];
            temp[2] = expandedKey[(i - 1) * 4 + 2];
            temp[3] = expandedKey[(i - 1) * 4 + 3];

            if (i % Nk == 0) {
                uint8_t t = temp[0];
                temp[0] = sBox[temp[1]] ^ rCon[i / Nk - 1];
                temp[1] = sBox[temp[2]];
                temp[2] = sBox[temp[3]];
                temp[3] = sBox[t];
            }

            expandedKey[i * 4 + 0] = expandedKey[(i - Nk) * 4 + 0] ^ temp[0];
            expandedKey[i * 4 + 1] = expandedKey[(i - Nk) * 4 + 1] ^ temp[1];
            expandedKey[i * 4 + 2] = expandedKey[(i - Nk) * 4 + 2] ^ temp[2];
            expandedKey[i * 4 + 3] = expandedKey[(i - Nk) * 4 + 3] ^ temp[3];
            i++;
        }
    }

    void addRoundKey(int round) {
        for (int col = 0; col < Nb; ++col) {
            for (int row = 0; row < 4; ++row) {
                state[row][col] ^= getExpandedKeyByte(round, row, col);
            }
        }
    }

    void subBytes() {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                state[row][col] = sBox[state[row][col]];
            }
        }
    }

    void shiftRows() {
        uint8_t temp;

        // Row 1: Rotate left by 1
        temp = state[1][0];
        state[1][0] = state[1][1];
        state[1][1] = state[1][2];
        state[1][2] = state[1][3];
        state[1][3] = temp;

        // Row 2: Rotate left by 2
        temp = state[2][0];
        state[2][0] = state[2][2];
        state[2][2] = temp;
        temp = state[2][1];
        state[2][1] = state[2][3];
        state[2][3] = temp;

        // Row 3: Rotate left by 3
        temp = state[3][3];
        state[3][3] = state[3][2];
        state[3][2] = state[3][1];
        state[3][1] = state[3][0];
        state[3][0] = temp;
    }

    void mixColumns() {
        for (int col = 0; col < Nb; ++col) {
            uint8_t a[4], b[4];
            for (int row = 0; row < 4; ++row) {
                a[row] = state[row][col];
                b[row] = (state[row][col] << 1) ^ (0x1b & -(state[row][col] >> 7));
            }
            state[0][col] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
            state[1][col] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
            state[2][col] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
            state[3][col] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];
        }
    }

    // Utility to print the state for debugging
    void printState() const {
        for (const auto& row : state) {
            for (const auto& val : row) {
                cout << hex << setw(2) << setfill('0') << static_cast<int>(val) << " ";
            }
            cout << endl;
        }
    }
    void invSubBytes()
    {
        for (int row = 0; row < 4; ++row)
        {
            for (int col = 0; col < Nb; ++col)
            {
                state[row][col] = invSBox[state[row][col]];
            }
        }
    }
    void invShiftRows()
    {
        // Row 0 remains unchanged

        // Row 1: Right rotate by 1
        uint8_t temp = state[1][3];
        state[1][3] = state[1][2];
        state[1][2] = state[1][1];
        state[1][1] = state[1][0];
        state[1][0] = temp;

        // Row 2: Right rotate by 2
        swap(state[2][0], state[2][2]);
        swap(state[2][1], state[2][3]);

        // Row 3: Right rotate by 3 (equivalent to left rotate by 1)
        temp = state[3][0];
        state[3][0] = state[3][1];
        state[3][1] = state[3][2];
        state[3][2] = state[3][3];
        state[3][3] = temp;
    }
    void invMixColumns()
    {
        for (int col = 0; col < Nb; ++col)
        {
            uint8_t a0 = state[0][col];
            uint8_t a1 = state[1][col];
            uint8_t a2 = state[2][col];
            uint8_t a3 = state[3][col];

            state[0][col] = gfMul(a0, 0x0e) ^ gfMul(a1, 0x0b) ^ gfMul(a2, 0x0d) ^ gfMul(a3, 0x09);
            state[1][col] = gfMul(a0, 0x09) ^ gfMul(a1, 0x0e) ^ gfMul(a2, 0x0b) ^ gfMul(a3, 0x0d);
            state[2][col] = gfMul(a0, 0x0d) ^ gfMul(a1, 0x09) ^ gfMul(a2, 0x0e) ^ gfMul(a3, 0x0b);
            state[3][col] = gfMul(a0, 0x0b) ^ gfMul(a1, 0x0d) ^ gfMul(a2, 0x09) ^ gfMul(a3, 0x0e);
        }
    }

    void encrypt(vector<uint8_t> &input)
    {
        if (input.size() != Nb * 4)
        {
            throw invalid_argument("Input block size must be 16 bytes (128 bits).");
        }

        // Load the input into the state matrix
        for (int i = 0; i < Nb * 4; ++i)
        {
            state[i % 4][i / 4] = input[i];
        }

        // Initial round: AddRoundKey
        addRoundKey(0);

        // Main rounds
        for (int round = 1; round < Nr; ++round)
        {
            subBytes();         // Substitute bytes using S-Box
            shiftRows();        // Shift rows
            mixColumns();       // Mix columns
            addRoundKey(round); // Add round key
        }

        // Final round (no MixColumns)
        subBytes();
        shiftRows();
        addRoundKey(Nr);

        // Write the state back to the output
        for (int i = 0; i < Nb * 4; ++i)
        {
            input[i] = state[i % 4][i / 4];
        }
    }
    void decrypt(vector<uint8_t> &input)
    {
        if (input.size() != Nb * 4)
        {
            throw invalid_argument("Input block size must be 16 bytes (128 bits).");
        }

        // Load the input into the state matrix
        for (int i = 0; i < Nb * 4; ++i)
        {
            state[i % 4][i / 4] = input[i];
        }

        // Initial round: AddRoundKey with the final round key
        addRoundKey(Nr);

        // Main rounds (Nr-1 to 1)
        for (int round = Nr - 1; round > 0; --round)
        {
            invShiftRows();     // Inverse shift rows
            invSubBytes();      // Inverse substitute bytes using inverse S-Box
            addRoundKey(round); // Add round key
            invMixColumns();    // Inverse mix columns
        }

        // Final round (no InvMixColumns)
        invShiftRows();
        invSubBytes();
        addRoundKey(0);

        // Write the state back to the output
        for (int i = 0; i < Nb * 4; ++i)
        {
            input[i] = state[i % 4][i / 4];
        }
    }
};

// Define the S-Box and RCon during implementation
const uint8_t AES::sBox[256] = {
    // 0     1    2     3     4    5     6    7     8    9     a    b     c    d     e    f
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // a
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // b
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // c
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // d
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // e
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // f
};

const uint8_t AES::invSBox[256] = {
    // 0     1    2     3     4    5     6    7     8    9     a    b     c    d     e    f
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, // 0
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, // 1
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, // 2
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, // 3
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, // 4
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, // 5
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, // 6
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, // 7
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, // 8
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, // 9
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, // a
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, // b
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, // c
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, // d
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, // e
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d  // f
};
const uint8_t AES::rCon[10] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

int main()
{
    vector<uint8_t> key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                                0xab, 0xf7, 0xcf, 0x0c, 0x1f, 0x83, 0x45, 0x65};
    // Example 16-byte ciphertext block
    vector<uint8_t> plaintext = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
                                      0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};

    try
    {
        AES aes(key);
        aes.encrypt(plaintext);

        cout << "Encrypted block: ";
        for (uint8_t byte : plaintext)
        {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
        }
        cout << endl;
    }
    catch (const exception &ex)
    {
        cerr << ex.what() << endl;
    }
    vector<uint8_t> ciphertext = plaintext;
    // Example 16-byte key
    try
    {
        AES aes(key);
        aes.decrypt(ciphertext);

        cout << "Decrypted block: ";
        for (uint8_t byte : ciphertext)
        {
            cout << hex << setw(2) << setfill('0') << static_cast<int>(byte) << " ";
        }
        cout << endl;
    }
    catch (const exception &ex)
    {
        cerr << ex.what() << endl;
    }

    return 0;
}
