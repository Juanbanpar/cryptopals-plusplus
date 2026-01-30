add_library(aes STATIC aes.cpp)
target_include_directories(aes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# aes does not depend on common unless I extracted something? No, I implemented helpers inline.
