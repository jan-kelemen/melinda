# melinda [![GitHub Build status](https://github.com/jan-kelemen/melinda/workflows/GitHub%20CI/badge.svg)](https://github.com/jan-kelemen/melinda/actions?query=workflow%3A%22GitHub+CI%22)

Melinda - Relational Database Management System

## Building
Necessary build tools are:
* CMake 3.27 or higher
* Conan 2.0 or higher
  * See [installation instructions](https://docs.conan.io/2/installation.html)
* One of supported compilers:
  * Clang-17
  * GCC-13
  * Visual Studio 2022 (MSVC v193)

### Install dependencies
```
conan install . --profile=conan/clang-17 --build=missing --settings build_type=Release
```
* Conan profiles for supported compilers: `gcc-13`, `clang-17` and `msvc-2022`
* Conan build types: `Release`, `Debug`

### Configure
```
cmake --preset conan-release
```
* When compiling with `MSVC` use `conan-default` preset, in other cases use either `conan-debug` or `conan-release`
* Use the same preset as the build type specified in previous step

### Build
```
cmake --build --preset=conan-release
```
* Use the same preset as the build type specified when installing dependencies

## Testing
Building of tests is enabled by default. Execute the tests with the following command:
```
ctest --preset conan-release || ctest --preset conan-release --rerun-failed --output-on-failure
```

Disable building of tests by:
* Adding `--conf tools.build:skip_test=True` to `conan install` command
* Adding `-DBUILD_TESTING=OFF` during CMake configure

## Additional tools
### ClangFormat 
Enable running `clang-format` automatically on all source files during build by
adding `-DMELINDA_ENABLE_CLANG_FORMAT=ON` during CMake configure. This option is
disabled by default.

## Additional compilation options
### Toolchain hardening
Enable toolchain security hardening compiler options, by adding an additional
profile to the `conan install` command, together with `--build=*` to recompile
dependencies with hardening enabled. The hardening options should only be used
with `Release` build type. This option is disabled by default.
```
conan install . --profile=conan/clang-17 --profile=conan/opt/linux-hardening --build=* --settings build_type=Release
```
* Conan profiles for toolchain hardening are: `linux-hardening` and `msvc-hardening`

### Sanitizers
Enable sanitizers by adding an additional profile to the `conan install` command,
together with `--build=*` to recompile dependencies with santizers enabled. Sanitizers
should only be used with `Release` build type. These options are disabled by default.
```
conan install . --profile=conan/clang-17 --profile=conan/opt/linux-address-sanitizer --build=* --settings build_type=Release
```
* Conan profiles for `Clang` and `GCC` sanitizers are: `linux-address-sanitizer`, `linux-leak-sanitizer`, `linux-thread-sanitizer`, `linux-undefined-sanitizer`
  * Thread sanitizer cannot be used in combination with any other sanitizer
* Conan profile for `MSVC` compiler is: `msvc-address-sanitizer`
  * Run the compiled executables from the developer command prompt, or execute `call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64` to correctly set up search paths for runtime libraries

### Static analysis
#### ClangTidy
Enable running `clang-tidy` automatically on all source files during build by
adding `-DMELINDA_ENABLE_CLANG_TIDY=ON` during CMake configure. This option is
disabled by default.

#### Compiler static analysis
Enable compiler flags for static analysis during build by adding `-DMELINDA_ENABLE_COMPILER_STATIC_ANALYSIS=ON`
during CMake configure. This option is disabled by default.
* Compiler static analysis is supported for `GCC` and `MSVC` compilers

#### include-what-you-use
Enable running of `include-what-you-use` automatically on all source files during
build by adding `-DMELINDA_ENABLE_IWYU=ON` during CMake configure. This option is
disabled by default.

#### Cppcheck
Enable running of `cppcheck` automatically on all source files during build by 
adding `-DMELINDA_ENABLE_CPPCHECK=ON` during CMake configure. This option is disabled 
by default.
