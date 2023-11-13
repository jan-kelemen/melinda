# melinda
Melinda - Relational Database Management System

# Develpment environment setup
#### Required build tools:
* CMake 3.27

#### Supported compilers:
* GCC-13
* Clang-17

#### Optional tools:
* include-what-you-use 0.21
  * Enable it with `-DCMAKE_MELINDA_ENABLE_IWYU=ON`
  * Official release of iwyu 0.21 crashes when running `mqlprs` component due to a bug fixed in iwyu 0.22. Backport available [here](https://github.com/jan-kelemen/include-what-you-use/tree/clang_17)
* clang-format-17
  * Enable it with `-DCMAKE_MELINDA_ENABLE_CLANG_FORMAT=ON`

#### Required third party dependencies
* See cmake/dependencies.cmake for up to date information
```
find_package(Boost REQUIRED COMPONENTS iostreams program_options)
find_package(Catch2 CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(flatbuffers CONFIG REQUIRED)
find_package(lexy REQUIRED)
find_package(tl-expected REQUIRED)
```

# Building with vcpkg
```
cmake -B [build directory] -S . -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build [build directory] -j16
```

# Testing
* Run `scripts/test.sh`
  * Script supports optional testing directory parameter (`-d`), specify the filesystem location used for tests, defaults to value of `MELINDA_TEST_DIRECTORY` or `/tmp` if not set
