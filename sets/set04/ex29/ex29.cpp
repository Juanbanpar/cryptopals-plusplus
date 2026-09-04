#include "ex29.h"
#include "sha1.h"
#include <stdexcept>

namespace set04::ex29
{

    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> forge_sha1_mac(
        const std::vector<unsigned char> &original_message,
        const std::vector<unsigned char> &original_mac,
        const std::vector<unsigned char> &append_data,
        const MacOracle &oracle,
        size_t min_key_length,
        size_t max_key_length
    )
    {
        if (original_mac.size() != 20)
            throw std::invalid_argument("SHA-1 MAC must be 20 bytes");
        if (min_key_length > max_key_length)
            throw std::invalid_argument("invalid SHA-1 key length range");

        const auto state = sha1::state_from_digest(original_mac);
        for (size_t key_length = min_key_length; key_length <= max_key_length; ++key_length)
        {
            const auto glue = sha1::glue_padding(key_length + original_message.size());
            std::vector<unsigned char> forged_message = original_message;
            forged_message.insert(forged_message.end(), glue.begin(), glue.end());
            forged_message.insert(forged_message.end(), append_data.begin(), append_data.end());

            const auto forged_mac = sha1::digest_with_state(
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

} // namespace set04::ex29
