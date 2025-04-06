#include "sha512.h"

SHA512::SHA512() {
    state[0] = 0x6a09e667f3bcc908;
    state[1] = 0xbb67ae8584caa73b;
    state[2] = 0x3c6ef372fe94f82b;
    state[3] = 0xa54ff53a5f1d36f1;
    state[4] = 0x510e527fade682d1;
    state[5] = 0x9b05688c2b3e6c1f;
    state[6] = 0x1f83d9abfb41bd6b;
    state[7] = 0x5be0cd19137e2179;
}

void SHA512::padMessage(std::vector<uint8_t>& message, uint64_t& bit_length) {
    bit_length = message.size() * 8;
    message.push_back(0x80);
    while ((message.size() + 8) % 128 != 0) {
        message.push_back(0x00);
    }
    for (int i = 7; i >= 0; --i) {
        message.push_back((bit_length >> (i * 8)) & 0xFF);
    }
}

std::string SHA512::digestToHexString() {
    std::ostringstream oss;
    for (int i = 0; i < 8; i++) {
        oss << std::hex << std::setw(16) << std::setfill('0') << state[i];
    }
    return oss.str();
}

void SHA512::processBlock(const uint8_t* block) {
    uint64_t w[80];
    for (int i = 0; i < 16; i++) {
        w[i] = ((uint64_t)block[i * 8] << 56) | ((uint64_t)block[i * 8 + 1] << 48) |
               ((uint64_t)block[i * 8 + 2] << 40) | ((uint64_t)block[i * 8 + 3] << 32) |
               ((uint64_t)block[i * 8 + 4] << 24) | ((uint64_t)block[i * 8 + 5] << 16) |
               ((uint64_t)block[i * 8 + 6] << 8) | (uint64_t)block[i * 8 + 7];
    }
    // Expand message schedule...
}

std::string SHA512::compute(const std::string& data) {
    std::vector<uint8_t> message(data.begin(), data.end());
    uint64_t bit_length;
    padMessage(message, bit_length);
    for (size_t i = 0; i < message.size(); i += 128) {
        processBlock(&message[i]);
    }
    return digestToHexString();
}
