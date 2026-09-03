add_library(ex19_lib STATIC ex19.cpp)
target_include_directories(ex19_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex19_lib PUBLIC common aes)

add_executable(ex19 ex19_main.cpp)
target_link_libraries(ex19 PRIVATE ex19_lib)

add_executable(ex19_test ex19_test.cpp)
target_link_libraries(ex19_test PRIVATE ex19_lib GTest::gtest_main)

add_test(NAME ex19_test COMMAND ex19_test)
