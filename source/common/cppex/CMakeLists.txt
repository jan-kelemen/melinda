include(../../cmake/global-tools.cmake)

set(TARGET_NAME common_cppex)

add_library(${TARGET_NAME} STATIC 
    src/iterator_traits.cpp
    src/scope_exit.cpp
    src/unused.cpp)
global_tools(${TARGET_NAME})

add_executable(${TARGET_NAME}_test 
    test/main.cpp
    test/iterator_traits.t.cpp
    test/scope_exit.t.cpp
    test/unused.t.cpp)
target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::Catch2
)
global_tools(${TARGET_NAME}_test)

