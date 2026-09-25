add_library(ex42_lib STATIC ex42.cpp)
target_include_directories(ex42_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex42_lib PUBLIC ex39_lib bignum sha1)

add_executable(ex42 ex42_main.cpp)
target_link_libraries(ex42 PRIVATE ex42_lib)

add_executable(ex42_test ex42_test.cpp)
target_link_libraries(ex42_test PRIVATE ex42_lib GTest::gtest_main)

add_test(NAME ex42_test COMMAND ex42_test)
