add_library(ex26_lib STATIC ex26.cpp)
target_include_directories(ex26_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex26_lib PUBLIC common aes)

add_executable(ex26 ex26_main.cpp)
target_link_libraries(ex26 PRIVATE ex26_lib)

add_executable(ex26_test ex26_test.cpp)
target_link_libraries(ex26_test PRIVATE ex26_lib GTest::gtest_main)

add_test(NAME ex26_test COMMAND ex26_test)
