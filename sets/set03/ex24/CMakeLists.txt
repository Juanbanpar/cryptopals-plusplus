add_library(ex24_lib STATIC ex24.cpp)
target_include_directories(ex24_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex24_lib PUBLIC ex21_lib common)

add_executable(ex24 ex24_main.cpp)
target_link_libraries(ex24 PRIVATE ex24_lib)

add_executable(ex24_test ex24_test.cpp)
target_link_libraries(ex24_test PRIVATE ex24_lib GTest::gtest_main)

add_test(NAME ex24_test COMMAND ex24_test)
