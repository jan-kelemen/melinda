add_library(mqlprs INTERFACE)

target_sources(mqlprs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_parse.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_parse_errors.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlprs_tree_util.hpp
)

target_include_directories(mqlprs
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mqlprs
    INTERFACE
        foonathan::lexy
        mblcxx
        project-options
)

if (MELINDA_BUILD_TESTS)
    add_executable(mqlprs_test)

    target_sources(mqlprs_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlprs_parse.t.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlprs_parse_errors.t.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mqlprs_tree_util.t.cpp
    )

    target_link_libraries(mqlprs_test
        PRIVATE
            Catch2::Catch2WithMain
            mqlprs
            project-options
    )

    include(Catch)
    catch_discover_tests(mqlprs_test)
endif()

