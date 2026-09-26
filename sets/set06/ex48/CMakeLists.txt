add_library(ex48_lib STATIC ex48.cpp)
target_include_directories(ex48_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex48_lib PUBLIC ex47_lib ex39_lib bignum)

add_executable(ex48 ex48_main.cpp)
target_link_libraries(ex48 PRIVATE ex48_lib)

add_executable(ex48_test ex48_test.cpp)
target_link_libraries(ex48_test PRIVATE ex48_lib GTest::gtest_main)

add_test(NAME ex48_test COMMAND ex48_test)
