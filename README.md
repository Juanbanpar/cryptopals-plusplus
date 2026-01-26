# cryptopals-plusplus
Cryptopals crypto challenges in C++

## Project layout

- `CMakeLists.txt` — top-level CMake that fetches GoogleTest and includes `sets/`.
- `sets/` — contains one folder per Cryptopals set (`set01`, `set02`, ...).
  - `setXX/` — CMake file that pulls in exercises for that set.
    - `exYY/` — folder for each exercise (source + tests).

Example tree:

```
CMakeLists.txt
README.md
sets/
  set01/
    CMakeLists.txt
    ex01/
      CMakeLists.txt
      ex01.cpp
      ex01.h
      ex01_test.cpp
  set02/
    ex01/
    ex02/
  ...
```

## How to build and run tests

Requirements:

- `cmake` (>= 3.14)
- A C++ compiler (g++, clang++)
- `make` / build tools

Build and run tests:

```bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j$(nproc)
ctest --output-on-failure
```

## Notes

- Each exercise should expose library targets and a corresponding test executable linked to `GTest::gtest_main`.
