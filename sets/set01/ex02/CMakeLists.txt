add_executable(ex02_test ex02_test.cpp)
target_link_libraries(ex02_test PRIVATE common GTest::gtest_main)

add_executable(ex02 ex02_main.cpp)
target_link_libraries(ex02 PRIVATE common)

add_test(NAME ex02_test COMMAND ex02_test)
