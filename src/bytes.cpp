#include "../include/eth/bytes.hpp"


namespace eth {

/* extract each byte from left to right */
void write_u32(Bytes& output, std::uint32_t value) {
    for (int shift = 24; shift >= 0; shift -= 8){
        output.push_back(static_cast<std::uint8_t>(value >> shift) & 0xffu);
    }
}


}
