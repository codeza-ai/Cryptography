#include <cstring>
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include "DES.h"

using namespace std;

vector<int> DES::generateIPInverse() {
    vector<int> result;
    for (int i = 1; i <= 64; i++) {
        auto it = find(IP.begin(), IP.end(), i);
        result.push_back((it - IP.begin() + 1));
    }

    return result;
}

string DES::arrangePermutation(string& str, vector<int>& permutation) {
    string result = "";
    for (auto &idx : permutation) {
        result += str[idx - 1];
    }
    return result;
}

string DES::shiftKey(string& key, int& shiftTimes) {
    string result = "";
    for (int i = shiftTimes; i < key.size(); i++) {
        result += key[i];
    }

    for (int i = 0; i < shiftTimes; i++) {
        result += key[i];
    }
    return result;
}

vector<string> DES::generateKeys(vector<int>& rounds,string& key) {
    string keyAfterIP = arrangePermutation(key, IP);

    string leftPartOfKey = keyAfterIP.substr(0, 32);
    string rightPartOfKey = keyAfterIP.substr(32, 32);

    vector<string> result;
    for (int i = 0; i < 16; i++) {
        int shiftTimes = rounds[i];

        leftPartOfKey = shiftKey(leftPartOfKey, shiftTimes);
        rightPartOfKey = shiftKey(rightPartOfKey, shiftTimes);

        string combinedKey = leftPartOfKey + rightPartOfKey;
        string tempResult = arrangePermutation(combinedKey, CP);

        result.push_back(tempResult);
    }

    int i = 15, j = 0;
    while (i > j) {
        swap(result[i], result[j]);
        i--, j++;
    }

    return result;
}

string DES::XOR(string& a, string& b) {
    string result = "";
    for (int i = 0; i < a.length(); i++) {
        result += (a[i] == b[i] ? '0' : '1');
    }

    return result;
}

int DES::BinToDec(string& bin) {
    int result = 0, n = bin.length();

    for (int i = n - 1; i >= 0; i--) {
        result += (bin[i] == '1' ? pow(2, i) : 0);
    }
    return result;
}

string DES::DecToBin(int& dec) {
    string result = "";
    while (dec) {
        result = (dec % 2 == 0 ? '0' : '1') + result;
        dec /= 2;
    }

    while (result.length() < 4) {
        result = '0' + result;
    }

    return result;
}

string DES::DES_Decryption(string& text, vector<int>& IP, vector<string>& sixteenKeys, vector<int>& EP, vector<int>& PF, vector<vector<vector<int>>>& SB, vector<int>& IPInv) {
    string textAfterIP = arrangePermutation(text, IP);

    string leftPartOfText = textAfterIP.substr(0, 32);
    string rightPartOfText = textAfterIP.substr(32);

    for (int i = 0; i < sixteenKeys.size(); i++) {
        string expandedRightPart = arrangePermutation(rightPartOfText, EP);

        string XORedResult = XOR(expandedRightPart, sixteenKeys[i]);

        string tempResult = "";
        for (int j = 0; j < 8; j++)
        {
            string row = XORedResult.substr(j * 6, 1) + XORedResult.substr(j * 6 + 5, 1);
            string col = XORedResult.substr(j * 6 + 1, 1) + XORedResult.substr(j * 6 + 2, 1)
            + XORedResult.substr(j * 6 + 3, 1) + XORedResult.substr(j * 6 + 4, 1);

            int rowVal = BinToDec(row);
            int colVal = BinToDec(col);

            int subsitutionBoxValue = SB[j][rowVal][colVal];

            tempResult += DecToBin(subsitutionBoxValue);
        }

        string tempResultAfterPF = arrangePermutation(tempResult, PF);

        XORedResult = XOR(leftPartOfText, tempResultAfterPF);
        leftPartOfText = XORedResult;
        if (i < 15) {
            swap(leftPartOfText, rightPartOfText);
        }
    }

    string combindedText = leftPartOfText + rightPartOfText;
    string result = arrangePermutation(combindedText, IPInv);

    return result;
}

