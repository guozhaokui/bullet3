#!/usr/bin/env bash

mkdir -p build/linearmath
pushd build/linearmath

clang++ \
    -c \
    --target=wasm32 -nostdlib -O3 -DWEBASM -DBT_USE_DOUBLE_PRECISION -D__wasi__ -DNDEBUG -DNO_OPENGL3 \
    -fvisibility=hidden \
    -fno-threadsafe-statics \
    -mthread-model single \
    -fno-exceptions \
    -std=c++11 \
    -Wall \
    -I ../include \
    ../../../../src/LinearMath/*.cpp \
    

llvm-ar \
    rc ./LinearMath.a \
    *.o

popd


