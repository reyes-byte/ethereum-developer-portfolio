#include "../include/eth/bytes.hpp"


namespace eth {

/* extract each byte from left to right */
void ByteWriter::write_u32(std::uint32_t value) {
    for (int shift = 24; shift >= 0; shift -= 8){
        bytes_.push_back(static_cast<std::uint8_t>(value >> shift) & 0xffu);
    }
}

void ByteWriter::write_u64(std::uint64_t value) {
    for (int shift = 56; shift >= 0; shift -= 8){
        bytes_.push_back(static_cast<std::uint8_t>(value >> shift) & 0xffu);
    }
}

void ByteWriter::write_fixed(ByteView value) {
    bytes_.insert(bytes_.end(), value.begin(), value.end());
}

[[nodiscard]] const Bytes& ByteWriter::bytes() const noexcept {
        return bytes_;
    }

/* variable-length data needs a length prefix */
void ByteWriter::write_sized(ByteView value){

    if (value.size() > std::numeric_limits<std::uint32_t>::max()) {
       throw std::length_error("byte field exceeds the 32-bit length limit");
    }
    write_u32(static_cast<std::uint32_t>(value.size()));
    write_fixed(value);
}

/* appends an already calculated 32-byte hash to the canonical byte sequence */
void ByteWriter::write_hash(const Hash& hash){
    bytes_.insert(bytes_.end(), hash.begin(), hash.end());
}

}




