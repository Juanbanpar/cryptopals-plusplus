add_library(ex23_lib STATIC ex23.cpp)
target_include_directories(ex23_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex23_lib PUBLIC ex21_lib)

add_executable(ex23 ex23_main.cpp)
target_link_libraries(ex23 PRIVATE ex23_lib)

add_executable(ex23_test ex23_test.cpp)
target_link_libraries(ex23_test PRIVATE ex23_lib GTest::gtest_main)

add_test(NAME ex23_test COMMAND ex23_test)
