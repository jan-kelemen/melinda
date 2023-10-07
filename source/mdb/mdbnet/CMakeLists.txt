set(TARGET_NAME mdbnet)

find_package(flatbuffers CONFIG REQUIRED)
find_package(cppzmq REQUIRED)
find_package(Boost REQUIRED)

build_flatbuffers(
    "mdbnet_wire.fbs" # Schema files
    "" # Schema include directories 
    "mdbnet_generate_fb" # Target name 
    "" # Additional dependencies
    "${CMAKE_CURRENT_BINARY_DIR}/include" # Generated includes directory
    "" # Binary schema directory
    "" # Text schemas directory
)

add_library(${TARGET_NAME} SHARED)

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbnet_client.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbnet_server.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbnet_types.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbnet_serialization.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_client.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_internal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_types.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_serialization.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_wire_generated.cpp
)

add_dependencies(${TARGET_NAME} mdbnet_generate_fb clang-format)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        flatbuffers::flatbuffers
        cppzmq
        mblcxx
    PRIVATE
        Boost::headers
        mbltrc
        project-options
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE
        src
)

