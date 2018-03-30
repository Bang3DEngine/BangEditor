#!/bin/bash

cd "$(dirname "$0")"

mkdir -p build
cd build
PATH=${PATH}:/usr/lib/x86_64-linux-gnu/ ; cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF ..
if [ $? -ne 0 ] ; then exit 1 ; fi

make VERBOSE=1 -j6
if [ $? -ne 0 ] ; then exit 1 ; fi

cd ..
tar -czvf BangEditor.tar.gz BangEditor/
if [ $? -ne 0 ] ; then exit 1 ; fi
