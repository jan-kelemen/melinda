add_library(mblcxx INTERFACE)

target_sources(mblcxx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_result.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_scope_exit.hpp
)

target_include_directories(mblcxx
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(mblcxx
    INTERFACE
        tl::expected
)

if (MELINDA_BUILD_TESTS)
    add_executable(mblcxx_test)

    target_sources(mblcxx_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mblcxx_result.t.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/mblcxx_scope_exit.t.cpp
    )

    target_link_libraries(mblcxx_test
        PRIVATE
            Catch2::Catch2WithMain
            mblcxx
            project-options
    )

    include(Catch)
    catch_discover_tests(mblcxx_test)
endif()
