#!/bin/bash
set -e

make
./build/tests/test_sr
# valgrind --track-origins=yes ./build/tests/test_sr
