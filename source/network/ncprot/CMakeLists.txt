include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

build_flatbuffers(
    "wire.fbs" # Schema files
    "" # Schema include directories 
    "generate_fb" # Target name 
    "" # Additional dependencies
    "${CMAKE_CURRENT_SOURCE_DIR}/include" # Generated includes directory
    "" # Binary schema directory
    "" # Text schemas directory
)


set(TARGET_NAME network_ncprot)

add_library(${TARGET_NAME} SHARED 
    src/ncprot_client.cpp
    src/ncprot_internal.cpp
    src/ncprot_server.cpp
    src/ncprot_types.cpp
    src/ncprot_serialization.cpp
    src/wire_generated.cpp
)
add_dependencies(${TARGET_NAME} generate_fb clang-format)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        CONAN_PKG::flatbuffers
        CONAN_PKG::cppzmq
        common_cppex
    PRIVATE
        CONAN_PKG::boost
        common_trace
)
target_include_directories(${TARGET_NAME} PUBLIC include)

add_executable(${TARGET_NAME}_test
    test/main.cpp
    test/ncprot_client.t.cpp
    test/ncprot_server.t.cpp
    test/ncprot_types.t.cpp
    test/ncprot_serialization.t.cpp
    test/wire_generated.t.cpp
)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)
