set(TARGET_NAME mqlprs)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mqlprs_ast_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mqlprs_ast_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mqlprs_ast_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ast/mqlprs_ast_unicode_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_parse_error.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_parser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_unicode_delimited_identifier.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mqlprs_ast_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mqlprs_ast_delimited_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mqlprs_ast_regular_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/mqlprs_ast_unicode_delimited_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlprs_common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlprs_parse_error.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlprs_parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlprs_regular_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlprs_delimited_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlprs_unicode_delimited_identifier.cpp
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
)

add_global_settings_to_target(${TARGET_NAME})

add_executable(${TARGET_NAME}_test)

target_sources(${TARGET_NAME}_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlprs_regular_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlprs_delimited_identifier.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlprs_unicode_delimited_identifier.t.cpp
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

