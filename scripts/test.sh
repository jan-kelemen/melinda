#!/bin/bash

TEST_DIRECTORY=${MELINDA_TEST_DIRECTORY:-/tmp}

optstring="d:"

if [ ! -d build ]
then
    echo ERROR: Build directory doesn\'t exist.
    exit 1
fi

while getopts ${optstring} option
do
    case "${option}"
    in
        d) TEST_DIRECTORY=${OPTARG};;
    esac
done

echo "TD = ${TEST_DIRECTORY}"

cd build && \
    MELINDA_TEST_DIRECTORY=${TEST_DIRECTORY} ctest -V -W && \
    cd ..

