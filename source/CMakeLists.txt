add_subdirectory(common)

set(TARGET_NAME tryout)

add_executable(${TARGET_NAME} main.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        common_trace
)

