add_library(ex25_lib STATIC ex25.cpp)
target_include_directories(ex25_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex25_lib PUBLIC common aes)
target_compile_definitions(ex25_lib PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex25 ex25_main.cpp)
target_link_libraries(ex25 PRIVATE ex25_lib)
target_compile_definitions(ex25 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex25_test ex25_test.cpp)
target_link_libraries(ex25_test PRIVATE ex25_lib GTest::gtest_main)
target_compile_definitions(ex25_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex25_test COMMAND ex25_test)
