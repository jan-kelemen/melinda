include(${PROJECT_SOURCE_DIR}/cmake/conan.cmake)

# Conan and dependencies configuration
conan_add_remote(
    NAME
        bincrafters
    URL
        https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
)

conan_cmake_run(
    REQUIRES
        catch2/2.13.6
        fmt/7.1.3
        date/3.0.0
        boost/1.76.0
        cppzmq/4.7.1
        flatc/1.12.0
        flatbuffers/1.12.0
    BASIC_SETUP
        CMAKE_TARGETS
    BUILD
        missing
)
