add_library(ex04_lib STATIC ex04.cpp)
target_include_directories(ex04_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex04_lib PUBLIC ex03_lib)

add_executable(ex04 ex04_main.cpp)
target_link_libraries(ex04 PRIVATE ex04_lib)
target_compile_definitions(ex04 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex04_test ex04_test.cpp)
target_link_libraries(ex04_test PRIVATE ex04_lib GTest::gtest_main)
target_compile_definitions(ex04_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex04_test COMMAND ex04_test)
