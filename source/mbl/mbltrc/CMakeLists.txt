include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mbltrc)

add_library(${TARGET_NAME} SHARED src/mbltrc_trace.cpp)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt
    PRIVATE
        mblcxx
        date
)
add_dependencies(${TARGET_NAME} clang-format)
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC include)

add_executable(${TARGET_NAME}_test 
    test/mbltrc_test.m.cpp
    test/mbltrc_trace.t.cpp)
target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        Catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)

