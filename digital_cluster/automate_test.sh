#!/bin/bash
export $(grep -v '^#' .env | xargs)
set -e
# rm -rf build
# mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON .. # -DUSE_SANITIZER=ON
make 
# LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 ctest -V -R DigitalClusterTest -N
# gdb --args ./digital_cluster_test
# valgrind 
valgrind ./digital_cluster_test
#/lib/x86_64-linux-gnu/libasan.so.8
# valgrind --leak-check=full ./digital_cluster_test
# valgrind --leak-check=full ctest -V --rerun-failed --output-on-failure 