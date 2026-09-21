#include "../include/eth/hash.hpp"
#include <openssl/evp.h>
#include <stdexcept>

namespace eth {

Hash sha256(const ByteView input) {
    Hash digest {};
    std::size_t digest_length = 0;

/*
It hashes datalen bytes of data at data using the digest algorithm name, which is fetched using the optional libctx and propq parameters.  The di‐
         gest value is placed in md and its length is written at mdlen if the pointer is not NULL. At most EVP_MAX_MD_SIZE bytes will be written.
*/
    const int result = EVP_Q_digest(
        nullptr, 
        "SHA256",
        nullptr,
        input.data(),
        input.size(),
        digest.data(),
        &digest_length
    );

    if (result != 1 || digest_length != digest.size()) {
        throw std::runtime_error("SHA-256 calculation failed");
    }

    return digest;

}

} //namespace eth
