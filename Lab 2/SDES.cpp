#include <iostream>
#include <vector>
#include <string>
#include "SDES.h"
using namespace std;

vector<int> SDES::shift(vector<int>& ar, int n) {
    while (n > 0) {
        int temp = ar[0];
        for (int i = 0; i < ar.size() - 1; i++) {
            ar[i] = ar[i + 1];
        }
        ar[ar.size() - 1] = temp;
        n--;
    }
    return ar;
}

string SDES::binary_(int val) {
    if (val == 0) return "00";
    else if (val == 1) return "01";
    else if (val == 2) return "10";
    else return "11";
}

vector<int> SDES::function_(vector<int>& ar, vector<int>& key_) {
    vector<int> l(4), r(4);

    for (int i = 0; i < 4; i++) {
        l[i] = ar[i];
        r[i] = ar[i + 4];
    }

    vector<int> ep(8);
    for (int i = 0; i < 8; i++) {
        ep[i] = r[EP[i] - 1];
    }

    for (int i = 0; i < 8; i++) {
        ar[i] = key_[i] ^ ep[i];
    }

    vector<int> l_1(4), r_1(4);
    for (int i = 0; i < 4; i++) {
        l_1[i] = ar[i];
        r_1[i] = ar[i + 4];
    }

    int row, col, val;
    row = (l_1[0] << 1) | l_1[3];
    col = (l_1[1] << 1) | l_1[2];
    val = S0[row][col];
    string str_l = binary_(val);

    row = (r_1[0] << 1) | r_1[3];
    col = (r_1[1] << 1) | r_1[2];
    val = S1[row][col];
    string str_r = binary_(val);

    vector<int> r_(4);
    for (int i = 0; i < 2; i++) {
        r_[i] = str_l[i] - '0';
        r_[i + 2] = str_r[i] - '0';
    }

    vector<int> r_p4(4);
    for (int i = 0; i < 4; i++) {
        r_p4[i] = r_[P4[i] - 1];
    }

    for (int i = 0; i < 4; i++) {
        l[i] = l[i] ^ r_p4[i];
    }

    vector<int> output(8);
    for (int i = 0; i < 4; i++) {
        output[i] = l[i];
        output[i + 4] = r[i];
    }
    return output;
}

vector<int> SDES::swap(vector<int>& array, int n) {
    vector<int> l(n), r(n);
    for (int i = 0; i < n; i++) {
        l[i] = array[i];
        r[i] = array[i + n];
    }

    vector<int> output(2 * n);
    for (int i = 0; i < n; i++) {
        output[i] = r[i];
        output[i + n] = l[i];
    }

    return output;
}
void SDES::key_generation() {
    vector<int> key_(10);
    for (int i = 0; i < 10; i++) {
        key_[i] = key[P10[i] - 1];
    }

    vector<int> Ls(5), Rs(5);
    for (int i = 0; i < 5; i++) {
        Ls[i] = key_[i];
        Rs[i] = key_[i + 5];
    }

    vector<int> Ls_1 = shift(Ls, 1);
    vector<int> Rs_1 = shift(Rs, 1);

    for (int i = 0; i < 5; i++) {
        key_[i] = Ls_1[i];
        key_[i + 5] = Rs_1[i];
    }

    for (int i = 0; i < 8; i++) {
        key1[i] = key_[P8[i] - 1];
    }

    vector<int> Ls_2 = shift(Ls, 2);
    vector<int> Rs_2 = shift(Rs, 2);

    for (int i = 0; i < 5; i++) {
        key_[i] = Ls_2[i];
        key_[i + 5] = Rs_2[i];
    }

    for (int i = 0; i < 8; i++) {
        key2[i] = key_[P8[i] - 1];
    }
}

vector<int> SDES::encryption(vector<int>& plaintext) {
    vector<int> arr(8);
    for (int i = 0; i < 8; i++) {
        arr[i] = plaintext[IP[i] - 1];
    }
    vector<int> arr1 = function_(arr, key1);
    vector<int> after_swap = swap(arr1, arr1.size() / 2);
    vector<int> arr2 = function_(after_swap, key2);

    vector<int> ciphertext(8);
    for (int i = 0; i < 8; i++) {
        ciphertext[i] = arr2[IP_inv[i] - 1];
    }

    return ciphertext;
}

vector<int> SDES::decryption(vector<int>& ar) {
    vector<int> arr(8);
    for (int i = 0; i < 8; i++) {
        arr[i] = ar[IP[i] - 1];
    }

    vector<int> arr1 = function_(arr, key2);
    vector<int> after_swap = swap(arr1, arr1.size() / 2);
    vector<int> arr2 = function_(after_swap, key1);

    vector<int> decrypted(8);
    for (int i = 0; i < 8; i++) {
        decrypted[i] = arr2[IP_inv[i] - 1];
    }

    return decrypted;
}