#!/bin/bash

# Create build directory
mkdir build

# Move to build directory
cd build

# Create build files using cmake
cmake ..

# Compile program using make
make