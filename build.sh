#!/bin/sh

set -e

if [ -d build ]; then
	rm -rf build
fi

BUILD_TYPE=Release
if [ "$1" = "" ]; then
	BUILD_TYPE=$1
fi

mkdir build && cd build
cmake .. -D CMAKE_BUILD_TYPE=$BUILD_TYPE
cmake --build .
cd ..