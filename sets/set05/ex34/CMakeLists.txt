add_library(ex34_lib STATIC ex34.cpp)
target_include_directories(ex34_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex34_lib PUBLIC ex33_lib common aes sha1)

add_executable(ex34 ex34_main.cpp)
target_link_libraries(ex34 PRIVATE ex34_lib)

add_executable(ex34_test ex34_test.cpp)
target_link_libraries(ex34_test PRIVATE ex34_lib GTest::gtest_main)

add_test(NAME ex34_test COMMAND ex34_test)
