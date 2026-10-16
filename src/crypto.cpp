#include "../include/eth/crypto.hpp"

#include <openssl/ec.h>
#include <openssl/x509.h>
#include <stdexcept>

namespace eth {

Key generate_key() {
  Key key{EVP_EC_gen("secp256k1"), &EVP_PKEY_free};

  if (!key) {
    throw std::runtime_error("EC key generation failed");
  }
  return key;
}

Bytes public_key_bytes(const Key &key) {
  if (!key) {
    throw std::runtime_error("Key is null");
  }

  // query required buffer length
  const int length = i2d_PUBKEY(key.get(), nullptr);
  if (length <= 0) {
    throw std::runtime_error("length is less than zero");
  }
}

} // namespace eth
