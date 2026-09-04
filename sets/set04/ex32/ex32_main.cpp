#include "common.h"
#include "ex31.h"
#include "ex32.h"
#include <iostream>

int main()
{
    const std::string file = "foo";
    const auto key = rand_bytes(16);
    set04::ex31::TimingOracle oracle(key, set04::ex31::Timing(5000), set04::ex31::Timing(500));
    const auto recovered = set04::ex32::recover_hmac_sha1(file, oracle);

    std::cout << "Recovered HMAC: " << bytes_to_hex(recovered) << '\n'
              << "Verification: " << (oracle.verify(file, recovered) ? "SUCCESS" : "FAILURE")
              << std::endl;
    return oracle.verify(file, recovered) ? 0 : 1;
}
