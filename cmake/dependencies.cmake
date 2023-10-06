include(FetchContent)
FetchContent_Declare(
    lexy
    GIT_REPOSITORY https://github.com/foonathan/lexy.git
    GIT_TAG 7e583fe0c717715a10227d2d4ba14581143cf0af
)
FetchContent_MakeAvailable(lexy)

find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)
find_package(date REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams program_options)
find_package(cppzmq REQUIRED)
find_package(flatbuffers REQUIRED)
#find_package(lexy REQUIRED)

