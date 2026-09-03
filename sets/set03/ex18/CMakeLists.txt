add_library(ex18_lib STATIC ex18.cpp)
target_include_directories(ex18_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex18_lib PUBLIC common aes)

add_executable(ex18 ex18_main.cpp)
target_link_libraries(ex18 PRIVATE ex18_lib)

add_executable(ex18_test ex18_test.cpp)
target_link_libraries(ex18_test PRIVATE ex18_lib GTest::gtest_main)

add_test(NAME ex18_test COMMAND ex18_test)
