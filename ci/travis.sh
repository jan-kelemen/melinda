#!/bin/bash

. scripts/build.sh -c Debug && . scripts/test.sh

. scripts/build.sh -c RelWithDebInfo && . scripts/test.sh


