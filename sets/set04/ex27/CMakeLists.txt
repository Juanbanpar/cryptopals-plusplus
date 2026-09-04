add_library(ex27_lib STATIC ex27.cpp)
target_include_directories(ex27_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex27_lib PUBLIC common aes pkcs7)

add_executable(ex27 ex27_main.cpp)
target_link_libraries(ex27 PRIVATE ex27_lib)

add_executable(ex27_test ex27_test.cpp)
target_link_libraries(ex27_test PRIVATE ex27_lib GTest::gtest_main)

add_test(NAME ex27_test COMMAND ex27_test)
