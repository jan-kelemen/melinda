include(../../cmake/global-tools.cmake)

set(TARGET_NAME common_trace)

add_library(${TARGET_NAME} SHARED src/trace.cpp)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        CONAN_PKG::fmt
        CONAN_PKG::boost_filesystem
    PRIVATE
        CONAN_PKG::date
)
global_tools(${TARGET_NAME})

add_executable(${TARGET_NAME}_test test/trace.t.cpp)
target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::Catch2
)
global_tools(${TARGET_NAME}_test)

