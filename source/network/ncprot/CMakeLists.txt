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
    src/ncprot_message.cpp
    src/wire_generated.cpp
)
add_dependencies(${TARGET_NAME} generate_fb clang-format)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        CONAN_PKG::flatbuffers
        common_cppex
    PRIVATE
        CONAN_PKG::cppzmq
        CONAN_PKG::boost_uuid
)
target_include_directories(${TARGET_NAME} PUBLIC include)

add_executable(${TARGET_NAME}_test
    test/main.cpp
    test/ncprot_client.t.cpp
    test/ncprot_message.t.cpp
    test/wire_generated.t.cpp
)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::Catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)
