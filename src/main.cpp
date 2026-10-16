#include "../include/eth/blocks.hpp"      // Block, Hash, sha256, mine
#include "../include/eth/crypto.hpp"      // generate_key
#include "../include/eth/transaction.hpp" //signing_bytes
#include <cstdlib>                        // EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>                       // std::cout, std::cerr
#include <stdexcept>
#include <string>
#include <vector>

/*
Each byte becomes two hexadecimal characters: one for its upper four bits and
one for its lower four bits. This is only for display; continue hashing raw
bytes.
*/
std::string to_hex(eth::ByteView bytes) {
  constexpr char digits[] = "0123456789abcdef";
  std::string result;

  for (std::uint8_t byte : bytes) {
    result.push_back(digits[byte >> 4]);
    result.push_back(digits[byte & 0x0f]);
  }

  return result;
}
/*compute the payload hash, set the link, mine, then append */
int main() {
  eth::Block genesis{
      .header =
          {
              .version = 1,
              .height = 0,
              .previous_hash = eth::Hash{},
              .payload_hash = eth::Hash{},
              .timestamp = 0,
              .nonce = 0,

          },
      .payload = {0x47, 0x45, 0x4E}, // ASCII "GEN" .
  };

  genesis.header.payload_hash = eth::sha256(genesis.payload);

  constexpr std::size_t difficulty = 1;

  if (!eth::mine(genesis.header, difficulty, 100'000)) {
    throw std::runtime_error("Genesis mining exhausted its budget");
  }

  /*after mining succeeds , start my chain: */
  std::vector<eth::Block> chain;
  chain.push_back(genesis);

  eth::Block second{
      .header =
          {
              .version = 1,
              .height = 1,
              .previous_hash = eth::Hash{},
              .payload_hash = eth::Hash{},
              .timestamp = 0,
              .nonce = 0,

          },
      .payload = {0x48, 0x49}, // ASCII "GEN" .
  };

  second.header.payload_hash = eth::sha256(second.payload);
  second.header.previous_hash = eth::hash_block_header(chain.back().header);

  if (!eth::mine(second.header, difficulty, 100'000)) {
    throw std::runtime_error("Second block mining exhausted its budget");
  }

  chain.push_back(second);

  if (!eth::is_valid_chain(chain, difficulty)) {
    throw std::runtime_error("Chain validation failed");
  }
  std::cout << "Validated " << chain.size() << " blocks\n";
  auto tampered = chain;
  tampered[1].payload[0] ^= 0x01;

  if (eth::is_valid_chain(tampered, difficulty)) {
    throw std::runtime_error("Tampered chain was accepted");
  }

  std::cout << "Tampered chain rejected\n";

  for (const auto &block : chain) {
    std::cout << "Block " << block.header.height << ", nonce "
              << block.header.nonce << '\n'
              << "  hash:     " << to_hex(eth::hash_block_header(block.header))
              << '\n'
              << "  previous: " << to_hex(block.header.previous_hash) << '\n';
  }

  /*These keys will let Alice sign transfers and Bob receive them. Next we’ll
   * export their public keys into byte representations*/
  auto alice_key = eth::generate_key();
  auto bob_key = eth::generate_key();
}
