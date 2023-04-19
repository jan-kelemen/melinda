# melinda
Melinda - Relational Database Management System

# Develpment environment setup
#### Required packages:
* python3
* python3-pip
* python3-setuptools
* cmake

#### Supported compilers:
* GCC-12
* Clang-16

#### Steps:
* Install required packages
* Install conan, https://docs.conan.io/en/latest/installation.html

# Building
* Run scripts/build.sh
  * Script supports optional configuration parameter (-c) with options Debug, RelWithDebInfo, Release

# Testing
* Run scripts/test.sh
  * Script supports optional testing directory parameter (-d), specify the filesystem location used for tests, defaults to value of MELINDA\_TEST\_DIRECTORY or /tmp if not set
