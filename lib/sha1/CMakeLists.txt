add_library(sha1 STATIC sha1.cpp)
target_include_directories(sha1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
