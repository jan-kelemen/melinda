include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mdbsql)

add_library(${TARGET_NAME}
    src/mdbsql_engine.cpp
    src/mdbsql_ast.cpp
    src/mdbsql_parser.cpp
    src/ast/mdbsql_ast_column_definition.cpp
    src/ast/mdbsql_ast_identifier.cpp
    src/ast/mdbsql_ast_schema_definition.cpp
    src/ast/mdbsql_ast_schema_element.cpp
    src/ast/mdbsql_ast_sql_executable_statement.cpp
    src/ast/mdbsql_ast_sql_schema_statement.cpp
    src/ast/mdbsql_ast_table_definition.cpp
    src/ast/mdbsql_ast_table_elements.cpp
)
add_dependencies(${TARGET_NAME} clang-format)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt::fmt
        mblcxx
        mbltrc
    PRIVATE
        foonathan::lexy_core
        foonathan::lexy_unicode
        foonathan::lexy_ext
        project-options
)
target_include_directories(${TARGET_NAME} PUBLIC include include/ast)

add_executable(${TARGET_NAME}_test
    test/mdbsql.t.cpp
    test/mdbsql_engine.t.cpp
    test/mdbsql_ast.t.cpp
    test/mdbsql_parser.t.cpp
    test/ast/mdbsql_ast_column_definition.t.cpp
    test/ast/mdbsql_ast_identifier.t.cpp
    test/ast/mdbsql_ast_schema_definition.t.cpp
    test/ast/mdbsql_ast_schema_element.t.cpp
    test/ast/mdbsql_ast_sql_executable_statement.t.cpp
    test/ast/mdbsql_ast_sql_schema_statement.t.cpp
    test/ast/mdbsql_ast_table_definition.t.cpp
    test/ast/mdbsql_ast_table_elements.t.cpp
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
