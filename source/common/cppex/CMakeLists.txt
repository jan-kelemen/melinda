include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME common_cppex)

add_library(${TARGET_NAME} STATIC 
    src/iterator_traits.cpp
    src/scope_exit.cpp
    src/unused.cpp)
add_dependencies(${TARGET_NAME} clang-format)

add_executable(${TARGET_NAME}_test 
    test/main.cpp
    test/iterator_traits.t.cpp
    test/scope_exit.t.cpp
    test/unused.t.cpp)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::Catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)

