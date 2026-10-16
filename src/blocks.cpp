#include "../include/eth/blocks.hpp"

namespace eth {

/*BlockHeader ->(serialize)
 * -> 92 canonical bytes ->
 * (sha-256) 32 byte block hash*/
Hash hash_block_header(const BlockHeader &header) {
  const Bytes encoded = serialize(header);
  return sha256(encoded);
}

Bytes serialize(const BlockHeader &header) {
  ByteWriter writer;

  writer.write_u32(header.version);
  writer.write_u64(header.height);
  writer.write_hash(header.previous_hash);
  writer.write_hash(header.payload_hash);
  writer.write_u64(header.timestamp);
  writer.write_u64(header.nonce);

  return writer.bytes();
}

bool has_valid_payload(const Block &block) {
  return sha256(block.payload) == block.header.payload_hash;
}

bool follows(const Block &previous, const Block &current) {
  return previous.header.height != std::numeric_limits<std::uint64_t>::max() &&
         current.header.height == previous.header.height + 1 &&
         hash_block_header(previous.header) == current.header.previous_hash;
}

bool is_valid_chain(std::span<const Block> chain, std::size_t zero_bytes) {
  if (chain.empty()) {
    return false;
  }
  /*inspect chain.front() for the genesis rules.*/
  /*use Hash{} to represent an all-zero hash */
  const Block &genesis = chain.front();
  if (genesis.header.height != 0 || genesis.header.previous_hash != Hash{}) {
    return false;
  }

  for (const Block &block : chain) {
    if (!has_valid_payload(block) ||
        !meets_difficulty(hash_block_header((block.header)), zero_bytes)) {
      return false;
    }
  }

  for (std::size_t i = 1; i < chain.size(); ++i) {
    if (!follows(chain[i - 1], chain[i])) {
      return false;
    }
  }

  return true;
}

bool meets_difficulty(const Hash &hash, std::size_t zero_bytes) {
  if (zero_bytes > hash.size()) {
    return false;
  }

  for (std::size_t i = 0; i < zero_bytes; ++i) {
    if (hash[i] != 0) {
      return false;
    }
  }

  return true;
}

[[nodiscard]] bool mine(BlockHeader &header, std::size_t zero_bytes,
                        std::uint64_t max_attempt) {
  if (zero_bytes > Hash{}.size()) {
    return false;
  }

  // repeatedly hash the header while changing its nonce until meets_difficulty
  // succeeds
  for (std::uint64_t attempt = 0; attempt < max_attempt; ++attempt) {
    // 1. Hash the current header
    const Hash candidate = hash_block_header(header);

    // 2. Return true if its hash meets the difficulty
    if (meets_difficulty(candidate, zero_bytes)) {
      return true;
    }
    // 3. Return false if the nonce is already UINT64_MAX
    if (header.nonce == UINT64_MAX) {
      return false;
    }
    // 4.. Otherwiise, increment the nonce
    ++header.nonce;
  }

  return false;
}

} // namespace eth
