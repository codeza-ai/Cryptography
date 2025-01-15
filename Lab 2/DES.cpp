#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class DES
{
    int sbox[8][4][16] = {
        {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
         {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
         {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
         {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 12, 5, 2, 11}},
        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 15, 4},
         {13, 8, 10, 1, 3, 15, 4, 2, 11, 14, 7, 12, 0, 5, 9, 6},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 9, 0, 14, 2},
         {7, 11, 6, 13, 14, 3, 0, 9, 10, 1, 2, 8, 5, 12, 4, 15}},
        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
         {14, 11, 2, 12, 4, 7, 13, 1, 5, 8, 15, 10, 3, 9, 0, 6},
         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 14, 0},
         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
         {9, 14, 15, 5, 1, 3, 8, 13, 12, 7, 11, 4, 10, 2, 0, 6},
         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 9, 2, 8, 5, 6, 0},
         {6, 11, 13, 8, 1, 10, 4, 7, 9, 3, 15, 5, 0, 14, 2, 12}},
        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 15, 13, 3, 5, 8},
         {2, 9, 6, 8, 12, 5, 15, 10, 7, 13, 3, 14, 11, 4, 1, 0}}};

    int bin2dec(int binary)
    {
        int decimal = 0, i = 0;
        while (binary != 0)
        {
            int dec = binary % 10;
            decimal = decimal + dec * pow(2, i);
            binary = binary / 10;
            i++;
        }
        return decimal;
    }

    string dec2bin(int num)
    {
        string res = "";
        while (num > 0)
        {
            res = to_string(num % 2) + res;
            num /= 2;
        }
        return res;
    }

    string xor_func(string a, string b)
    {
        string ans = "";
        for (int i = 0; i < a.length(); i++)
        {
            if (a[i] == b[i])
            {
                ans = ans + "0";
            }
            else
            {
                ans = ans + "1";
            }
        }
        return ans;
    }

