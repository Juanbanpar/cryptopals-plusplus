add_library(ex46_lib STATIC ex46.cpp)
target_include_directories(ex46_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex46_lib PUBLIC ex39_lib bignum base64)

add_executable(ex46 ex46_main.cpp)
target_link_libraries(ex46 PRIVATE ex46_lib)

add_executable(ex46_test ex46_test.cpp)
target_link_libraries(ex46_test PRIVATE ex46_lib GTest::gtest_main)

add_test(NAME ex46_test COMMAND ex46_test)
