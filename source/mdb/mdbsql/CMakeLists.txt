include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mdbsql)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_engine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_ast.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_parser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_column_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_schema_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_schema_element.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_sql_executable_statement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_sql_schema_statement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_table_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_table_elements.h
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
    PRIVATE
        foonathan::lexy_core
        foonathan::lexy_unicode
        project-options
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        include 
        include/ast
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
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_schema_definition.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_schema_element.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_sql_executable_statement.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_sql_schema_statement.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_table_definition.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ast/mdbsql_ast_table_elements.t.cpp
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

catch_discover_tests(${TARGET_NAME}_test)

