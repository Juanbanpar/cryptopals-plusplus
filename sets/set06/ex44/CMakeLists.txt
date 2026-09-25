add_library(ex44_lib STATIC ex44.cpp)
target_include_directories(ex44_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex44_lib PUBLIC dsa_lib)

add_executable(ex44 ex44_main.cpp)
target_compile_definitions(ex44 PRIVATE EX44_RESOURCE="${PROJECT_SOURCE_DIR}/resources/set06/ex44/44.txt")
target_link_libraries(ex44 PRIVATE ex44_lib)

add_executable(ex44_test ex44_test.cpp)
target_compile_definitions(ex44_test PRIVATE EX44_RESOURCE="${PROJECT_SOURCE_DIR}/resources/set06/ex44/44.txt")
target_link_libraries(ex44_test PRIVATE ex44_lib GTest::gtest_main)

add_test(NAME ex44_test COMMAND ex44_test)
