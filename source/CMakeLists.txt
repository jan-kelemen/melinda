cmake_minimum_required(VERSION 3.10)

set(TARGET_NAME melinda)

project(${TARGET_NAME} VERSION 0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)
include(cmake/global-tools.cmake)
include(cmake/conan.cmake)

conan_add_remote(
    NAME
        catchorg
    URL
        https://api.bintray.com/conan/catchorg/Catch2
)

conan_cmake_run(
    REQUIRES
        Catch2/2.9.1@catchorg/stable
    BASIC_SETUP
        CMAKE_TARGETS
    BUILD
        missing
)

add_compile_options(-Wall -Wextra)

add_executable(${TARGET_NAME} src/main.cpp)
global_tools(${TARGET_NAME})

