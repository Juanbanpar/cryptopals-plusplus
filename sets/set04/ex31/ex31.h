#pragma once

#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace set04::ex31
{

    using Timing = std::chrono::microseconds;
    using TimingProbe = std::function<Timing(
        const std::string &file,
        const std::vector<unsigned char> &signature
    )>;

    // Compute HMAC-SHA1 using the standard 64-byte compression block.
    std::vector<unsigned char> hmac_sha1(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    );

    // Compare a signature byte by byte, returning at the first mismatch.
    bool insecure_compare(
        const std::vector<unsigned char> &expected,
        const std::vector<unsigned char> &candidate
    );

    class TimingOracle
    {
    public:
        TimingOracle(
            const std::vector<unsigned char> &key,
            Timing delay_per_byte = Timing(50000),
            Timing jitter = Timing(0)
        );

        bool verify(
            const std::string &file,
            const std::vector<unsigned char> &signature
        ) const;

        Timing measure(
            const std::string &file,
            const std::vector<unsigned char> &signature
        ) const;

    private:
        std::vector<unsigned char> key_;
        Timing delay_per_byte_;
        Timing jitter_;
        mutable uint64_t query_count_;
    };

    // Recover a tag one byte at a time from a timing probe.
    std::vector<unsigned char> recover_hmac_sha1(
        const std::string &file,
        const TimingProbe &probe,
        size_t digest_size = 20,
        size_t samples_per_candidate = 1
    );

} // namespace set04::ex31
