add_library(ex08_lib STATIC ex08.cpp)
target_include_directories(ex08_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex08_lib PUBLIC common)

add_executable(ex08 ex08_main.cpp)
target_link_libraries(ex08 PRIVATE ex08_lib)
target_compile_definitions(ex08 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex08_test ex08_test.cpp)
target_link_libraries(ex08_test PRIVATE ex08_lib GTest::gtest_main)
target_compile_definitions(ex08_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex08_test COMMAND ex08_test)
