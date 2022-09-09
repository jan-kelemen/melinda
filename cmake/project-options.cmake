add_library(project-options INTERFACE)

target_compile_options(project-options
    INTERFACE -W -Wall -Wextra -pedantic)
