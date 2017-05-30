#include "MD5.h"
#include <iostream>
#include <iomanip>


MD5::MD5()
    : result{}
{
    this->fill_k();
}

MD5::~MD5()
{}

void MD5::fill_k()
{
    for (size_t i = 0; i < 64; i++)
        this->k[i] = static_cast<uint32_t>(0x100000000 * abs(sin(i + 1))); // Dec: 4294967296
}

void MD5::to_bytes(const uint32_t& val, uint8_t* const bytes)
{
    bytes[0] = static_cast<uint8_t>(val);
    bytes[1] = static_cast<uint8_t>(val >> 8);
    bytes[2] = static_cast<uint8_t>(val >> 16);
    bytes[3] = static_cast<uint8_t>(val >> 24);
}

uint32_t MD5::to_int32(const uint8_t* const bytes)
{
    return static_cast<uint32_t>(bytes[0]) |
        static_cast<uint32_t>(bytes[1]) << 8 |
        static_cast<uint32_t>(bytes[2]) << 16 |
        static_cast<uint32_t>(bytes[3]) << 24;
}

uint32_t MD5::shift_left(const uint32_t& x, const uint32_t& c)
{
    return (x << c) | (x >> (32 - c));
}

uint32_t MD5::f(const uint32_t& x, const uint32_t& y, const uint32_t& z) const
{
    return (x & y) | (~x & z);
}

uint32_t MD5::g(const uint32_t& x, const uint32_t& y, const uint32_t& z) const
{
    return (x & z) | (y & ~z);
}

uint32_t MD5::h(const uint32_t& x, const uint32_t& y, const uint32_t& z) const
{
    return x ^ y ^ z;
}

uint32_t MD5::i(const uint32_t& x, const uint32_t& y, const uint32_t& z) const
{
    return y ^ (x | ~z);
}

void MD5::runMD5(const std::string& init_msg)
{
    // Message length
    const size_t init_len = init_msg.size();

    // Final message - after padding
    uint8_t *msg = nullptr;

    size_t offset{};
    uint32_t w[16]{};
    uint32_t a{}, b{}, c{}, d{}, i{}, f{}, g{}, temp{};

    // h0, h1, h2, h3 - will contain MD5 hash
    // initialization by some constant values
    uint32_t h0{ 0x67452301 };
    uint32_t h1{ 0xefcdab89 };
    uint32_t h2{ 0x98badcfe };
    uint32_t h3{ 0x10325476 };

    // Add padding
    // Append '1' bit to message
    // Append '0' until new_len equals 448 (modulo 512)
    // Append 64 bit length

    size_t new_len = init_len + 1;
    while ((new_len & 63) != 56)
        ++new_len; // Equal to while (new_len % (512 / 8) != 448 / 8)

    msg = new uint8_t[new_len + 8];

    for (int i = 0; i < init_len; i++)
        msg[i] = init_msg[i];

    msg[init_len] = 0x80; // 0x80 = 10000000

    for (offset = init_len + 1; offset < new_len; ++offset)
        msg[offset] = 0;


    // Append lower 32bits of message length
    to_bytes(static_cast<uint32_t>(init_len << 3), msg + new_len);
    // Append higher 32bits of message length
    to_bytes(static_cast<uint32_t>(init_len >> 29), msg + new_len + 4);

    // Proccess the message in 512 bits blocks
    for (offset = 0; offset < new_len; offset += (512 / 8))
    {
        // split it into 16 32-bits words
        for (i = 0; i < 16; ++i)
            w[i] = to_int32(msg + offset + i * 4);

        // Initialize temp buffers
        a = h0;
        b = h1;
        c = h2;
        d = h3;

        for (i = 0; i < 64; i++)
        {
            if (i < 16)
            {
                f = this->f(b, c, d);
                g = i;
            }
            else if (i < 32)
            {
                f = this->g(b, c, d);
                g = (5 * i + 1) % 16;
            }
            else if (i < 48)
            {
                f = this->h(b, c, d);
                g = (3 * i + 5) % 16;
            }
            else
            {
                f = this->i(b, c, d);
                g = (7 * i) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + this->shift_left((a + f + k[i] + w[g]), r[i]);
            a = temp;

        }

        // Add this round values to so far's result
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }

    // Deallocate memory
    delete[] msg;

    // Write final MD5 result to result array
    to_bytes(h0, result);
    to_bytes(h1, result + 4);
    to_bytes(h2, result + 8);
    to_bytes(h3, result + 12);

    this->printMD5();
}

void MD5::printMD5() const
{
    std::cout << std::endl << "MD5: ";
    for (const int &i : result)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << i;
    std::cout << std::endl;
}