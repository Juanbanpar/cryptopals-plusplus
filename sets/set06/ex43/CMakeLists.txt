add_library(dsa_lib STATIC dsa.cpp)
target_include_directories(dsa_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dsa_lib PUBLIC bignum sha1)

add_library(ex43_lib STATIC ex43.cpp)
target_include_directories(ex43_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ex43_lib PUBLIC dsa_lib)

add_executable(ex43 ex43_main.cpp)
target_link_libraries(ex43 PRIVATE ex43_lib)

add_executable(ex43_test ex43_test.cpp)
target_link_libraries(ex43_test PRIVATE ex43_lib GTest::gtest_main)

add_test(NAME ex43_test COMMAND ex43_test)
