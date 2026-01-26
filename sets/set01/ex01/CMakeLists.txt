add_library(ex01_lib STATIC ex01.cpp)
target_include_directories(ex01_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ex01_test ex01_test.cpp)
target_link_libraries(ex01_test PRIVATE ex01_lib GTest::gtest_main)

add_test(NAME ex01_test COMMAND ex01_test)
