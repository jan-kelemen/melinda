set(TARGET_NAME mdbnet)

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

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_dependencies(${TARGET_NAME} mdbnet_generate_fb)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        cppzmq
        flatbuffers::flatbuffers
        tl::expected
        mblcxx
    PRIVATE
        Boost::headers
        mbltrc
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/mdbnet_wire_generated.cpp PROPERTIES
    SKIP_LINTING ON
)

add_global_settings_to_target(${TARGET_NAME})
