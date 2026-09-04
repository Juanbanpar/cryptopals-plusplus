#include "ex28.h"
#include "sha1.h"

namespace set04::ex28
{

    std::vector<unsigned char> sha1_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    )
    {
        std::vector<unsigned char> input;
        input.reserve(key.size() + message.size());
        input.insert(input.end(), key.begin(), key.end());
        input.insert(input.end(), message.begin(), message.end());
        return sha1::digest(input);
    }

    bool verify_sha1_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message,
        const std::vector<unsigned char> &mac
    )
    {
        if (mac.size() != 20)
            return false;
        return sha1_mac(key, message) == mac;
    }

} // namespace set04::ex28
