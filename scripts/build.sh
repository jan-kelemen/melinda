#!/bin/bash

CONFIGURATION=Debug

optstring=":c"

while getopts ${optstring} option
do
    case "${option}"
    in
        c) CONFIGURATION=${OPTARG};;
    esac
done

if [ -d build ]
then
    rm -rf build
fi

mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=${CONFIGURATION} && \
    cmake --build . && \
    cd ..

