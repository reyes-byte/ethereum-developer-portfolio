#pragma once

#include "bytes.hpp"
#include "hash.hpp"

#include <cstdint>
#include <limits>
#include <span>

namespace eth {

    struct BlockHeader {
        std::uint32_t version;
        std::uint64_t height;
        Hash previous_hash;
        Hash payload_hash;
        std::uint64_t timestamp;
        std::uint64_t nonce;
    };

    struct Block {
        BlockHeader header;
        Bytes payload;
    };

    [[nodiscard]] Bytes serialize(const BlockHeader& header);
    [[nodiscard]] Hash hash_block_header(const BlockHeader& header);
    [[nodiscard]] bool has_valid_payload(const Block& block);
    [[nodiscard]] bool follows(const Block& previous, const Block& current);
    [[nodiscard]] bool is_valid_chain(std::span<const Block> chain);
    [[nodiscard]] bool meets_difficulty(const Hash& hash, std::size_t zero_bytes);

}
