add_library(ex11_lib STATIC ex11.cpp)
target_include_directories(ex11_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex11_lib PUBLIC common aes pkcs7)

add_executable(ex11 ex11_main.cpp)
target_link_libraries(ex11 PRIVATE ex11_lib)
target_compile_definitions(ex11 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex11_test ex11_test.cpp)
target_link_libraries(ex11_test PRIVATE ex11_lib GTest::gtest_main)
target_compile_definitions(ex11_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex11_test COMMAND ex11_test)