public:
    string atobConv(string asciistr)
    {
        string binstr = "";
        for (char c : asciistr)
        {
            int asciiValue = static_cast<int>(c);
            bitset<8> binary(asciiValue);
            binstr += binary.to_string();
        }
        return binstr;
    }
    string btoaConv(string binstr)
    {
        string asciistr = "";
        for (int i = 0; i < binstr.length(); i += 8)
        {
            string byte = binstr.substr(i, 8);
            bitset<8> binary(byte);
            char asciiChar = (char)(binary.to_ulong());
            asciistr += asciiChar;
        }
        return asciistr;
    }
    string permute(string k, vector<int> arr, int n)
    {
        string permutation = "";
        for (int i = 0; i < n; i++)
        {
            permutation = permutation + k[arr[i] - 1];
        }
        return permutation;
    }

    string shift_left(string k, int nth_shifts)
    {
        string s = "";
        for (int i = 0; i < nth_shifts; i++)
        {
            for (int j = 1; j < k.length(); j++)
            {
                s = s + k[j];
            }
            s = s + k[0];
            k = s;
            s = "";
        }
        return k;
    }
    string encrypt(string pt, vector<string> rkb, vector<string> rk)
    {
        pt = atobConv(pt);
        pt = permute(pt, {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7}, 64);
        string left = pt.substr(0, 32);
        string right = pt.substr(32, 32);
        for (int i = 0; i < 16; i++)
        {
            string right_expanded = permute(right, {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1}, 48);
            string xor_x = xor_func(right_expanded, rkb[i]);
            string sbox_str = "";
            for (int j = 0; j < 8; j++)
            {
                int row = bin2dec(xor_x[j * 6] - '0' + xor_x[j * 6 + 5] - '0');
                int col = bin2dec((xor_x[j * 6 + 1] - '0') * 8 + (xor_x[j * 6 + 2] - '0') * 4 + (xor_x[j * 6 + 3] - '0') * 2 + (xor_x[j * 6 + 4] - '0'));
                int val = sbox[j][row][col];
                sbox_str += dec2bin(val);
            }
            sbox_str = permute(sbox_str, {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25}, 32);
            string result = xor_func(left, sbox_str);
            left = result;
            if (i != 15)
            {
                swap(left, right);
            }
        }
        string combine = left + right;
        string cipher_text = permute(combine, {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25}, 64);
        return cipher_text;
    }
    string decrypt(string ct, vector<string> rkb, vector<string> rk)
    {
        // Convert ASCII text to binary
        ct = atobConv(ct);

        // Initial Permutation
        ct = permute(ct,
                     {58, 50, 42, 34, 26, 18, 10, 2,
                      60, 52, 44, 36, 28, 20, 12, 4,
                      62, 54, 46, 38, 30, 22, 14, 6,
                      64, 56, 48, 40, 32, 24, 16, 8,
                      57, 49, 41, 33, 25, 17, 9, 1,
                      59, 51, 43, 35, 27, 19, 11, 3,
                      61, 53, 45, 37, 29, 21, 13, 5,
                      63, 55, 47, 39, 31, 23, 15, 7},
                     64);

        // Split the text into left and right halves
        string left = ct.substr(0, 32);
        string right = ct.substr(32, 32);

        // Apply 16 rounds of decryption
        for (int i = 15; i >= 0; i--)
        {
            // Expansion permutation for the right half
            string right_expanded = permute(right,
                                            {32, 1, 2, 3, 4, 5, 4, 5,
                                             6, 7, 8, 9, 8, 9, 10, 11,
                                             12, 13, 12, 13, 14, 15, 16, 17,
                                             16, 17, 18, 19, 20, 21, 20, 21,
                                             22, 23, 24, 25, 24, 25, 26, 27,
                                             28, 29, 28, 29, 30, 31, 32, 1},
                                            48);

            // XOR right half with the i-th round key
            string xor_x = xor_func(right_expanded, rkb[i]);

            // S-box substitution
            string sbox_str = "";
            for (int j = 0; j < 8; j++)
            {
                int row = bin2dec(xor_x[j * 6] - '0' + xor_x[j * 6 + 5] - '0');
                int col = bin2dec((xor_x[j * 6 + 1] - '0') * 8 +
                                  (xor_x[j * 6 + 2] - '0') * 4 +
                                  (xor_x[j * 6 + 3] - '0') * 2 +
                                  (xor_x[j * 6 + 4] - '0'));
                int val = sbox[j][row][col];
                sbox_str += dec2bin(val);
            }

            // Permutation after S-box
            sbox_str = permute(sbox_str,
                               {16, 7, 20, 21, 29, 12, 28, 17,
                                1, 15, 23, 26, 5, 18, 31, 10,
                                2, 8, 24, 14, 32, 27, 3, 9,
                                19, 13, 30, 6, 22, 11, 4, 25},
                               32);

            // XOR with left half
            string result = xor_func(left, sbox_str);

            // Swap left and right for next round (except last round)
            left = result;
            if (i != 0)
            {
                swap(left, right);
            }
        }

        // Combine left and right halves
        string combine = left + right;

        // Final permutation
        string plain_text = permute(combine,
                                    {40, 8, 48, 16, 56, 24, 64, 32,
                                     39, 7, 47, 15, 55, 23, 63, 31,
                                     38, 6, 46, 14, 54, 22, 62, 30,
                                     37, 5, 45, 13, 53, 21, 61, 29,
                                     36, 4, 44, 12, 52, 20, 60, 28,
                                     35, 3, 43, 11, 51, 19, 59, 27,
                                     34, 2, 42, 10, 50, 18, 58, 26,
                                     33, 1, 41, 9, 49, 17, 57, 25},
                                    64);

        // Convert binary text to ASCII
        return btoaConv(plain_text);
    }
};

int main() {
    string pt = "hi_there";  // Plaintext
    string key = "superkey"; // Key
    DES obj;

    key = obj.atobConv(key);
    key = obj.permute(key, {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4}, 56);

    vector<string> rkb, rk;
    string left = key.substr(0, 28);
    string right = key.substr(28, 28);

    vector<int> shift_table = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    vector<int> key_comp = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

    for (int i = 0; i < 16; i++) {
        left = obj.shift_left(left, shift_table[i]);
        right = obj.shift_left(right, shift_table[i]);
        string combine = left + right;
        rkb.push_back(obj.permute(combine, key_comp, 48));
    }

    string ct = obj.encrypt(pt, rkb, rk);
    cout << "Ciphertext: " << ct << endl;

    string decrypted_pt = obj.decrypt(ct, rkb, rk);
    cout << "Decrypted Plaintext: " << decrypted_pt << endl;

    return 0;
}

