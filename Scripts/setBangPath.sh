#!/bin/bash

cd "$(dirname "$0")"
cd .. # Go to root directory

BANG_PATH_ROOT="$1"
if [ -z ${BANG_PATH_ROOT} ]
then
	echo "Error: Please specify as first parameter the full path to Bang. For example: '/home/user123/Bang'"
	exit 1
fi

if [ ! -d "${BANG_PATH_ROOT}" ]
then
	echo "Error: Bang path '${BANG_PATH_ROOT}' not found!"
	exit 2
fi

echo "set(BANG_PROJECT_ROOT \"${BANG_PATH_ROOT}\")" > "CMakeIncludes/CMakeConfig.txt"
