cmake_minimum_required(VERSION 3.4)

set(TARGET_NAME melinda)
project(${TARGET_NAME} VERSION 0.1)

option(SETUP_DEPENDENCIES_ONLY "Build dependencies and not the sources" OFF)

# Export compile_commands.json during build for integration with other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/clang-format.cmake)
# include(cmake/clang-tidy.cmake)

include(cmake/dependencies.cmake)

set(FLATBUFFERS_FLATC_EXECUTABLE ${CONAN_BIN_DIRS_FLATC}/flatc)

enable_testing()

add_compile_options(-W -Wall -Wextra -pedantic)

if (NOT SETUP_DEPENDENCIES_ONLY)
    add_subdirectory(source)
endif()

