add_library(ex37_lib STATIC ex37.cpp)
target_include_directories(ex37_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex37_lib PUBLIC ex36_lib)

add_executable(ex37 ex37_main.cpp)
target_link_libraries(ex37 PRIVATE ex37_lib)

add_executable(ex37_test ex37_test.cpp)
target_link_libraries(ex37_test PRIVATE ex37_lib GTest::gtest_main)

add_test(NAME ex37_test COMMAND ex37_test)
