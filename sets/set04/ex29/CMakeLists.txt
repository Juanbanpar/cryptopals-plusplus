add_library(ex29_lib STATIC ex29.cpp)
target_include_directories(ex29_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex29_lib PUBLIC ex28_lib sha1)

add_executable(ex29 ex29_main.cpp)
target_link_libraries(ex29 PRIVATE ex29_lib ex28_lib common)

add_executable(ex29_test ex29_test.cpp)
target_link_libraries(ex29_test PRIVATE ex29_lib ex28_lib common GTest::gtest_main)

add_test(NAME ex29_test COMMAND ex29_test)
