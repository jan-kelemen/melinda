include(${PROJECT_SOURCE_DIR}/cmake/conan.cmake)

# Conan and dependencies configuration

conan_cmake_configure(
    REQUIRES
        catch2/3.1.0
        fmt/9.1.0
        date/3.0.1
        boost/1.79.0
        cppzmq/4.8.1
        flatbuffers/2.0.5
        zlib/1.2.12
    OPTIONS
        zeromq:encryption=tweetnacl
        boost:without_test=True
    GENERATORS
        cmake_find_package
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
find_package(FlatBuffers REQUIRED)
