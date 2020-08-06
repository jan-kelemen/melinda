include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME client)

add_executable(${TARGET_NAME}
    src/main.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        CONAN_PKG::cppzmq
        common_trace
)

add_executable(
    ${TARGET_NAME}_test
    test/main.cpp
)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        CONAN_PKG::Catch2
)

