# https://arcanis.me/en/2015/10/17/cppcheck-and-clang-format

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h)
    
add_custom_target(clangformat 
        COMMAND 
            clang-format -style=file -i ${ALL_SOURCE_FILES})

