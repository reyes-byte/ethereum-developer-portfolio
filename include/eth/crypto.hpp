#pragma once

#include "bytes.hpp"

#include <memory>
#include <openssl/evp.h>

namespace eth {

using Key = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;

[[nodiscard]] Key generate_key();
[[nodiscard]] Bytes public_key_bytes(const Key &key);
} // namespace eth
