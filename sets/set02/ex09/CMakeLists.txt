add_library(ex09_lib STATIC ex09.cpp)
target_include_directories(ex09_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex09_lib PUBLIC common pkcs7)

add_executable(ex09 ex09_main.cpp)
target_link_libraries(ex09 PRIVATE ex09_lib)
target_compile_definitions(ex09 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex09_test ex09_test.cpp)
target_link_libraries(ex09_test PRIVATE ex09_lib GTest::gtest_main)
target_compile_definitions(ex09_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex09_test COMMAND ex09_test)
