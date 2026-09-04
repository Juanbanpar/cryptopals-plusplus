add_library(ex32_lib STATIC ex32.cpp)
target_include_directories(ex32_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex32_lib PUBLIC ex31_lib)

add_executable(ex32 ex32_main.cpp)
target_link_libraries(ex32 PRIVATE ex32_lib ex31_lib common)

add_executable(ex32_test ex32_test.cpp)
target_link_libraries(ex32_test PRIVATE ex32_lib ex31_lib common GTest::gtest_main)

add_test(NAME ex32_test COMMAND ex32_test)
