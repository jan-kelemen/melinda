set(TARGET_NAME mclxmp)

add_executable(${TARGET_NAME}
    src/mclxmp.m.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        mblcxx
        mbltrc
        mdbnet
        project-options
)
