set(TARGET_NAME mdbsql)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsql_engine.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsql_engine.cpp
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
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

