add_library(ex45_lib STATIC ex45.cpp)
target_include_directories(ex45_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex45_lib PUBLIC dsa_lib)

add_executable(ex45 ex45_main.cpp)
target_link_libraries(ex45 PRIVATE ex45_lib)

add_executable(ex45_test ex45_test.cpp)
target_link_libraries(ex45_test PRIVATE ex45_lib GTest::gtest_main)

add_test(NAME ex45_test COMMAND ex45_test)
