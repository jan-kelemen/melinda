FROM ubuntu:focal

LABEL maintainer="Jan Kelemen" \
      description="C++ build environment for cpp-starter-template" \
      version="0.1.0"

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update -y

# Add machine prerequisites
RUN apt install -y --no-install-recommends \ 
    # For add-apt-repository
    software-properties-common \ 
    # For wget
    wget \
    # For gpg-agent
    gpg-agent \
    # For make
    build-essential

# Add apt sources
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add
RUN add-apt-repository 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-11 main'

RUN apt-get update -y

# Install prerequisites
RUN apt install -y --no-install-recommends \
    git \
    ssh \
    # Prerequisites for conan
    python3 \
    python3-pip \
    python3-setuptools

# Install compilers
RUN apt install -y --no-install-recommends \
    gcc-10 g++-10 \
    clang-11

# Install build tools
RUN apt install -y --no-install-recommends \
    cmake \
    clang-format-11 clang-tidy-11

# Cleanup the packages to reduce the size of the image
RUN apt autoclean && \ 
    apt autoremove && \
    apt clean

RUN rm -rf /var/lib/apt/lists/

# Install conan
RUN pip3 install conan

