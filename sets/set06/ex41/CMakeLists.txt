add_library(ex41_lib STATIC ex41.cpp)
target_include_directories(ex41_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex41_lib PUBLIC ex39_lib bignum)

add_executable(ex41 ex41_main.cpp)
target_link_libraries(ex41 PRIVATE ex41_lib)

add_executable(ex41_test ex41_test.cpp)
target_link_libraries(ex41_test PRIVATE ex41_lib GTest::gtest_main)

add_test(NAME ex41_test COMMAND ex41_test)
