add_library(ex06_lib STATIC ex06.cpp)
target_include_directories(ex06_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex06_lib PUBLIC common)

add_executable(ex06 ex06_main.cpp)
target_link_libraries(ex06 PRIVATE ex06_lib)
target_compile_definitions(ex06 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex06_test ex06_test.cpp)
target_link_libraries(ex06_test PRIVATE ex06_lib GTest::gtest_main)
target_compile_definitions(ex06_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex06_test COMMAND ex06_test)
