#include "eth/crypto.hpp"
#include "eth/transaction.hpp"

#include <openssl/core_names.h>
#include <openssl/evp.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string_view>

namespace {

void test_generate_secp256k1_key_pair() {
    eth::Key key = eth::generate_key();
    assert(key != nullptr);
    assert(EVP_PKEY_is_a(key.get(), "EC") == 1);

    char group_name[32]{};
    std::size_t group_name_length = 0;
    assert(EVP_PKEY_get_utf8_string_param(key.get(), OSSL_PKEY_PARAM_GROUP_NAME,
                                          group_name, sizeof(group_name),
                                          &group_name_length) == 1);
    assert(std::string_view(group_name, group_name_length) == "secp256k1");

    using Context = std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>;
    Context context{EVP_PKEY_CTX_new_from_pkey(nullptr, key.get(), nullptr),
                    &EVP_PKEY_CTX_free};
    assert(context != nullptr);
    assert(EVP_PKEY_pairwise_check(context.get()) == 1);
}

void test_signing_bytes_layout() {
    eth::UnsignedTransaction tx{};
    tx.sender_public_key = {0x02, 0xCA, 0xFE};
    for (std::size_t i = 0; i < tx.recipient.size(); ++i) {
        tx.recipient[i] = static_cast<std::uint8_t>(0xA0 + i);
    }
    tx.amount = 0x0102030405060708ULL;
    tx.nonce = 0x1122334455667788ULL;

    const eth::Bytes expected{
        0x00, 0x00, 0x00, 0x01, // format version
        0x00, 0x00, 0x00, 0x03, // sender-key length
        0x02, 0xCA, 0xFE,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
        0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
        0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, // amount
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88  // nonce
    };
    assert(eth::signing_bytes(tx) == expected);
}

void test_signing_bytes_empty_sender() {
    const eth::UnsignedTransaction tx{};
    eth::Bytes expected(56, 0);
    expected[3] = 1; // version; empty sender still has a four-byte length.

    assert(eth::signing_bytes(tx) == expected);
}

} // namespace

int main() {
    test_generate_secp256k1_key_pair();
    test_signing_bytes_layout();
    test_signing_bytes_empty_sender();
    std::cout << "All crypto and transaction tests passed\n";
}
