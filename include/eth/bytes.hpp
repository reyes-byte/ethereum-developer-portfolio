/* 
### Encoding rules

  Use rules such as:

  - Integers: fixed-width unsigned types (uint32_t, uint64_t)
  - Integer byte order: big-endian
  - Hashes: exactly 32 raw bytes, not hexadecimal text
  - Variable-length data: 4-byte length followed by the data
  - Field order: permanently fixed
  - Currency values: integer smallest units—never float or double
  - Include a format version
  - Reject values that cannot be represented
*/


#pragma once 
#include <cstdint>
#include <vector>

namespace eth {
    using Bytes = std::vector<std::uint8_t>;

    void write_u32(Bytes& output, std::uint32_t value);
}

