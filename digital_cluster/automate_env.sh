#!/bin/bash
export $(grep -v '^#' .env | xargs)
set -e
rm -rf build
mkdir build
cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make 
# export QT_WAYLAND_DISABLE_TABLET=1
# export QT_WAYLAND_DISABLE_CLIPBOARD=1
# export QT_QPA_PLATFORM=xcb
LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 
valgrind ./digital_cluster
# export QT_LOGGING_RULES="qt5.*=true;qt5.wayland=true"
# GSETTINGS_BACKEND=memory QT_WAYLAND_DISABLE_KEYBOARD=1
# valgrind --leak-check=full --track-origins=yes 
 