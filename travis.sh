#!/bin/bash

pushd "source" && . debug.sh && . test.sh && popd

pushd "source" && . release.sh && . test.sh && popd

