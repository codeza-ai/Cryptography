#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <unordered_map>
#include <algorithm>
#include "DES.h"

using namespace std;

string DES::ASCIItoHex(string &s)
{
    string hex = "";
    for (char c : s)
    {
        hex += bitset<8>(c).to_string();
    }
    return bin2hex(hex);
}
string DES::HexToASCII(string &s)
{
    string bin = hex2bin(s);
    string ascii = "";
    for (size_t i = 0; i < bin.size(); i += 8)
    {
        bitset<8> bits(bin.substr(i, 8));
        ascii += char(bits.to_ulong());
    }
    return ascii;
}
// Convert Hexadecimal to Binary
string DES::hex2bin(const string &s)
{
    unordered_map<char, string> mp = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};

    string bin = "";
    for (char c : s)
        bin += mp[c];
    return bin;
}

// Convert Binary to Hexadecimal
string DES::bin2hex(const string &s)
{
    unordered_map<string, char> mp = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'}, {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'}, {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'}, {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}};

    string hex = "";
    for (size_t i = 0; i < s.size(); i += 4)
    {
        hex += mp[s.substr(i, 4)];
    }
    return hex;
}

// Convert Binary to Decimal
int DES::bin2dec(const string &binary)
{
    return stoi(binary, nullptr, 2);
}

// Convert Decimal to Binary (4-bit)
string DES::dec2bin(int num)
{
    string res = bitset<4>(num).to_string();
    return res;
}

// Perform Permutation
string DES::permute(const string &k, const vector<int> &arr, int n)
{
    string permuted = "";
    for (int i = 0; i < n; i++)
    {
        permuted += k[arr[i] - 1];
    }
    return permuted;
}

// Perform Left Shift
string DES::shift_left(string k, int nth_shifts)
{
    return k.substr(nth_shifts) + k.substr(0, nth_shifts);
}

// Perform XOR on two binary strings
string DES::xor_op(const string &a, const string &b)
{
    string ans = "";
    for (size_t i = 0; i < a.length(); i++)
    {
        ans += (a[i] == b[i]) ? "0" : "1";
    }
    return ans;
}

// Encrypt Function
string DES::encrypt(string pt)
{
    pt = ASCIItoHex(pt);
    pt = hex2bin(pt);
    pt = permute(pt, initial_perm, 64);

    string left = pt.substr(0, 32);
    string right = pt.substr(32, 32);

    for (int i = 0; i < 16; i++)
    {
        string right_expanded = permute(right, exp_d, 48);
        string xor_x = xor_op(right_expanded, rkb[i]);

        string sbox_str = "";
        for (int j = 0; j < 8; j++)
        {
            int row = bin2dec(xor_x.substr(j * 6, 1) + xor_x.substr(j * 6 + 5, 1));
            int col = bin2dec(xor_x.substr(j * 6 + 1, 4));
            sbox_str += dec2bin(sbox[j][row][col]);
        }

        sbox_str = permute(sbox_str, per, 32);
        string result = xor_op(left, sbox_str);
        left = result;

        if (i != 15)
            swap(left, right);
    }

    string combine = left + right;
    return bin2hex(permute(combine, final_perm, 64));
}
string DES::decrypt(string cipher_text)
{
    reverse(rkb.begin(), rkb.end()); // Reverse the round keys for decryption
    reverse(rk.begin(), rk.end());
    string decrypted = encrypt(cipher_text); // DES decryption is just encryption with reversed keys
    return HexToASCII(decrypted);
}

// Key Generation
void DES::keyGeneration(string key)
{
    key = hex2bin(key);
    key = permute(key, keyp, 56);

    string left = key.substr(0, 28);
    string right = key.substr(28, 28);

    for (int i = 0; i < 16; i++)
    {
        left = shift_left(left, shift_table[i]);
        right = shift_left(right, shift_table[i]);
        string combined_key = left + right;
        string round_key = permute(combined_key, key_comp, 48);
        rkb.push_back(round_key);
        rk.push_back(bin2hex(round_key));
    }
}

// int main()
// {
//     DES des;
//     string pt = "123456ABCD132536";
//     string key = "AABB09182736CCDD";

//     vector<string> rkb, rk;
//     des.keyGeneration(key);

//     cout << "Encryption\n";
//     string cipher_text = des.encrypt(pt);
//     cout << "Cipher Text: " << cipher_text << endl;

//     cout << "Decryption\n";
//     string text = des.decrypt(cipher_text);
//     cout << "Plain Text: " << text << endl;

//     return 0;
// }
