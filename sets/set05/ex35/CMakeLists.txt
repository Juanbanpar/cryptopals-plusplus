add_library(ex35_lib STATIC ex35.cpp)
target_include_directories(ex35_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex35_lib PUBLIC ex34_lib ex33_lib)

add_executable(ex35 ex35_main.cpp)
target_link_libraries(ex35 PRIVATE ex35_lib)

add_executable(ex35_test ex35_test.cpp)
target_link_libraries(ex35_test PRIVATE ex35_lib GTest::gtest_main)

add_test(NAME ex35_test COMMAND ex35_test)
