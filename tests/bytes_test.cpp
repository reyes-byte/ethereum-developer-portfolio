#include "../include/eth/bytes.hpp"

#include <cassert>
#include <iostream>


int main() {
    eth::Bytes actual;

    eth::write_u32(actual, 0xA1B2C3D4);

    const eth::Bytes expected_32{
        0xA1, 0xB2, 0xC3, 0xD4
    };


    assert(actual == expected_32);
    actual.clear();
    std::cout << "write u_32 test passed\n";


    eth::write_u64(actual, 0x123456789ABCDEF0ULL);

    const eth::Bytes expected_64{
        0x12, 0x34, 0x56, 0x78,
        0x9A, 0xBC, 0xDE, 0xF0
    };
    assert(actual == expected_64);
    actual.clear();
    std::cout << "write u_64 test passed\n";
    
    
}
