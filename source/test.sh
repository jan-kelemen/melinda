#!/bin/bash

for filename in build/bin/*_test; do 
    [ -e "$filename" ] && ./$filename
done

