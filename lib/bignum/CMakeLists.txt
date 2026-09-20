add_library(bignum STATIC bignum.cpp)
target_include_directories(bignum PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
