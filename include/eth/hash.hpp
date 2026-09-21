#pragma once

#include "bytes.hpp"

#include <array>
#include <cstdint>

namespace eth {

using Hash = std::array<std::uint8_t, 32>;


[[nodiscard]] Hash sha256(const ByteView input);
}
