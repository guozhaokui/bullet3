#!/bin/sh

if [ -e CMakeCache.txt ]; then
  rm CMakeCache.txt
fi
mkdir -p build_cmake
cd build_cmake
cmake -DBUILD_PYBULLET=OFF -DBUILD_PYBULLET_NUMPY=OFF -DUSE_DOUBLE_PRECISION=ON -DBT_USE_EGL=OFF -DCMAKE_BUILD_TYPE=Release  -DBUILD_CLSOCKET=OFF -DBUILD_OPENGL3_DEMOS=OFF  .. || exit 1
make -j $(command nproc 2>/dev/null || echo 12) || exit 1
echo "Completed build of Bullet."
