add_library(ex14_lib STATIC ex14.cpp)
target_include_directories(ex14_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex14_lib PUBLIC common aes pkcs7)

add_executable(ex14 ex14_main.cpp)
target_link_libraries(ex14 PRIVATE ex14_lib)

add_executable(ex14_test ex14_test.cpp)
target_link_libraries(ex14_test PRIVATE ex14_lib GTest::gtest_main)

add_test(NAME ex14_test COMMAND ex14_test)
