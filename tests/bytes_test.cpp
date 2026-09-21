#include "../include/eth/bytes.hpp"

#include <cassert>
#include <iostream>


int main() {
    eth::Bytes actual;

    eth::write_u32(actual, 0xA1B2C3D4);

    const eth::Bytes expected{
        0xA1, 0xB2, 0xC3, 0xD4
    };

    assert(actual == expected);
    std::cout << "write u_32 test passed\n";
    
}
