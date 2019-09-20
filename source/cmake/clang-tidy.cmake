# http://mariobadr.com/using-clang-tidy-with-cmake-36.html

find_program(CLANG_TIDY_EXE
        NAMES "clang-tidy-9"
        DOC "Path to clang-tidy executable"
    )

if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=\
        bugprone-*,\
        cppcoreguidelines-*,\
        clang-analyzer-*,\
        misc-*,\
        modernize-*,\
        performance-*,\
        portability-*,\
        readability-*"
    )
endif()

