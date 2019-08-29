#!/usr/bin/env bash

mkdir -p build/dynamics
pushd build/dynamics

clang++ \
    -c \
    --target=wasm32 -nostdlib -O3 -DWEBASM -DSINGLE_THREAD -DBT_USE_DOUBLE_PRECISION -D__wasi__ -DNDEBUG -DNO_OPENGL3 \
    -std=c++11 \
    -fno-exceptions \
    -I../../../../src \
    -Wall \
    -I ../include \
    ../../../../src/BulletDynamics/**/*.cpp \
    

llvm-ar \
    rc ./dynamics.a \
    *.o

popd


