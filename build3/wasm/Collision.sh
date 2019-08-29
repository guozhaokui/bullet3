#!/usr/bin/env bash

mkdir -p build/collision
pushd build/collision

clang++ \
    -c \
    --target=wasm32 -nostdlib -O3 -DWEBASM -DSINGLE_THREAD -DBT_USE_DOUBLE_PRECISION -D__wasi__ -DNDEBUG -DNO_OPENGL3 \
    -fno-exceptions \
    -std=c++11 \
    -I../../../../src \
    -Wall \
    -I ../include \
    ../../../../src/BulletCollision/**/*.cpp \
    

llvm-ar \
    rc ./Collision.a \
    *.o

popd


