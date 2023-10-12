set(TARGET_NAME mqlast)

add_library(${TARGET_NAME})

target_sources(${TARGET_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_common.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_delimited_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_numeric_literal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_regular_identifier.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/mqlast_unicode_delimited_identifier.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlast_common.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlast_delimited_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlast_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlast_numeric_literal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlast_regular_identifier.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/mqlast_unicode_delimited_identifier.cpp
)

target_include_directories(${TARGET_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

target_link_libraries(${TARGET_NAME}
    PUBLIC
        fmt::fmt
)

add_global_settings_to_target(${TARGET_NAME})

