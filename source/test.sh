#!/bin/bash

run_tests () {
    declare -i cumulative_error=0
    for test_binary in build/bin/*_test; do
        echo "Executing: $test_binary" 
        ./$test_binary > "${test_binary}_report.txt"
        if [ $? -ne 0 ] 
        then
            cat "${test_binary}_report.txt"
            let cumulative_error=1
        fi
    done
    return $cumulative_error
}

run_tests

