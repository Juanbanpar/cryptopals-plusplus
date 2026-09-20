add_library(ex39_lib STATIC ex39.cpp)
target_include_directories(ex39_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex39_lib PUBLIC bignum)

add_executable(ex39 ex39_main.cpp)
target_link_libraries(ex39 PRIVATE ex39_lib)

add_executable(ex39_test ex39_test.cpp)
target_link_libraries(ex39_test PRIVATE ex39_lib GTest::gtest_main)

add_test(NAME ex39_test COMMAND ex39_test)
