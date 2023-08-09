cmake_minimum_required(VERSION 3.4)

set(TARGET_NAME melinda)
project(${TARGET_NAME} VERSION 0.1 LANGUAGES CXX)

# Export compile_commands.json during build for integration with other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_BINARY_DIR})

include(GNUInstallDirs)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH $ORIGIN $ORIGIN/../lib)

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message("${_variableName}=${${_variableName}}")
endforeach()

include(${PROJECT_SOURCE_DIR}/cmake/clang-format.cmake)

include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message("${_variableName}=${${_variableName}}")
endforeach()

include(${PROJECT_SOURCE_DIR}/cmake/project-options.cmake)

enable_testing()
 
add_subdirectory(source)

include(${PROJECT_SOURCE_DIR}/cmake/install.cmake)
