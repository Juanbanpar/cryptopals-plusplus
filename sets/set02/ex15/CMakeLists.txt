add_library(ex15_lib STATIC ex15.cpp)
target_include_directories(ex15_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex15_lib PUBLIC pkcs7)

add_executable(ex15 ex15_main.cpp)
target_link_libraries(ex15 PRIVATE ex15_lib)

add_executable(ex15_test ex15_test.cpp)
target_link_libraries(ex15_test PRIVATE ex15_lib GTest::gtest_main)

add_test(NAME ex15_test COMMAND ex15_test)
