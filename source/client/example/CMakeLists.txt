set(TARGET_NAME client_example)

add_executable(${TARGET_NAME}
    src/main.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        common_trace
        client_mppci
)
