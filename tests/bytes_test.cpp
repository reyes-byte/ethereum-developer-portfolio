#include "../include/eth/bytes.hpp"

#include <cassert>
#include <iostream>

namespace {

void test_write_u32() {
    eth::ByteWriter writer;
    writer.write_u32(0xA1B2C3D4U);

    const eth::Bytes expected{0xA1, 0xB2, 0xC3, 0xD4};
    assert(writer.bytes() == expected);
}

void test_write_u64() {
    eth::ByteWriter writer;
    writer.write_u64(0x123456789ABCDEF0ULL);

    const eth::Bytes expected{
        0x12, 0x34, 0x56, 0x78,
        0x9A, 0xBC, 0xDE, 0xF0
    };
    assert(writer.bytes() == expected);
}

void test_write_fixed() {
    eth::ByteWriter writer;
    const eth::Bytes value{0xDE, 0xAD, 0xBE, 0xEF};

    writer.write_fixed(value);

    assert(writer.bytes() == value);
}

void test_write_sized() {
    eth::ByteWriter writer;
    const eth::Bytes value{0xDE, 0xAD, 0xBE, 0xEF};

    writer.write_sized(value);

    const eth::Bytes expected{
        0x00, 0x00, 0x00, 0x04,
        0xDE, 0xAD, 0xBE, 0xEF
    };
    assert(writer.bytes() == expected);
}

void test_write_empty_sized() {
    eth::ByteWriter writer;
    const eth::Bytes value;

    writer.write_sized(value);

    const eth::Bytes expected{0x00, 0x00, 0x00, 0x00};
    assert(writer.bytes() == expected);
}

void test_write_hash_appends_32_bytes() {
    eth::ByteWriter writer;
    const eth::Bytes prefix{0xA1, 0xB2};
    const eth::Hash hash{
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };

    writer.write_fixed(prefix);
    writer.write_hash(hash);

    const eth::Bytes expected{
        0xA1, 0xB2,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };
    assert(writer.bytes() == expected);
}

} // namespace

int main() {
    test_write_u32();
    test_write_u64();
    test_write_fixed();
    test_write_sized();
    test_write_empty_sized();
    test_write_hash_appends_32_bytes();

    std::cout << "All ByteWriter tests passed\n";
}
