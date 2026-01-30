add_library(common STATIC common.cpp)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common PUBLIC base64)

add_executable(common_test common_test.cpp)
target_link_libraries(common_test PRIVATE common GTest::gtest_main)
add_test(NAME common_test COMMAND common_test)
