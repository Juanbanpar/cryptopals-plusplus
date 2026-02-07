# Top-level Makefile to configure, build and run tests
.PHONY: all configure build test clean

all: test

configure:
	cmake -S . -B build

build: configure
	cmake --build build -- -j$(shell nproc)

test: build
	(cd build && ctest --output-on-failure)

clean:
	rm -rf build
