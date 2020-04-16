# https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
    
add_custom_target(clang-format 
        COMMAND 
            clang-format-10 -style=file -i ${ALL_SOURCE_FILES})

