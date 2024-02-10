add_library(compiler-analyzer INTERFACE)

set(MSVC_ANALYZER
    /analyze /analyze:external-)

set(GCC_ANALYZER
    -fanalyzer)

if(MSVC)
    set(ANALYZER_OPTIONS ${MSVC_ANALYZER})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(ANALYZER_OPTIONS ${GCC_ANALYZER})
else()
    message(AUTHOR_WARNING "No compiler analyzer options for CXX compiler: '${CMAKE_CXX_COMPILER_ID}'")
endif()

target_compile_options(compiler-analyzer
    INTERFACE $<$<COMPILE_LANGUAGE:C,CXX>:${ANALYZER_OPTIONS}>)

