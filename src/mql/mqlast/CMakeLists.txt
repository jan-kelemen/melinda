add_library(mqlast INTERFACE)

target_sources(mqlast
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_from_parse_tree.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_syntax_tree.hpp
)

target_include_directories(mqlast
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mqlast
    INTERFACE
        mblcxx
        mqlprs
        project-options
)

if (MELINDA_BUILD_TESTS)
    add_executable(mqlast_test)

    target_sources(mqlast_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlast_from_parse_tree.t.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlast_syntax_tree.t.cpp
    )

    target_link_libraries(mqlast_test
        PRIVATE
            Catch2::Catch2WithMain
            mqlast
            project-options
    )

    include(Catch)
    catch_discover_tests(mqlast_test)
endif()

