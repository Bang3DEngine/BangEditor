#!/bin/bash

BIN_FILEPATH=$1
OUTPUT_DIRECTORY=$2
if [ -z "${BIN_FILEPATH}" ] || [ -z "${OUTPUT_DIRECTORY}" ]
then
	echo "Error. Please:"
	echo " - 1st parameter: Binary filepath."
	echo " - 2nd parameter: Output directory."
        exit 1
fi

if [ ! -f "${BIN_FILEPATH}" ]
then
	echo "Error. Binary file '${BIN_FILEPATH}' was not found."
	exit 2
fi

if [ ! -d "${OUTPUT_DIRECTORY}" ]
then
	echo "Error. Output directory '${OUTPUT_DIRECTORY}' was not found."
	exit 3
fi

DEPS_LIST=$(ldd "${BIN_FILEPATH}" | sed 's/.*=>//g' | sed 's/(0x.*//g' | xargs)
cp -f ${DEPS_LIST} ${OUTPUT_DIRECTORY}
echo "${DEPS_LIST}"
