#!/bin/bash
set -e

cd $DEPS_DIR

git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

echo 'Install vcpkg succesfully'