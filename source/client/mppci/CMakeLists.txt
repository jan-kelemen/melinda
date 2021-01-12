include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME client_mppci)

add_library(${TARGET_NAME} SHARED
    src/mppci.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        CONAN_PKG::cppzmq
        common_cppex
        common_trace
        network_ncprot
)
target_include_directories(${TARGET_NAME} PUBLIC include)

add_executable(
    ${TARGET_NAME}_test
    test/mppci.cpp
)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        CONAN_PKG::Catch2
        ${TARGET_NAME}
)

