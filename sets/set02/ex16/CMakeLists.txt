add_library(ex16_lib STATIC ex16.cpp)
target_include_directories(ex16_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex16_lib PUBLIC common aes pkcs7)

add_executable(ex16 ex16_main.cpp)
target_link_libraries(ex16 PRIVATE ex16_lib)

add_executable(ex16_test ex16_test.cpp)
target_link_libraries(ex16_test PRIVATE ex16_lib GTest::gtest_main)

add_test(NAME ex16_test COMMAND ex16_test)
