add_library(ex17_lib STATIC ex17.cpp)
target_include_directories(ex17_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex17_lib PUBLIC common aes pkcs7)

add_executable(ex17 ex17_main.cpp)
target_link_libraries(ex17 PRIVATE ex17_lib)

add_executable(ex17_test ex17_test.cpp)
target_link_libraries(ex17_test PRIVATE ex17_lib GTest::gtest_main)

add_test(NAME ex17_test COMMAND ex17_test)
