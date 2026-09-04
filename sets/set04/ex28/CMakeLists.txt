add_library(ex28_lib STATIC ex28.cpp)
target_include_directories(ex28_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex28_lib PUBLIC sha1)

add_executable(ex28 ex28_main.cpp)
target_link_libraries(ex28 PRIVATE ex28_lib common)

add_executable(ex28_test ex28_test.cpp)
target_link_libraries(ex28_test PRIVATE ex28_lib common GTest::gtest_main)

add_test(NAME ex28_test COMMAND ex28_test)