string DES::DES_Encryption(string& plainText, vector<int>& IP, vector<string>& sixteenKeys, vector<int>& EP, vector<int> PF, vector<vector<vector<int>>>& SB, vector<int>& IPInv) {
    string textAfterIP = arrangePermutation(plainText, IP);

    string leftPartOfText = textAfterIP.substr(0, 32);
    string rightPartOfText = textAfterIP.substr(32);

    for(int i = 0; i < sixteenKeys.size(); i++) {
        string expandedRightPart = arrangePermutation(rightPartOfText, EP);

        string XORedResult = XOR(expandedRightPart, sixteenKeys[i]);

        string tempResult = "";
        for(int j = 0; j < 8; j++) {
            string row = XORedResult.substr(j * 6, 1) + XORedResult.substr(j * 6 + 5, 1);
            string col = XORedResult.substr(j * 6 + 1, 1) + XORedResult.substr(j * 6 + 2, 1) + XORedResult.substr(j * 6 + 3, 1) + XORedResult.substr(j * 6 + 4, 1);

            int rowVal = BinToDec(row);
            int colVal = BinToDec(col);

            int subsitutionBoxValue = SB[j][rowVal][colVal];

            tempResult += DecToBin(subsitutionBoxValue);
        }

        string tempResultAfterPF = arrangePermutation(tempResult, PF);

        XORedResult = XOR(leftPartOfText, tempResultAfterPF);
        leftPartOfText = XORedResult;
        if(i < 15) {
            // Not swapping in the final round
            swap(leftPartOfText, rightPartOfText);
        }
    }

    string combindedText = leftPartOfText + rightPartOfText;
    string result = arrangePermutation(combindedText, IPInv);

    return result;
}

string DES::HexToBin(string& bin) {
    string result = "";
    for (int i = 0; i < bin.length(); i++) {
        switch (bin[i]) {
        case '0':
            result += "0000";
            break;
        case '1':
            result += "0001";
            break;
        case '2':
            result += "0010";
            break;
        case '3':
            result += "0011";
            break;
        case '4':
            result += "0100";
            break;
        case '5':
            result += "0101";
            break;
        case '6':
            result += "0110";
            break;
        case '7':
            result += "0111";
            break;
        case '8':
            result += "1000";
            break;
        case '9':
            result += "1001";
            break;
        case 'A':
        case 'a':
            result += "1010";
            break;
        case 'B':
        case 'b':
            result += "1011";
            break;
        case 'C':
        case 'c':
            result += "1100";
            break;
        case 'D':
        case 'd':
            result += "1101";
            break;
        case 'E':
        case 'e':
            result += "1110";
            break;
        case 'F':
        case 'f':
            result += "1111";
            break;
        }
    }

    return result;
}

string DES::BinToHex(string& hex) {
    string result = "";
    for (int i = 0; i < hex.length(); i += 4) {
        string temp = hex.substr(i, 4);
        if (temp == "0000"){
            result += "0";
        }
        else if (temp == "0001") {
            result += "1";
        }
        else if (temp == "0010") {
            result += "2";
        }
        else if (temp == "0011") {
            result += "3";
        }
        else if (temp == "0100") {
            result += "4";
        }
        else if (temp == "0101") {
            result += "5";
        }
        else if (temp == "0110") {
            result += "6";
        }
        else if (temp == "0111") {
            result += "7";
        }
        else if (temp == "1000") {
            result += "8";
        }
        else if (temp == "1001") {
            result += "9";
        }
        else if (temp == "1010") {
            result += "a";
        }
        else if (temp == "1011") {
            result += "b";
        }
        else if (temp == "1100") {
            result += "c";
        }
        else if (temp == "1101") {
            result += "d";
        }
        else if (temp == "1110") {
            result += "e";
        }
        else if (temp == "1111") {
            result += "f";
        }
    }

    return result;
}

string DES::atobConv(string asciistr) {
    string binstr="";
    for(char c:asciistr){
        int asciiValue = static_cast<int>(c);
        bitset<8> binary(asciiValue);
        binstr+=binary.to_string();
    }
    return binstr;
}

string DES::btoaConv(string binstr) {
    string asciistr = "";
    for (int i = 0; i < binstr.length(); i += 8) { 
        string byte = binstr.substr(i, 8);
        bitset<8> binary(byte); 
        char asciiChar = (char)(binary.to_ulong()); 
        asciistr += asciiChar; 
    }
    return asciistr;
}
