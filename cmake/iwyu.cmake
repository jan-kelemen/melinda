# https://stackoverflow.com/a/30951493

find_program(IWYU_EXE NAMES include-what-you-use iwyu REQUIRED)

if(NOT IWYU_EXE)
    message(STATUS "include-what-you-use not found.")
else()
    message(STATUS "include-what-you-use found: ${IWYU_EXE}")
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_EXE};-Xiwyu;--comment_style=long;-Xiwyu;--cxx17ns;-Xiwyu;--mapping_file=${PROJECT_SOURCE_DIR}/iwyu.imp")
endif()
