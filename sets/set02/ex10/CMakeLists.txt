add_library(ex10_lib STATIC ex10.cpp)
target_include_directories(ex10_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex10_lib PUBLIC common aes)

add_executable(ex10 ex10_main.cpp)
target_link_libraries(ex10 PRIVATE ex10_lib)
target_compile_definitions(ex10 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex10_test ex10_test.cpp)
target_link_libraries(ex10_test PRIVATE ex10_lib GTest::gtest_main)
target_compile_definitions(ex10_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex10_test COMMAND ex10_test)
