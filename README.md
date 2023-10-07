# melinda
Melinda - Relational Database Management System

# Develpment environment setup
#### Required build tools:
* CMake 3.27

#### Supported compilers:
* gcc-13
* clang-16

#### Optional tools:
* include-what-you-use 0.20
  * Enable it with `-DCMAKE\_MELINDA\_ENABLE\_IWYU=ON`
* clang-format-16
  * Enable it with `-DCMAKE\_MELINDA\_ENABLE\_CLANG_FORMAT=ON`

#### Required third party dependencies
* See cmake/dependencies.cmake for up to date information
```
find_package(Catch2 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(date CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams program_options)
find_package(cppzmq CONFIG REQUIRED)
find_package(flatbuffers CONFIG REQUIRED)
find_package(lexy REQUIRED)
```

# Building with vcpkg
* `cmake -B [build directory] -S . -DCMAKE\_TOOLCHAIN\_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake -DCMAKE\_BUILD\_TYPE=Release`
* `cmake --build [build directory] -j16`

# Testing
* Run scripts/test.sh
  * Script supports optional testing directory parameter (-d), specify the filesystem location used for tests, defaults to value of MELINDA\_TEST\_DIRECTORY or /tmp if not set
