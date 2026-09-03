add_library(ex22_lib STATIC ex22.cpp)
target_include_directories(ex22_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex22_lib PUBLIC ex21_lib)

add_executable(ex22 ex22_main.cpp)
target_link_libraries(ex22 PRIVATE ex22_lib)

add_executable(ex22_test ex22_test.cpp)
target_link_libraries(ex22_test PRIVATE ex22_lib GTest::gtest_main)

add_test(NAME ex22_test COMMAND ex22_test)
