add_library(ex12_lib STATIC ex12.cpp)
target_include_directories(ex12_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex12_lib PUBLIC common aes pkcs7 ex11_lib)

add_executable(ex12 ex12_main.cpp)
target_link_libraries(ex12 PRIVATE ex12_lib)
target_compile_definitions(ex12 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex12_test ex12_test.cpp)
target_link_libraries(ex12_test PRIVATE ex12_lib GTest::gtest_main)
target_compile_definitions(ex12_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex12_test COMMAND ex12_test)
