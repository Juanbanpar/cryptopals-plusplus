add_library(ex47_lib STATIC ex47.cpp)
target_include_directories(ex47_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex47_lib PUBLIC ex39_lib bignum)

add_executable(ex47 ex47_main.cpp)
target_link_libraries(ex47 PRIVATE ex47_lib)

add_executable(ex47_test ex47_test.cpp)
target_link_libraries(ex47_test PRIVATE ex47_lib GTest::gtest_main)

add_test(NAME ex47_test COMMAND ex47_test)
