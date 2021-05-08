#!/bin/bash

CONFIGURATION=Debug
SETUP_DEPENDENCIES_ONLY=OFF

optstring=":c:s"

while getopts ${optstring} option
do
    case "${option}"
    in
        c) CONFIGURATION=${OPTARG};;
        s) SETUP_DEPENDENCIES_ONLY="ON";;
    esac
done

if [ -d build ]
then
    rm -rf build
fi

mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=${CONFIGURATION} -DSETUP_DEPENDENCIES_ONLY=${SETUP_DEPENDENCIES_ONLY} && \
    cmake --build . && \
    cd ..

