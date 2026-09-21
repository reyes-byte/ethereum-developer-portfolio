#include "../include/eth/blocks.hpp"


namespace eth {

/*BlockHeader ->(serialize) 
 * -> 60 canonical bytes -> 
 * (sha-256) 32 byte block hash*/
Hash hash_block_header(const BlockHeader& header) {
    const Bytes encoded = serialize(header);
    return sha256(encoded);
}
}
