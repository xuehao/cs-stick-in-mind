#!/bin/bash
set -e

cd build
./build.sh
cd ..
./build/main
