#!/bin/bash
export $(grep -v '^#' .env | xargs)
set -e
rm -rf build
mkdir build
cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make 
# LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 
valgrind  ./digital_cluster
# --leak-check=full --track-origins=yes