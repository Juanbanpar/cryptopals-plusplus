add_library(ex33_lib STATIC ex33.cpp)
target_include_directories(ex33_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex33_lib PUBLIC bignum)

add_executable(ex33 ex33_main.cpp)
target_link_libraries(ex33 PRIVATE ex33_lib)

add_executable(ex33_test ex33_test.cpp)
target_link_libraries(ex33_test PRIVATE ex33_lib GTest::gtest_main)

add_test(NAME ex33_test COMMAND ex33_test)
