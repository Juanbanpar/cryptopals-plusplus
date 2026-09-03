add_library(ex21_lib STATIC ex21.cpp)
target_include_directories(ex21_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(ex21 ex21_main.cpp)
target_link_libraries(ex21 PRIVATE ex21_lib)

add_executable(ex21_test ex21_test.cpp)
target_link_libraries(ex21_test PRIVATE ex21_lib GTest::gtest_main)

add_test(NAME ex21_test COMMAND ex21_test)
