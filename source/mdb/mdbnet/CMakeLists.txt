include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

build_flatbuffers(
    "mdbnet_wire.fbs" # Schema files
    "" # Schema include directories 
    "mdbnet_generate_fb" # Target name 
    "" # Additional dependencies
    "${CMAKE_CURRENT_SOURCE_DIR}/include" # Generated includes directory
    "" # Binary schema directory
    "" # Text schemas directory
)


set(TARGET_NAME mdbnet)

add_library(${TARGET_NAME} SHARED 
    src/mdbnet_client.cpp
    src/mdbnet_internal.cpp
    src/mdbnet_server.cpp
    src/mdbnet_types.cpp
    src/mdbnet_serialization.cpp
    src/mdbnet_wire_generated.cpp
)
add_dependencies(${TARGET_NAME} mdbnet_generate_fb clang-format)
target_link_libraries(${TARGET_NAME}
    PUBLIC
        CONAN_PKG::flatbuffers
        CONAN_PKG::cppzmq
        mblcxx
    PRIVATE
        CONAN_PKG::boost
        mbltrc
)
target_include_directories(${TARGET_NAME} SYSTEM PUBLIC include)

add_executable(${TARGET_NAME}_test
    test/mdbnet_test.m.cpp
    test/mdbnet_client.t.cpp
    test/mdbnet_server.t.cpp
    test/mdbnet_types.t.cpp
    test/mdbnet_serialization.t.cpp
    test/mdbnet_wire_generated.t.cpp
)
target_link_libraries(
    ${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        CONAN_PKG::catch2
)
add_dependencies(${TARGET_NAME}_test clang-format)
catch_discover_tests(${TARGET_NAME}_test)
