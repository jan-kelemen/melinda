include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mdbsql)

add_library(${TARGET_NAME}
    src/mdbsql_engine.cpp
    src/mdbsql_ast.cpp
    src/mdbsql_parser.cpp
)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        foonathan::lexy_core
        foonathan::lexy_unicode
        foonathan::lexy_ext
        mblcxx
        mbltrc
    PRIVATE
        project-options
)
target_include_directories(${TARGET_NAME} PUBLIC include)

add_executable(${TARGET_NAME}_test
    test/mdbsql.t.cpp
    test/mdbsql_engine.t.cpp
    test/mdbsql_ast.t.cpp
    test/mdbsql_parser.t.cpp
)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        Catch2::Catch2WithMain
        project-options
)
target_include_directories(${TARGET_NAME}_test PRIVATE test)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)
