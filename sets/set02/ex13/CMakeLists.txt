add_library(ex13_lib STATIC ex13.cpp)
target_include_directories(ex13_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex13_lib PUBLIC common aes pkcs7)

add_executable(ex13 ex13_main.cpp)
target_link_libraries(ex13 PRIVATE ex13_lib)

add_executable(ex13_test ex13_test.cpp)
target_link_libraries(ex13_test PRIVATE ex13_lib GTest::gtest_main)

add_test(NAME ex13_test COMMAND ex13_test)
