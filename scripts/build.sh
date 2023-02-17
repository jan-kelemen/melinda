#!/bin/bash

CONFIGURATION=Debug

optstring="c:"

while getopts ${optstring} option
do
    case "${option}"
    in
        c) CONFIGURATION=${OPTARG};;
    esac
done

if [ ! -d build ]
then
    rm -rf build
    bash scripts/configure.sh -c ${CONFIGURATION}
fi

cmake --build build --parallel $(getconf _NPROCESSORS_ONLN)
