# melinda [![Build Status](https://circleci.com/gh/jan-kelemen/melinda.svg?style=shield)](https://app.circleci.com/pipelines/github/jan-kelemen/melinda)
Melinda - Relational Database Management System

# Develpment environment setup
#### Required packages:
* python3
* python3-pip
* python3-setuptools
* cmake

#### Supported compilers:
* GCC-11
* Clang-12

#### Steps:
* Install required packages
* Install conan, https://docs.conan.io/en/latest/installation.html

# Building [![Build Status](https://circleci.com/gh/jan-kelemen/melinda.svg?style=shield)](https://app.circleci.com/pipelines/github/jan-kelemen/melinda)
* Run scripts/build.sh
  * Script supports optional configuration parameter (-c) with options Debug, RelWithDebInfo, Release
  * Script supports optional setup parameter (-s) which sets up the dependencies only

