# melinda
Melinda - Relational Database Management System

# Develpment environment setup
#### Required packages:
* cmake

#### Required third party dependencies
* See cmake/dependencies.cmake for up to date information
```
find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)
find_package(date REQUIRED)
find_package(Boost REQUIRED COMPONENTS iostreams program_options)
find_package(cppzmq REQUIRED)
find_package(flatbuffers REQUIRED)
find_package(lexy REQUIRED)
```

#### Supported compilers:
* GCC-13
* Clang-16

# Building with vcpkg
* cmake -B [build directory] -S . -DCMAKE\_TOOLCHAIN\_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake -DCMAKE\_BUILD\_TYPE=Release
* cmake --build [build directory] -j16

# Testing
* Run scripts/test.sh
  * Script supports optional testing directory parameter (-d), specify the filesystem location used for tests, defaults to value of MELINDA\_TEST\_DIRECTORY or /tmp if not set
