set(TARGET_NAME mdbsql)

#find_package(lexy REQUIRED)
find_package(fmt REQUIRED)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_engine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_ast.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_parser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_column_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_unicode_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_schema_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_schema_element.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_sql_executable_statement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_sql_schema_statement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_table_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_table_elements.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_parse_error.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_unicode_delimited_identifier.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsql_engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsql_ast.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsql_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_column_definition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_schema_definition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_schema_element.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_sql_executable_statement.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_sql_schema_statement.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_table_definition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mdbsql_ast_table_elements.cpp
)

add_dependencies(${TARGET_NAME} clang-format)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt::fmt
        mblcxx
        mbltrc
        foonathan::lexy
    PRIVATE
        project-options
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        include 
        include/ast
        include/parser
)

add_executable(${TARGET_NAME}_test)

target_sources(${TARGET_NAME}_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql_engine.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql_ast.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql_parser.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_column_definition.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_regular_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_schema_definition.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_schema_element.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_sql_executable_statement.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_sql_schema_statement.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_table_definition.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_table_elements.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/parser/mdbsql_parser_regular_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/parser/mdbsql_parser_delimited_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/parser/mdbsql_parser_unicode_delimited_identifier.t.cpp
)

target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        Catch2::Catch2WithMain
        project-options
)

target_include_directories(${TARGET_NAME}_test 
    PRIVATE 
        test)

add_dependencies(${TARGET_NAME}_test clang-format)

include(Catch)
catch_discover_tests(${TARGET_NAME}_test)

