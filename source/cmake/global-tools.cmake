function(global_tools target)
    # Region: Dependencies to clangformat target to execute it before compile
    add_dependencies(${target} clangformat)

    # Region: Do clang-tidy
    if(CLANG_TIDY_EXE)
        set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
    endif()
endfunction(global_tools)

