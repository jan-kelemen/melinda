cmake_minimum_required(VERSION 3.4)

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

# ugly hack around conan_add_remote

#conan_add_remote(
#    NAME
#        catchorg
#    URL
#        https://api.bintray.com/conan/catchorg/Catch2
#)

# for some reasoun the above code results in a remote not being added (?)

execute_process(COMMAND conan remote add 
    catchorg https://api.bintray.com/conan/catchorg/Catch2 -f)
execute_process(COMMAND conan remote add 
    bincrafters https://api.bintray.com/conan/bincrafters/public-conan -f)

conan_cmake_run(
    REQUIRES
        Catch2/2.9.1@catchorg/stable
        fmt/6.0.0@bincrafters/stable
        date/2.4.1@bincrafters/stable
        boost_filesystem/1.69.0@bincrafters/stable
    BASIC_SETUP
        CMAKE_TARGETS
    BUILD
        missing
)

add_compile_options(-W -Wall -Wextra -pedantic)

add_subdirectory(common)

add_executable(tryout src/main.cpp)
target_link_libraries(tryout
    PRIVATE
    common_trace)
