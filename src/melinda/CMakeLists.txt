add_executable(melinda)

target_sources(melinda
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/melinda.m.cpp
)

target_link_libraries(melinda
    PRIVATE
        fmt::fmt
        project-options
)

if (MELINDA_BUILD_TESTS)
    add_executable(melinda_test)

    target_sources(melinda_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/test/melinda.t.cpp
    )

    target_link_libraries(melinda_test
        PRIVATE
            Catch2::Catch2WithMain
            project-options
    )

    include(Catch)
    catch_discover_tests(melinda_test)
endif()
