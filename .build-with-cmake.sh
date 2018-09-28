#!/bin/sh

echo ""
echo "================================================================================"
echo "Building the Envelope Protection Solution on Linux/Unix"
echo "================================================================================"
echo ""

echo "--------------------------------------------------------------------------------"
#echo "Building with the auto-tools from the root directory"
echo "Building with cmake from the root directory"
echo "--------------------------------------------------------------------------------"

# Auto-tools build
#autoreconf -ivf
#./configure
#make
#make check

# Create the build (if on amazon use cmake3)
echo "Create the build with cmake"
cmake .

echo "Build the project"
make

echo "Execute the thest"
./cpp/tests/assumption_tests > assumption_tests.log

