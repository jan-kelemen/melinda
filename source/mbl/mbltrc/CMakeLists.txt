include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mbltrc)

add_library(${TARGET_NAME} SHARED)

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mbltrc_trace.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mbltrc_trace.cpp
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt::fmt
    PRIVATE
        mblcxx
        date::date
        project-options
)
add_dependencies(${TARGET_NAME} clang-format)

target_include_directories(${TARGET_NAME} PUBLIC include)

add_executable(${TARGET_NAME}_test)

target_sources(${TARGET_NAME}_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mbltrc_trace.t.cpp
)

target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        Catch2::Catch2WithMain
        project-options
)
add_dependencies(${TARGET_NAME}_test clang-format)

catch_discover_tests(${TARGET_NAME}_test)

