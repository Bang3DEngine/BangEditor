#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"
RD=${RELEASE_OR_DEBUG}

VERBOSE=1
NUM_THREADS=6

CMAKE_CMD="cmake -DCMAKE_BUILD_TYPE=${RD} .."
if [ "${BANG_COMPILER}" != "" ]
then
        CMAKE_CMD="${CMAKE_CMD} -DCMAKE_CXX_COMPILER=${BANG_COMPILER}"
fi

echo "Compiling Editor..."
mkdir -p build${RD} && \
cd build${RD} && \
${CMAKE_CMD} && \
make all --no-print-directory VERBOSE=${VERBOSE} -j${NUM_THREADS}


