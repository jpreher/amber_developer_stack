#!/bin/bash

# @brief The purpose of this script is to show that one must
# first build the package `common_cmake` before using it.
# This specifically shows using workspace overlays (via
# `source devel/setup.bash` in the shadow build directory)

# @todo Use regex to confirm build process

set -e
mkcd () 
{ 
    local dir=${!#} flags="${@:1:$(($#-1))}";
    mkdir $flags $dir && cd $dir
}

# Change directories
roscd common_cmake
# Create directory
mkcd -p build
# Configure cmake
cmake ..
# Use the new develspace
source devel/setup.bash

roscd common_cmake_test
mkcd -p build
cmake ..

echo
echo "You should have see the following path printed out:"
echo "    $(rospack find common_cmake)/cmake"
