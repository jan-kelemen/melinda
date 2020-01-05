include(../../cmake/global-tools.cmake)

set(TARGET_NAME common_cppex)

add_library(${TARGET_NAME} STATIC src/iterator_traits.cpp)
global_tools(${TARGET_NAME})

add_executable(${TARGET_NAME}_test test/iterator_traits.t.cpp)
target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::Catch2
)
global_tools(${TARGET_NAME}_test)

