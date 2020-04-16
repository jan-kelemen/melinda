#!/bin/bash

if [ ! -d build ]
then
    echo ERROR: Build directory doesn\'t exist.
    exit 1
fi

cd build && ctest -V -W && cd ..

