find_package(OpenSSL REQUIRED)

add_library(ex07_lib STATIC ex07.cpp)
target_include_directories(ex07_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex07_lib PUBLIC common OpenSSL::Crypto)

add_executable(ex07 ex07_main.cpp)
target_link_libraries(ex07 PRIVATE ex07_lib)
target_compile_definitions(ex07 PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(ex07_test ex07_test.cpp)
target_link_libraries(ex07_test PRIVATE ex07_lib GTest::gtest_main)
target_compile_definitions(ex07_test PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_test(NAME ex07_test COMMAND ex07_test)
