add_library(mdbxtr)

target_sources(mdbxtr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mdbxtr_execute.hpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mdbxtr_execute.cpp
)

target_include_directories(mdbxtr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mdbxtr
    PUBLIC
        mblcxx
    PRIVATE
        mqlast
        mqlprs
        project-options
)

if (MELINDA_BUILD_TESTS)
    add_executable(mdbxtr_test)

    target_sources(mdbxtr_test
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mdbxtr_execute.t.cpp
    )

    target_link_libraries(mdbxtr_test
            PRIVATE
            Catch2::Catch2WithMain
            mdbxtr
            project-options
    )

    include(Catch)
    catch_discover_tests(mdbxtr_test)
endif()

