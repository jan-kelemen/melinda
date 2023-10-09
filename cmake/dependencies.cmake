include(FetchContent)
FetchContent_Declare(
    lexy
    GIT_REPOSITORY https://github.com/foonathan/lexy.git
    GIT_TAG 7e583fe0c717715a10227d2d4ba14581143cf0af
)
FetchContent_MakeAvailable(lexy)

find_package(Boost REQUIRED COMPONENTS iostreams program_options)
find_package(Catch2 CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(flatbuffers CONFIG REQUIRED)
#find_package(lexy REQUIRED) # Currently rolled forward to avoid build errors with clang
find_package(tl-expected REQUIRED)

