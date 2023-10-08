set(TARGET_NAME mqlprs)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mdbsql_ast_unicode_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_parse_error.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser/mdbsql_parser_unicode_delimited_identifier.h
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt::fmt
        foonathan::lexy
        mblcxx
)

add_global_settings_to_target(${TARGET_NAME})

add_executable(${TARGET_NAME}_test)

target_sources(${TARGET_NAME}_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/parser/mdbsql_parser_regular_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/parser/mdbsql_parser_delimited_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/parser/mdbsql_parser_unicode_delimited_identifier.t.cpp
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

