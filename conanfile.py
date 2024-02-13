from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps, CMakeToolchain

class DanaConan(ConanFile):
    name = "melinda"
    settings = "os", "compiler", "build_type", "arch"
    build_policy = "missing"

    def requirements(self):
        self.requires("foonathan-lexy/2022.12.1")
        self.requires("tl-expected/20190710")

    def build_requirements(self):
        self.test_requires("catch2/3.5.2")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        cmake = CMakeDeps(self)
        cmake.generate()

