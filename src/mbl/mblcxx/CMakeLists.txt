add_library(mblcxx INTERFACE)

target_sources(mblcxx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_expected.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_scope_exit.hpp
)

target_include_directories(mblcxx
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(mblcxx
        INTERFACE
            tl::expected
    )

    target_compile_definitions(mblcxx
        INTERFACE
            MELINDA_MBLCXX_USE_TL_EXPECTED
    )
endif()

if (MELINDA_BUILD_TESTS)
    add_executable(mblcxx_test)

    target_sources(mblcxx_test
        PRIVATE
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
