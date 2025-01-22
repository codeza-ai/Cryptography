#include <iostream>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

class SimpleBlockCipher
{
private:
    string masterKey;
    vector<string> keys;

    // XOR two binary strings
    string xorKeys(const string &key1, const string &key2)
    {
        string newKey = "";
        for (int i = 0; i < 8; i++)
        {
            newKey += (key1[i] == key2[i]) ? "0" : "1";
        }
        return newKey;
    }

    // Add two binary strings
    string addKeys(const string &key1, const string &key2)
    {
        string newKey = "";
        bool carry = false;
        for (int i = 7; i >= 0; i--)
        {
            bool bit1 = (key1[i] == '1');
            bool bit2 = (key2[i] == '1');
            bool sum = bit1 ^ bit2 ^ carry;
            carry = (bit1 && bit2) || (carry && (bit1 ^ bit2));
            newKey = (sum ? "1" : "0") + newKey;
        }
        return newKey;
    }

    // Generate round keys
    void generateKeys()
    {
        string xorKey = masterKey.substr(0, 8);
        string addKey = masterKey.substr(8, 8);

        keys.clear();
        for (int i = 0; i < 8; i++)
        {
            string temp = bitset<8>(i).to_string(); // Use iteration number as input
            string newKey = xorKeys(temp, xorKey);
            newKey = addKeys(newKey, addKey);
            keys.push_back(newKey);
        }
    }

public:
    SimpleBlockCipher(string key)
    {
        if (key.length() != 16)
        {
            throw invalid_argument("Key must be exactly 16 bits long.");
        }
        masterKey = key;
        generateKeys();
    }

    // Encrypt plaintext
    string encrypt(string plaintext)
    {
        string ciphertext = "";
        int j = 0;
        for (size_t i = 0; i < plaintext.length(); i += 8)
        {
            string block = plaintext.substr(i, 8);
            if (block.length() < 8)
            {
                block.append(8 - block.length(), '0'); // Pad with zeros if needed
            }
            block = xorKeys(block, keys[j % keys.size()]);
            ciphertext += block;
            j++;
        }
        return ciphertext;
    }

    // Decrypt ciphertext
    string decrypt(string ciphertext)
    {
        string decrypted = "";
        int j = 0;
        for (size_t i = 0; i < ciphertext.length(); i += 8)
        {
            string block = ciphertext.substr(i, 8);
            block = xorKeys(block, keys[j % keys.size()]);
            decrypted += block;
            j++;
        }
        return decrypted;
    }
};

int main()
{
    string key;
    cout << "Enter the key (16 bits long): ";
    cin >> key;

    try
    {
        SimpleBlockCipher cipher(key);

        string plaintext;
        cout << "Enter the plaintext (6 characters): ";
        cin >> plaintext;

        // Convert plaintext to binary (48 bits)
        string binaryPlaintext = "";
        for (char c : plaintext)
        {
            binaryPlaintext += bitset<8>(c).to_string();
        }

        string ciphertext = cipher.encrypt(binaryPlaintext);
        cout << "Ciphertext: " << ciphertext << endl;

        string decryptedBinary = cipher.decrypt(ciphertext);

        // Convert decrypted binary back to text
        string decryptedText = "";
        for (size_t i = 0; i < decryptedBinary.length(); i += 8)
        {
            bitset<8> charBits(decryptedBinary.substr(i, 8));
            decryptedText += static_cast<char>(charBits.to_ulong());
        }

        cout << "Decrypted: " << decryptedText << endl;
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
