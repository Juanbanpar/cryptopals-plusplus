add_library(ex31_lib STATIC ex31.cpp)
target_include_directories(ex31_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex31_lib PUBLIC common sha1)

add_executable(ex31 ex31_main.cpp)
target_link_libraries(ex31 PRIVATE ex31_lib common)

add_executable(ex31_test ex31_test.cpp)
target_link_libraries(ex31_test PRIVATE ex31_lib common GTest::gtest_main)

add_test(NAME ex31_test COMMAND ex31_test)
