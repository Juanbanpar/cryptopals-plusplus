add_library(ex30_lib STATIC ex30.cpp)
target_include_directories(ex30_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex30_lib PUBLIC md4)

add_executable(ex30 ex30_main.cpp)
target_link_libraries(ex30 PRIVATE ex30_lib common)

add_executable(ex30_test ex30_test.cpp)
target_link_libraries(ex30_test PRIVATE ex30_lib common GTest::gtest_main)

add_test(NAME ex30_test COMMAND ex30_test)
