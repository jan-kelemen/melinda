include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mblcxx)

add_library(${TARGET_NAME} STATIC 
    src/mblcxx_iterator_traits.cpp
    src/mblcxx_scope_exit.cpp
    src/mblcxx_unused.cpp
    src/mblcxx_always_false.cpp
    src/mblcxx_result.cpp)
add_dependencies(${TARGET_NAME} clang-format)
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC include)

add_executable(${TARGET_NAME}_test 
    test/mblcxx_test.m.cpp
    test/mblcxx_iterator_traits.t.cpp
    test/mblcxx_scope_exit.t.cpp
    test/mblcxx_unused.t.cpp
    test/mblcxx_always_false.t.cpp
    test/mblcxx_result.t.cpp)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)

