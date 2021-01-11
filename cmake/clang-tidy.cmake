# http://mariobadr.com/using-clang-tidy-with-cmake-36.html

find_program(
    CLANG_TIDY_EXE
    NAMES 
        "clang-tidy-11"
    DOC 
        "Path to clang-tidy executable"
)

if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

