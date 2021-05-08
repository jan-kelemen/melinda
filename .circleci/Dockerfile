FROM jankelemen/cpp-build-environment:0.1.0

LABEL maintainer="Jan Kelemen" \
      description="Build environment for melinda" \
      version="0.1.1"

ARG DEBIAN_FRONTEND=noninteractive

# Bincrafters remote now needs revisions enabled in conan
RUN conan config set general.revisions_enabled=1

