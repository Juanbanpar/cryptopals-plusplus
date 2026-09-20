add_library(ex40_lib STATIC ex40.cpp)
target_include_directories(ex40_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex40_lib PUBLIC ex39_lib bignum)

add_executable(ex40 ex40_main.cpp)
target_link_libraries(ex40 PRIVATE ex40_lib)

add_executable(ex40_test ex40_test.cpp)
target_link_libraries(ex40_test PRIVATE ex40_lib GTest::gtest_main)

add_test(NAME ex40_test COMMAND ex40_test)
