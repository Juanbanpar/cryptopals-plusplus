#include "common.h"
#include "ex31.h"
#include <iostream>

int main()
{
    const std::string file = "foo";
    const auto key = rand_bytes(16);
    set04::ex31::TimingOracle oracle(key, set04::ex31::Timing(50000));
    const auto recovered = set04::ex31::recover_hmac_sha1(
        file,
        [&oracle](const auto &name, const auto &signature)
        { return oracle.measure(name, signature); }
    );

    std::cout << "Recovered HMAC: " << bytes_to_hex(recovered) << '\n'
              << "Verification: " << (oracle.verify(file, recovered) ? "SUCCESS" : "FAILURE")
              << std::endl;
    return oracle.verify(file, recovered) ? 0 : 1;
}
