#pragma once

#include "bytes.hpp"
#include <cstdint>

namespace eth {

/* Signatures, balances, and the mempool all depend on this format
 * This byte sequence will become the input to ECDSA signing
 * It must exclude the signature itself
 * */
struct UnsignedTransaction {
  Bytes sender_public_key{};
  Hash recipient{};
  std::uint64_t amount{};
  std::uint64_t nonce{};
};

/* IMPLEMENTATION DETAILS
 * transaction holds the transfer details
 * signature holds the ECDSA signature bytes
 */
struct SignedTransaction {
  UnsignedTransaction transaction{};
  Bytes signature{};
};

[[nodiscard]] Bytes signing_bytes(const UnsignedTransaction &tx);

} // namespace eth
