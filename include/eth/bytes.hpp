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
#include <array>
#include <limits>
#include <stdexcept>
#include <span>


struct BlockHeader {
    std::uint32_t version;
    std::uint64_t height;
    std::array<std::uint8_t, 32> previous_hash;
    std::uint64_t timestamp;
    std::uint64_t nonce;
};

namespace eth {
    using Bytes = std::vector<std::uint8_t>;
    using Hash = std::array<std::uint8_t, 32>;
    using ByteView = std::span<const std::uint8_t>;

    class ByteWriter {
    public:  
        void write_u32(std::uint32_t value);
        void write_u64(std::uint64_t value);
        void write_fixed(ByteView value);
        void write_sized(ByteView value);
        void write_hash(const Hash& hash);

        /*warn when values are discarded*/
        [[nodiscard]] const Bytes& bytes() const noexcept;
    private:
        Bytes bytes_;

    };
    
    
}

