#include "ex32.h"

namespace set04::ex32
{

    std::vector<unsigned char> recover_hmac_sha1(
        const std::string &file,
        set04::ex31::TimingOracle &oracle,
        size_t samples_per_candidate
    )
    {
        return set04::ex31::recover_hmac_sha1(
            file,
            [&oracle](const auto &name, const auto &signature)
            { return oracle.measure(name, signature); },
            20,
            samples_per_candidate
        );
    }

} // namespace set04::ex32
