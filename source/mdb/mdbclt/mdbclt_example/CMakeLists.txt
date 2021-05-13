set(TARGET_NAME mdbclt_example)

add_executable(${TARGET_NAME}
    src/mdbclt_example.m.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        mblcxx
        mbltrc
        mdbnet
)
