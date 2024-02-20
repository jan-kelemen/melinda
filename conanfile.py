from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

require_conan_version = ">=2.0"

class DanaConan(ConanFile):
    name = "melinda"
    settings = "os", "compiler", "build_type", "arch"
    license = "BSD-3-Clause"
    author = "Jan Kelemen"
    url = "https://github.com/jan-kelemen/melinda"
    version = "0.1"

    exports_sources = "cmake", "src", "CMakeLists.txt", "LICENSE"

    def requirements(self):
        self.requires("foonathan-lexy/2022.12.1")
        self.requires("tl-expected/20190710")

    def build_requirements(self):
        self.tool_requires("cmake/[^3.27]")
        self.test_requires("catch2/[^3.5.2]")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        cmake = CMakeDeps(self)
        cmake.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
