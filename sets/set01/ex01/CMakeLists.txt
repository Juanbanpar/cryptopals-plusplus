add_executable(ex01_test ex01_test.cpp)
target_link_libraries(ex01_test PRIVATE common GTest::gtest_main)

add_executable(ex01 ex01_main.cpp)
target_link_libraries(ex01 PRIVATE common)

add_test(NAME ex01_test COMMAND ex01_test)

