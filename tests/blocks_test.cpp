#include "../include/eth/blocks.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

namespace {

eth::Block make_block(eth::Bytes payload, std::uint64_t height,
                      const eth::Hash &previous_hash = {}) {
  eth::Block block{};
  block.payload = std::move(payload);
  block.header.version = 1;
  block.header.height = height;
  block.header.previous_hash = previous_hash;
  block.header.payload_hash = eth::sha256(block.payload);
  block.header.timestamp = 42 + height;
  return block;
}

void test_chain_structure() {
  const std::vector<eth::Block> empty;
  assert(!eth::is_valid_chain(empty, 0));

  const eth::Block genesis = make_block({'a'}, 0);
  assert(eth::is_valid_chain(std::vector<eth::Block>{genesis}, 0));

  eth::Block bad_genesis = genesis;
  bad_genesis.header.height = 1;
  assert(!eth::is_valid_chain(std::vector<eth::Block>{bad_genesis}, 0));
  bad_genesis = genesis;
  bad_genesis.header.previous_hash[0] = 1;
  assert(!eth::is_valid_chain(std::vector<eth::Block>{bad_genesis}, 0));

  eth::Block tampered_genesis = genesis;
  tampered_genesis.payload[0] = 'x';
  assert(!eth::is_valid_chain(std::vector<eth::Block>{tampered_genesis}, 0));

  const eth::Block second =
      make_block({'b'}, 1, eth::hash_block_header(genesis.header));
  const std::vector<eth::Block> valid{genesis, second};
  assert(eth::is_valid_chain(valid, 0));

  auto broken = valid;
  broken[1].payload[0] = 'x';
  assert(!eth::is_valid_chain(broken, 0));
  broken = valid;
  broken[1].header.previous_hash[0] ^= 1;
  assert(!eth::is_valid_chain(broken, 0));
  broken = valid;
  broken[1].header.height = 2;
  assert(!eth::is_valid_chain(broken, 0));
  broken = valid;
  broken[0].header.nonce++;
  assert(!eth::is_valid_chain(broken, 0));
}

void test_chain_difficulty() {
  eth::Block genesis = make_block({'a'}, 0);
  // This header starts with hash byte 0xF2; nonce 109 starts with 0x00.
  assert(eth::hash_block_header(genesis.header)[0] == 0xF2);
  assert(!eth::is_valid_chain(std::vector<eth::Block>{genesis}, 1));
  assert(!eth::is_valid_chain(std::vector<eth::Block>{genesis}, 33));

  assert(eth::mine(genesis.header, 1, 110));
  assert(genesis.header.nonce == 109);
  assert(eth::is_valid_chain(std::vector<eth::Block>{genesis}, 1));

  eth::Block second =
      make_block({'b'}, 1, eth::hash_block_header(genesis.header));
  assert(!eth::is_valid_chain(std::vector<eth::Block>{genesis, second}, 1));
  assert(eth::mine(second.header, 1, 117));
  assert(second.header.nonce == 116);
  assert(eth::is_valid_chain(std::vector<eth::Block>{genesis, second}, 1));
}

void test_mine_boundaries() {
  eth::BlockHeader header{};
  header.version = 1;
  const eth::BlockHeader original = header;

  assert(!eth::mine(header, 0, 0));
  assert(!eth::mine(header, 33, 100));
  assert(eth::serialize(header) == eth::serialize(original));

  assert(eth::mine(header, 0, 1));
  assert(header.nonce == 0);

  // For this header, nonces 0..14 fail one zero byte; nonce 15 passes.
  assert(!eth::mine(header, 1, 15));
  assert(header.nonce == 15); // Budget exhaustion leaves the next candidate.
  assert(eth::mine(header, 1, 1));
  assert(header.nonce == 15);
  assert(header.version == original.version);
  assert(header.height == original.height);
  assert(header.previous_hash == original.previous_hash);
  assert(header.payload_hash == original.payload_hash);
  assert(header.timestamp == original.timestamp);

  header = original;
  header.nonce = std::numeric_limits<std::uint64_t>::max();
  assert(!eth::mine(header, 1, 2));
  assert(header.nonce == std::numeric_limits<std::uint64_t>::max());
}

} // namespace

int main() {
  test_chain_structure();
  test_chain_difficulty();
  test_mine_boundaries();
  std::cout << "All block tests passed\n";
}
