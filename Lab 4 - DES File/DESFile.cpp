#include <iostream>
#include <fstream>
#include "DES.h" // Assuming DES class is implemented in DES.h and DES.cpp

using namespace std;

void encryptFile(const string &inputFile, const string &encryptedFile, DES &des)
{
    ifstream inFile(inputFile, ios::binary);
    ofstream outFile(encryptedFile, ios::binary);

    if (!inFile || !outFile)
    {
        cerr << "Error opening file!" << endl;
        return;
    }

    string buffer(8, '\0');
    while (inFile.read(&buffer[0], 8) || inFile.gcount() > 0)
    {
        if (inFile.gcount() < 8)
        {
            buffer.resize(8, '\0'); // Pad with null characters if less than 8 characters
        }
        string encryptedText = des.encrypt(buffer);
        outFile << encryptedText;
    }

    inFile.close();
    outFile.close();
}

void decryptFile(const string &encryptedFile, const string &decryptedFile, DES &des)
{
    ifstream inFile(encryptedFile, ios::binary);
    ofstream outFile(decryptedFile, ios::binary);

    if (!inFile || !outFile)
    {
        cerr << "Error opening file!" << endl;
        return;
    }

    string buffer(16, '\0');
    while (inFile.read(&buffer[0], 16) || inFile.gcount() > 0)
    {
        if (inFile.gcount() < 16)
        {
            cerr << "Corrupted encrypted file: incomplete block." << endl;
            break;
        }
        string decryptedText = des.decrypt(buffer);
        outFile << decryptedText;
    }

    inFile.close();
    outFile.close();
}

#include <chrono>

int main()
{
    DES des; // Assuming DES class has a default constructor
    string key = "AABB09182736CCDD";
    des.keyGeneration(key);
    string inputFile = "plaintext.txt";
    string encryptedFile = "encrypted.txt";
    string decryptedFile = "decrypted.txt";

    auto start = chrono::high_resolution_clock::now();
    encryptFile(inputFile, encryptedFile, des);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> encryptDuration = end - start;
    cout << "File encrypted successfully" << endl;
    cout<< encryptDuration.count() << " seconds!" << endl;

    start = chrono::high_resolution_clock::now();
    decryptFile(encryptedFile, decryptedFile, des);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> decryptDuration = end - start;
    cout << "File decrypted successfully" <<endl;
    cout<< decryptDuration.count() << " seconds!" << endl;

    return 0;
}
