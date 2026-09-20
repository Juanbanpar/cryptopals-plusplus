add_library(ex36_lib STATIC ex36.cpp)
target_include_directories(ex36_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex36_lib PUBLIC ex33_lib bignum sha256 common)

add_executable(ex36 ex36_main.cpp)
target_link_libraries(ex36 PRIVATE ex36_lib)

add_executable(ex36_test ex36_test.cpp)
target_link_libraries(ex36_test PRIVATE ex36_lib GTest::gtest_main)

add_test(NAME ex36_test COMMAND ex36_test)
