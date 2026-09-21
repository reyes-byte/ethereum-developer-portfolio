cmake_minimum_required(VERSION 3.20)

project(ethereum_blockchain LANGUAGES CXX)

find_package(OpenSSL REQUIRED)

add_library(eth_blockchain
  src/bytes.cpp
  src/hash.cpp
  src/blocks.cpp
)

target_include_directories(eth_blockchain PUBLIC include)
target_compile_features(eth_blockchain PUBLIC cxx_std_20)
target_link_libraries(eth_blockchain PUBLIC OpenSSL::Crypto)
