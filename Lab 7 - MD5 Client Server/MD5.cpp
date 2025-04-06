#include "MD5.h"

uint32_t MD5::leftRotate(uint32_t x, uint32_t c) {
    return (x << c) | (x >> (32 - c));
}

MD5::MD5() {
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
}

// Processes a 512-bit chunk
void MD5::processChunk(const uint8_t* chunk) {
    uint32_t a = h0, b = h1, c = h2, d = h3, f, g;
    uint32_t M[16];

    memcpy(M, chunk, 64);

    for (int i = 0; i < 64; i++) {
        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7 * i) % 16;
        }

        uint32_t temp = d;
        d = c;
        c = b;
        b += leftRotate(a + f + K[i] + M[g], s[i]);
        a = temp;
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
}

std::string MD5::hash(const std::string& input) {
    std::vector<uint8_t> data(input.begin(), input.end());

    uint64_t bit_len = data.size() * 8;
    data.push_back(0x80);

    while ((data.size() * 8) % 512 != 448) {
        data.push_back(0x00);
    }

    for (int i = 0; i < 8; i++) {
        data.push_back(static_cast<uint8_t>(bit_len >> (i * 8)));
    }

    for (size_t i = 0; i < data.size(); i += 64) {
        processChunk(&data[i]);
    }

    std::ostringstream result;
    result << std::hex << std::setfill('0') << std::setw(8) << h0
           << std::setw(8) << h1 << std::setw(8) << h2 << std::setw(8) << h3;

    return result.str();
}
