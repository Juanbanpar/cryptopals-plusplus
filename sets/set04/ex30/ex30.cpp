#include "ex30.h"
#include "md4.h"
#include <stdexcept>

namespace set04::ex30
{

    std::vector<unsigned char> md4_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    )
    {
        std::vector<unsigned char> input;
        input.reserve(key.size() + message.size());
        input.insert(input.end(), key.begin(), key.end());
        input.insert(input.end(), message.begin(), message.end());
        return md4::digest(input);
    }

    bool verify_md4_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message,
        const std::vector<unsigned char> &mac
    )
    {
        if (mac.size() != 16)
            return false;
        return md4_mac(key, message) == mac;
    }

    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> forge_md4_mac(
        const std::vector<unsigned char> &original_message,
        const std::vector<unsigned char> &original_mac,
        const std::vector<unsigned char> &append_data,
        const MacOracle &oracle,
        size_t min_key_length,
        size_t max_key_length
    )
    {
        if (original_mac.size() != 16)
            throw std::invalid_argument("MD4 MAC must be 16 bytes");
        if (min_key_length > max_key_length)
            throw std::invalid_argument("invalid MD4 key length range");

        const auto state = md4::state_from_digest(original_mac);
        for (size_t key_length = min_key_length; key_length <= max_key_length; ++key_length)
        {
            const auto glue = md4::glue_padding(key_length + original_message.size());
            std::vector<unsigned char> forged_message = original_message;
            forged_message.insert(forged_message.end(), glue.begin(), glue.end());
            forged_message.insert(forged_message.end(), append_data.begin(), append_data.end());

            const auto forged_mac = md4::digest_with_state(
                append_data,
                state,
                key_length + original_message.size() + glue.size()
            );
            if (oracle(forged_message, forged_mac))
                return {forged_message, forged_mac};

            if (key_length == max_key_length)
                break;
        }

        return {};
    }

} // namespace set04::ex30
