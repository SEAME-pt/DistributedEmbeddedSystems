#!/bin/bash
export $(grep -v '^#' .env | xargs)
# rm -rf build
# mkdir build
set -e
cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make 
# LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 
# valgrind --leak-check=full --track-origins=yes 
./digital_cluster