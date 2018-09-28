#!/bin/sh

# Create the build (if on amazon use cmake3)
cmake .

# Build the projec
make

# Execute the test
./cpp/tests/assumption_tests

