#ifndef SDES_H
#define SDES_H

#include <vector>
#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>

using namespace std;
class SDES {
private:
    vector<int> key = {1, 0, 1, 0, 0, 0, 0, 0, 1, 0}; // Example key
    vector<int> P10 = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
    vector<int> P8 = {6, 3, 7, 4, 8, 5, 10, 9};
    vector<int> key1 = vector<int>(8);
    vector<int> key2 = vector<int>(8);
    vector<int> IP = {2, 6, 3, 1, 4, 8, 5, 7};
    vector<int> EP = {4, 1, 2, 3, 2, 3, 4, 1};
    vector<int> P4 = {2, 4, 3, 1};
    vector<int> IP_inv = {4, 1, 3, 5, 7, 2, 8, 6};

    int S0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
    int S1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};

    vector<int> shift(vector<int>& ar, int n);
    string binary_(int val);
    vector<int> function_(vector<int>& ar, vector<int>& key_);
    vector<int> swap(vector<int>& array, int n);

public:
    void key_generation();
    vector<int> encryption(vector<int>& plaintext);
    vector<int> decryption(vector<int>& ar);
};

#endif