set(TARGET_NAME mblcxx)

add_library(${TARGET_NAME} STATIC)

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_iterator_traits.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_scope_exit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_unused.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_always_false.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mblcxx_result.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mblcxx_iterator_traits.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mblcxx_scope_exit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mblcxx_unused.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mblcxx_always_false.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mblcxx_result.cpp
)
target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_global_settings_to_target(${TARGET_NAME})

add_executable(${TARGET_NAME}_test)

target_sources(${TARGET_NAME}_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mblcxx_iterator_traits.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mblcxx_scope_exit.t.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mblcxx_result.t.cpp
)

target_link_libraries(${TARGET_NAME}_test
    PRIVATE
        ${TARGET_NAME}
        Catch2::Catch2WithMain
)

add_global_settings_to_target(${TARGET_NAME}_test)

include(Catch)
catch_discover_tests(${TARGET_NAME}_test)

