add_library(ex03_lib STATIC ex03.cpp)
target_include_directories(ex03_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex03_lib PUBLIC common)

add_executable(ex03 ex03_main.cpp)
target_link_libraries(ex03 PRIVATE ex03_lib)

add_executable(ex03_test ex03_test.cpp)
target_link_libraries(ex03_test PRIVATE ex03_lib GTest::gtest_main)

add_test(NAME ex03_test COMMAND ex03_test)
