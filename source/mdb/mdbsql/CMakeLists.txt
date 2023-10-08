set(TARGET_NAME mdbsql)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_engine.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_parser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_column_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_schema_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_schema_element.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_sql_executable_statement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_sql_schema_statement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_table_definition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_table_elements.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsql_engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsql_parser.cpp
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt::fmt
        foonathan::lexy
        mblcxx
        mbltrc
        mqlprs
)

add_global_settings_to_target(${TARGET_NAME})

add_executable(${TARGET_NAME}_test)

target_sources(${TARGET_NAME}_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql_engine.t.cpp
        #${CMAKE_CURRENT_SOURCE_DIR}/test/mdbsql_parser.t.cpp
)

target_include_directories(${TARGET_NAME}_test 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/test)

target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        Catch2::Catch2WithMain
)

add_global_settings_to_target(${TARGET_NAME}_test)

include(Catch)
catch_discover_tests(${TARGET_NAME}_test)

