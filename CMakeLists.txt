cmake_minimum_required(VERSION 3.27)

project(melinda VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MELINDA_ENABLE_CLANG_FORMAT "Enable clang-format in build" OFF)
option(MELINDA_ENABLE_IWYU "Enable include-what-you-use tool in build" OFF)

include(${PROJECT_SOURCE_DIR}/cmake/iwyu.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/clang-format.cmake)

include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/global-settings.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/project-options.cmake)

enable_testing()
 
add_subdirectory(source)

