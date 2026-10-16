cmake_minimum_required(VERSION 3.20)

project(ethereum_blockchain LANGUAGES CXX)

find_package(OpenSSL REQUIRED)

add_library(eth_blockchain
  src/bytes.cpp
  src/hash.cpp
  src/blocks.cpp
  src/crypto.cpp
  src/transaction.cpp
)

add_executable(eth_demo src/main.cpp)
target_link_libraries(eth_demo PRIVATE eth_blockchain)
include(CTest)
#cmake -S . -B build
#cmake --build build
#ctest --test-dir build --output-on-failure
if(BUILD_TESTING)
    add_executable(eth_tests tests/bytes_test.cpp)

    target_link_libraries(eth_tests PRIVATE eth_blockchain)

    add_test(
        NAME eth_tests
        COMMAND eth_tests
    )

    add_executable(eth_blocks_tests tests/blocks_test.cpp)
    target_link_libraries(eth_blocks_tests PRIVATE eth_blockchain)
    add_test(NAME eth_blocks_tests COMMAND eth_blocks_tests)

    add_executable(eth_crypto_transaction_tests tests/crypto_transaction_test.cpp)
    target_link_libraries(eth_crypto_transaction_tests PRIVATE eth_blockchain)
    add_test(NAME eth_crypto_transaction_tests COMMAND eth_crypto_transaction_tests)
endif()


target_include_directories(eth_blockchain PUBLIC include)
target_compile_features(eth_blockchain PUBLIC cxx_std_20)
target_link_libraries(eth_blockchain PUBLIC OpenSSL::Crypto)
