#include "base64.h"

std::string bytes_to_base64(const std::vector<unsigned char> &data)
{
    static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    size_t i = 0;
    while (i + 2 < data.size())
    {
        unsigned int val = (data[i] << 16) | (data[i + 1] << 8) | data[i + 2];
        out.push_back(table[(val >> 18) & 0x3F]);
        out.push_back(table[(val >> 12) & 0x3F]);
        out.push_back(table[(val >> 6) & 0x3F]);
        out.push_back(table[val & 0x3F]);
        i += 3;
    }
    size_t rem = data.size() - i;
    if (rem == 1)
    {
        unsigned int val = (data[i] << 16);
        out.push_back(table[(val >> 18) & 0x3F]);
        out.push_back(table[(val >> 12) & 0x3F]);
        out.push_back('=');
        out.push_back('=');
    }
    else if (rem == 2)
    {
        unsigned int val = (data[i] << 16) | (data[i + 1] << 8);
        out.push_back(table[(val >> 18) & 0x3F]);
        out.push_back(table[(val >> 12) & 0x3F]);
        out.push_back(table[(val >> 6) & 0x3F]);
        out.push_back('=');
    }
    return out;
}

std::vector<unsigned char> base64_to_bytes(const std::string &b64)
{
    static const int T[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, 0, -1, -1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};
    std::vector<int> vals;
    vals.reserve(b64.size());
    for (unsigned char c : b64)
    {
        if (c == '=')
            continue;
        int v = (c < 128) ? T[c] : -1;
        if (v >= 0)
            vals.push_back(v);
    }
    std::vector<unsigned char> out;
    size_t i = 0;
    while (i + 4 <= vals.size())
    {
        int v = (vals[i] << 18) | (vals[i + 1] << 12) | (vals[i + 2] << 6) | vals[i + 3];
        out.push_back(static_cast<unsigned char>((v >> 16) & 0xFF));
        out.push_back(static_cast<unsigned char>((v >> 8) & 0xFF));
        out.push_back(static_cast<unsigned char>(v & 0xFF));
        i += 4;
    }
    size_t rem = vals.size() - i;
    if (rem == 2)
    {
        int v = (vals[i] << 18) | (vals[i + 1] << 12);
        out.push_back(static_cast<unsigned char>((v >> 16) & 0xFF));
    }
    else if (rem == 3)
    {
        int v = (vals[i] << 18) | (vals[i + 1] << 12) | (vals[i + 2] << 6);
        out.push_back(static_cast<unsigned char>((v >> 16) & 0xFF));
        out.push_back(static_cast<unsigned char>((v >> 8) & 0xFF));
    }
    return out;
}
