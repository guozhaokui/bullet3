#!/usr/bin/env bash

mkdir -p build/hello
pushd build/hello

clang++ -c\
    --target=wasm32 -nostdlib -O3 -DWEBASM -DSINGLE_THREAD -DBT_USE_DOUBLE_PRECISION -D__wasi__ -DNDEBUG -DNO_OPENGL3 \
    -mthread-model single \
    -fvisibility=hidden \
    -fno-exceptions \
    -std=c++11 \
    -I../../../../src/ \
    -Wall \
    ../../../../examples/HelloWorld/*.cpp \
    
#jslib.imports 中的符号在运行时连接
wasm-ld        \
    --lto-O3 --no-entry --export-dynamic\
    --allow-undefined-file=../../jslib.imports \
    --import-memory \
    ../collision/collision.a \
    ../dynamics/dynamics.a \
    ../linearmath/linearmath.a \
    F:/lib/wasm32-wasi/libc.a\
    F:/lib/wasm32-wasi/libc++.a\
    F:/lib/wasm32-wasi/libc++abi.a \
    F:/lib/wasm32-wasi/libc-printscan-long-double.a\
    *.o\
    -o hello.wasm

popd


