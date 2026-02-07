#include "ex04.h"

#include <fstream>
#include <stdexcept>
#include <cctype>

namespace set01::ex04
{

    std::tuple<std::string, unsigned char, std::string> detect_single_char_xor(const std::string &path)
    {
        std::ifstream in(path);
        if (!in)
            throw std::runtime_error("could not open file: " + path);
        std::string line;
        double best_score = -1e300;
        std::string best_plain;
        unsigned char best_key = 0;
        std::string best_hex;
        while (std::getline(in, line))
        {
            if (line.empty())
                continue;
            auto [key, plain] = ::crack_single_byte_xor_hex(line);
            double sc = ::score_english(plain);
            if (sc > best_score)
            {
                best_score = sc;
                // trim trailing CR/LF from candidate plaintext
                while (!plain.empty() && (plain.back() == '\n' || plain.back() == '\r'))
                    plain.pop_back();
                best_plain = plain;
                best_key = key;
                best_hex = line;
            }
        }
        return {best_plain, best_key, best_hex};
    }

} // namespace set01::ex04
