include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mdbsrv)

add_executable(${TARGET_NAME}
    src/mdbsrv_options.cpp
    src/mdbsrv.m.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_include_directories(${TARGET_NAME} PUBLIC include)
target_link_libraries(${TARGET_NAME}
    PRIVATE
        cppzmq::cppzmq
        mblcxx
        mbltrc
        mdbnet
        mdbsql
        project-options
)
