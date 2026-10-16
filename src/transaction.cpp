#include "../include/eth/transaction.hpp"
#include "../include/eth/bytes.hpp"

namespace eth {

Bytes signing_bytes(const UnsignedTransaction &tx) {
  ByteWriter writer;

  writer.write_u32(1);
  writer.write_sized(tx.sender_public_key);
  writer.write_hash(tx.recipient);
  writer.write_u64(tx.amount);
  writer.write_u64(tx.nonce);

  return writer.bytes();
}

} // namespace eth
