#pragma once

#include <string>
#include <vector>

std::string bytes_to_base64(const std::vector<unsigned char>& data);
std::vector<unsigned char> base64_to_bytes(const std::string& b64);
