# cryptopals-plusplus
Cryptopals crypto challenges in C++

## Project layout

- `CMakeLists.txt` — top-level CMake that fetches GoogleTest and includes `lib/` and `sets/`.
- `lib/` — shared libraries (e.g. `lib/common`).
- `sets/` — contains one folder per Cryptopals set (`set01`, `set02`, ...).
  - `setXX/` — CMake file that pulls in exercises for that set.
    - `exYY/` — folder for each exercise (source + tests, optional CLI).

Example tree:

```text
CMakeLists.txt
README.md
lib/
  common/
    common.h
    common.cpp
sets/
  set01/
    CMakeLists.txt
    ex01/
      CMakeLists.txt
      ex01_main.cpp
      ex01_test.cpp
      ex01.cpp
      ex01.h
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

## Run an exercise (CLI)

After building from the repo root, many exercises provide a small CLI executable under `sets/`.

Example (run ex01 with a hex string argument):

```bash
# from repo root
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . -- -j$(nproc)
# run the ex01 CLI
./sets/set01/ex01/ex01 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
```

## Library

- `lib/common/` — shared utilities (hex/base64/etc.) built as a `common` static library. Exercises should reuse `common` for functionality that will be shared across exercises.

## Notes

- Each exercise should expose a test executable (linked to `GTest::gtest_main`) and may provide a small CLI executable for manual runs.