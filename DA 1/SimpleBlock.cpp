#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class SimpleIDEABlockCipher
{
public:
    static constexpr size_t BLOCK_SIZE_WORDS = 4;
    static constexpr size_t KEY_SIZE_WORDS = 8;

    SimpleIDEABlockCipher(const vector<uint16_t> &key)
    {
        if (key.size() != KEY_SIZE_WORDS)
        {
            throw invalid_argument("Invalid key size");
        }
        this->roundKeys = generateRoundKeys(key);
    }

    vector<uint16_t> encrypt(const vector<uint16_t> &plaintext)
    {
        if (plaintext.size() != BLOCK_SIZE_WORDS)
        {
            throw invalid_argument("Invalid plaintext size");
        }

        vector<uint16_t> state = plaintext;

        for (size_t round = 0; round < 3; ++round)
        {
            state[0] = mul(state[0], roundKeys[4 * round]);
            state[1] = add(state[1], roundKeys[4 * round + 1]);
            state[2] = add(state[2], roundKeys[4 * round + 2]);
            state[3] = mul(state[3], roundKeys[4 * round + 3]);

            state[0] = add(state[0], state[2]);
            state[1] = mul(state[1], state[3]);

            state[2] = add(state[0], state[2]);
            state[3] = mul(state[1], state[3]);

            rotate(state.begin(), state.begin() + 1, state.end());
        }

        state[0] = mul(state[0], roundKeys[12]);
        state[1] = add(state[1], roundKeys[13]);
        state[2] = add(state[2], roundKeys[14]);
        state[3] = mul(state[3], roundKeys[15]);

        return state;
    }

    vector<uint16_t> decrypt(const vector<uint16_t> &ciphertext)
    {
        if (ciphertext.size() != BLOCK_SIZE_WORDS)
        {
            throw invalid_argument("Invalid ciphertext size");
        }

        vector<uint16_t> state = ciphertext;

        state[3] = mul_inv(state[3], roundKeys[15]);
        state[2] = sub(state[2], roundKeys[14]);
        state[1] = sub(state[1], roundKeys[13]);
        state[0] = mul_inv(state[0], roundKeys[12]);

        for (int round = 2; round >= 0; --round)
        {
            rotate(state.rbegin(), state.rbegin() + 1, state.rend());

            state[3] = mul_inv(state[3], state[1]);
            state[2] = sub(state[2], state[0]);
            state[1] = mul_inv(state[1], state[3]);
            state[0] = sub(state[0], state[2]);

            state[3] = mul_inv(state[3], roundKeys[4 * round + 3]);
            state[2] = sub(state[2], roundKeys[4 * round + 2]);
            state[1] = sub(state[1], roundKeys[4 * round + 1]);
            state[0] = mul(state[0], roundKeys[4 * round]);
        }

        return state;
    }

private:
    vector<uint16_t> roundKeys;

    uint16_t add(uint16_t a, uint16_t b)
    {
        return a + b;
    }

    uint16_t sub(uint16_t a, uint16_t b)
    {
        return a - b;
    }

    uint16_t mul(uint16_t a, uint16_t b)
    {
        return (uint16_t)(static_cast<int32_t>(a) * static_cast<int32_t>(b)) & 0xFFFF;
    }

    uint16_t mul_inv(uint16_t a, uint16_t m)
    {
        if (a == 0)
        {
            return 0;
        }
        for (uint16_t i = 1; i < 0xFFFF; ++i)
        {
            if ((mul(a, i) & 0xFFFF) == 1)
            {
                return i;
            }
        }
        return 0;
    }

    vector<uint16_t> generateRoundKeys(const vector<uint16_t> &key)
    {
        vector<uint16_t> roundKeys(KEY_SIZE_WORDS * 4);
        for (size_t i = 0; i < KEY_SIZE_WORDS; ++i)
        {
            roundKeys[i] = key[i];
        }
        for (size_t i = KEY_SIZE_WORDS; i < KEY_SIZE_WORDS * 4; ++i)
        {
            roundKeys[i] = roundKeys[i - KEY_SIZE_WORDS];
        }
        return roundKeys;
    }
};

int main()
{
    vector<uint16_t> key = {0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x0123, 0x4567, 0x89AB, 0xCDEF};
    SimpleIDEABlockCipher cipher(key);

    vector<uint16_t> plaintext = {0x1111, 0x2222, 0x3333, 0x4444};

    vector<uint16_t> ciphertext = cipher.encrypt(plaintext);
    cout << "Ciphertext: ";
    for (uint16_t word : ciphertext)
    {
        cout << hex << setw(4) << setfill('0') << word << " ";
    }
    cout << endl;

    vector<uint16_t> decrypted = cipher.decrypt(ciphertext);
    cout << "Decrypted: ";
    for (uint16_t word : decrypted)
    {
        cout << hex << setw(4) << setfill('0') << word << " ";
    }
    cout << endl;

    return 0;
}
