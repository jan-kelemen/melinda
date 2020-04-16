include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME common_trace)

add_library(${TARGET_NAME} SHARED src/trace.cpp)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        CONAN_PKG::fmt
        CONAN_PKG::boost_filesystem
    PRIVATE
        CONAN_PKG::date
)
add_dependencies(${TARGET_NAME} clang-format)

add_executable(${TARGET_NAME}_test test/trace.t.cpp)
target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::Catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)

