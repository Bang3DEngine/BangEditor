#!/bin/bash

cd "$(dirname "$0")"

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF ..
make -j6

tar -czvf BangEditorPackage.tar.gz BangEditorPackage/
