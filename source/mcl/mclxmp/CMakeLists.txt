set(TARGET_NAME mclxmp)

add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mclxmp.m.cpp
)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        Boost::headers
        mblcxx
        mbltrc
        mdbnet
)

add_global_settings_to_target(${TARGET_NAME})
