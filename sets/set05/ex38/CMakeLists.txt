add_library(ex38_lib STATIC ex38.cpp)
target_include_directories(ex38_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex38_lib PUBLIC ex36_lib)

add_executable(ex38 ex38_main.cpp)
target_link_libraries(ex38 PRIVATE ex38_lib)

add_executable(ex38_test ex38_test.cpp)
target_link_libraries(ex38_test PRIVATE ex38_lib GTest::gtest_main)

add_test(NAME ex38_test COMMAND ex38_test)
