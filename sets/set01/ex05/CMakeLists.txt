add_library(ex05_lib STATIC ex05.cpp)
target_include_directories(ex05_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex05_lib PUBLIC common)

add_executable(ex05 ex05_main.cpp)
target_link_libraries(ex05 PRIVATE ex05_lib)

add_executable(ex05_test ex05_test.cpp)
target_link_libraries(ex05_test PRIVATE ex05_lib GTest::gtest_main)

add_test(NAME ex05_test COMMAND ex05_test)
