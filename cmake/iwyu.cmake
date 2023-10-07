if (MELINDA_ENABLE_IWYU)
    find_program(IWYU_EXE NAMES include-what-you-use iwyu REQUIRED)
    message(STATUS "include-what-you-use found: ${IWYU_EXE}")

    set(MELINDA_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_EXE};-Xiwyu;--comment_style=long;-Xiwyu;--cxx17ns;-Xiwyu;--mapping_file=${PROJECT_SOURCE_DIR}/tools/iwyu.imp")
endif()

