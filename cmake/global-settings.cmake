function(add_global_settings_to_target target_name)
    target_compile_features(${target_name} PUBLIC cxx_std_23)
    set_target_properties(${target_name} PROPERTIES CXX_STANDARD_REQUIRED ON)
    set_target_properties(${target_name} PROPERTIES CXX_EXTENSIONS OFF)

    if (MELINDA_ENABLE_IWYU)
        set_property(
            TARGET 
                ${target_name}
            PROPERTY 
                CXX_INCLUDE_WHAT_YOU_USE ${MELINDA_CXX_INCLUDE_WHAT_YOU_USE})
    endif()

    if (MELINDA_ENABLE_CLANG_FORMAT)
        add_dependencies(${target_name} clang-format)
    endif()

    target_link_libraries(${target_name} PRIVATE project-options)
endfunction()

