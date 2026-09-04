#include "ex31.h"
#include "common.h"
#include "sha1.h"
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace set04::ex31
{

    namespace
    {
        constexpr size_t SHA1_BLOCK_SIZE = 64;
        constexpr unsigned char INNER_PAD = 0x36;
        constexpr unsigned char OUTER_PAD = 0x5c;

        size_t matching_prefix_length(
            const std::vector<unsigned char> &expected,
            const std::vector<unsigned char> &candidate
        )
        {
            const size_t limit = std::min(expected.size(), candidate.size());
            size_t matched = 0;
            while (matched < limit && expected[matched] == candidate[matched])
                ++matched;
            return matched;
        }

        std::vector<unsigned char> hmac_key_block(const std::vector<unsigned char> &key)
        {
            std::vector<unsigned char> block = key;
            if (block.size() > SHA1_BLOCK_SIZE)
                block = sha1::digest(block);
            block.resize(SHA1_BLOCK_SIZE, 0);
            return block;
        }
    }

    std::vector<unsigned char> hmac_sha1(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    )
    {
        const auto key_block = hmac_key_block(key);
        std::vector<unsigned char> inner_input;
        std::vector<unsigned char> outer_input;
        inner_input.reserve(SHA1_BLOCK_SIZE + message.size());
        outer_input.reserve(SHA1_BLOCK_SIZE + 20);

        for (unsigned char byte : key_block)
            inner_input.push_back(byte ^ INNER_PAD);
        inner_input.insert(inner_input.end(), message.begin(), message.end());

        for (unsigned char byte : key_block)
            outer_input.push_back(byte ^ OUTER_PAD);
        const auto inner_digest = sha1::digest(inner_input);
        outer_input.insert(outer_input.end(), inner_digest.begin(), inner_digest.end());
        return sha1::digest(outer_input);
    }

    bool insecure_compare(
        const std::vector<unsigned char> &expected,
        const std::vector<unsigned char> &candidate
    )
    {
        if (expected.size() != candidate.size())
            return false;

        for (size_t i = 0; i < expected.size(); ++i)
        {
            if (expected[i] != candidate[i])
                return false;
        }
        return true;
    }

    TimingOracle::TimingOracle(
        const std::vector<unsigned char> &key,
        Timing delay_per_byte,
        Timing jitter
    )
        : key_(key), delay_per_byte_(delay_per_byte), jitter_(jitter), query_count_(0)
    {
        if (delay_per_byte_.count() < 0 || jitter_.count() < 0)
            throw std::invalid_argument("timing values must not be negative");
    }

    bool TimingOracle::verify(
        const std::string &file,
        const std::vector<unsigned char> &signature
    ) const
    {
        const std::vector<unsigned char> message(file.begin(), file.end());
        return insecure_compare(hmac_sha1(key_, message), signature);
    }

    Timing TimingOracle::measure(
        const std::string &file,
        const std::vector<unsigned char> &signature
    ) const
    {
        const std::vector<unsigned char> message(file.begin(), file.end());
        const auto expected = hmac_sha1(key_, message);
        const size_t matched = matching_prefix_length(expected, signature);

        int64_t noise = 0;
        if (jitter_.count() > 0)
        {
            constexpr uint64_t MULTIPLIER = 0x9e3779b97f4a7c15ULL;
            const uint64_t sample = (++query_count_) * MULTIPLIER;
            const uint64_t span = static_cast<uint64_t>(jitter_.count()) * 2 + 1;
            noise = static_cast<int64_t>(sample % span) - jitter_.count();
        }
        else
        {
            ++query_count_;
        }

        const int64_t base = static_cast<int64_t>(matched) * delay_per_byte_.count();
        return Timing(std::max<int64_t>(0, base + noise));
    }

    std::vector<unsigned char> recover_hmac_sha1(
        const std::string &file,
        const TimingProbe &probe,
        size_t digest_size,
        size_t samples_per_candidate
    )
    {
        if (!probe)
            throw std::invalid_argument("timing probe must be callable");
        if (digest_size == 0 || samples_per_candidate == 0)
            throw std::invalid_argument("timing attack sizes must be non-zero");

        std::vector<unsigned char> recovered(digest_size, 0);
        for (size_t position = 0; position < digest_size; ++position)
        {
            long long best_score = std::numeric_limits<long long>::min();
            unsigned char best_byte = 0;

            for (int candidate = 0; candidate < 256; ++candidate)
            {
                recovered[position] = static_cast<unsigned char>(candidate);
                long long score = 0;
                for (size_t sample = 0; sample < samples_per_candidate; ++sample)
                    score += probe(file, recovered).count();

                if (score > best_score)
                {
                    best_score = score;
                    best_byte = static_cast<unsigned char>(candidate);
                }
            }

            recovered[position] = best_byte;
        }

        return recovered;
    }

} // namespace set04::ex31
