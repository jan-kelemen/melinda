include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mdbsrv)

add_executable(${TARGET_NAME}
    src/mdbsrv.m.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        cppzmq::cppzmq
        mblcxx
        mbltrc
        mdbnet
)
