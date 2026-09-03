add_library(ex20_lib STATIC ex20.cpp)
target_include_directories(ex20_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex20_lib PUBLIC common aes)
target_compile_definitions(ex20_lib PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex20 ex20_main.cpp)
target_link_libraries(ex20 PRIVATE ex20_lib)
target_compile_definitions(ex20 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex20_test ex20_test.cpp)
target_link_libraries(ex20_test PRIVATE ex20_lib GTest::gtest_main)
target_compile_definitions(ex20_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex20_test COMMAND ex20_test)
