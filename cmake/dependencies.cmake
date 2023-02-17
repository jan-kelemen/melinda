include(${PROJECT_SOURCE_DIR}/cmake/conan.cmake)

# Conan and dependencies configuration
conan_cmake_configure(
    REQUIRES
        catch2/3.3.1
        fmt/9.1.0
        date/3.0.1
        boost/1.81.0
        cppzmq/4.9.0
        flatbuffers/23.1.21
        foonathan-lexy/2022.12.1
    OPTIONS
        zeromq:encryption=tweetnacl
        boost:without_atomic=True
        boost:without_chrono=True
        boost:without_container=True
        boost:without_context=True
        boost:without_contract=True
        boost:without_coroutine=True
        boost:without_date_time=True
        boost:without_exception=True
        boost:without_fiber=True
        boost:without_filesystem=True
        boost:without_graph=True
        boost:without_graph_parallel=True
        boost:without_iostreams=True
        boost:without_json=True
        boost:without_locale=True
        boost:without_log=True
        boost:without_math=True
        boost:without_mpi=True
        boost:without_nowide=True
        boost:without_program_options=False
        boost:without_python=True
        boost:without_random=True
        boost:without_regex=True
        boost:without_serialization=True
        boost:without_stacktrace=True
        boost:without_system=True
        boost:without_test=True
        boost:without_thread=True
        boost:without_timer=True
        boost:without_type_erasure=True
        boost:without_url=True
        boost:without_wave=True
    GENERATORS
        cmake_find_package
    IMPORTS
        "bin, *.dll -> ./bin"
        "lib, *.so* -> ./lib"
)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    SETTINGS ${settings})

find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)
find_package(date REQUIRED)
find_package(Boost REQUIRED)
find_package(cppzmq REQUIRED)
find_package(flatbuffers REQUIRED)
find_package(lexy REQUIRED)

