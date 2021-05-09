enable_testing()

add_compile_options(-W -Wall -Wextra -pedantic)

add_subdirectory(network)
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

