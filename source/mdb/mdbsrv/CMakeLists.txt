include(${PROJECT_SOURCE_DIR}/cmake/Catch2.cmake)

set(TARGET_NAME mdbsrv)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbsrv_options.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsrv_options.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbsrv.m.cpp
)

add_dependencies(${TARGET_NAME} clang-format)

target_include_directories(${TARGET_NAME} PRIVATE include)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        Boost::program_options
        cppzmq::cppzmq
        mblcxx
        mbltrc
        mdbnet
        mdbsql
        project-options
)

