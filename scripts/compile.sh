#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

source "scripts/captureArgs.sh"
BT=${BUILD_TYPE}

VERBOSE=1
NUM_THREADS=6

CMAKE_CMD="cmake -DCMAKE_BUILD_TYPE=${BT} .."
if [ "${BANG_COMPILER}" != "" ]
then
        CMAKE_CMD="${CMAKE_CMD} -DCMAKE_CXX_COMPILER=${BANG_COMPILER}"
fi

echo "Compiling Editor..."
mkdir -p build${BT} && \
cd build${BT} && \
${CMAKE_CMD} && \
make all --no-print-directory VERBOSE=${VERBOSE} -j${NUM_THREADS}

if [[ $? != 0 ]]
then
        Error "ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
        EndHeader
        exit 3
fi

