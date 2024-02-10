cmake_minimum_required(VERSION 3.27)

project(melinda VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MELINDA_ENABLE_CLANG_FORMAT "Enable clang-format in build" OFF)
option(MELINDA_ENABLE_CLANG_TIDY "Enable clang-tidy in build" OFF)
option(MELINDA_ENABLE_COMPILER_STATIC_ANALYSIS "Enable static analysis provided by compiler in build" OFF)
option(MELINDA_ENABLE_CPPCHECK "Enable cppcheck in build" OFF)
option(MELINDA_ENABLE_IWYU "Enable include-what-you-use in build" OFF)

find_package(fmt REQUIRED)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if (BUILD_TESTING)
        find_package(Catch2)
        set(MELINDA_BUILD_TESTS ON)
    endif()

    include(${PROJECT_SOURCE_DIR}/cmake/clang-tidy.cmake)
    include(${PROJECT_SOURCE_DIR}/cmake/cppcheck.cmake)
    include(${PROJECT_SOURCE_DIR}/cmake/iwyu.cmake)

    include(${PROJECT_SOURCE_DIR}/cmake/project-options.cmake)
endif()

add_subdirectory(src)
