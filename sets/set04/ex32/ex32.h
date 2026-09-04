#pragma once

#include "../ex31/ex31.h"
#include <string>
#include <vector>

namespace set04::ex32
{

    // Recover an HMAC with a smaller timing leak by averaging samples.
    std::vector<unsigned char> recover_hmac_sha1(
        const std::string &file,
        set04::ex31::TimingOracle &oracle,
        size_t samples_per_candidate = 5
    );

} // namespace set04::ex32
