set(TARGET_NAME mclxmp)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mclxmp.m.cpp
)

add_dependencies(${TARGET_NAME} clang-format)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        Boost::Boost
        mblcxx
        mbltrc
        mdbnet
        project-options
)

