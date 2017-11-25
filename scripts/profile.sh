#!/bin/bash

source "scripts/messages.sh"

EXECUTABLE=$1
if [ -z "${EXECUTABLE}" ] 
then
        Error "Please, specify the executable as first parameter. Remember it must be compiled with RelWithDebInfo build type."
        exit 1
fi

if [ ! -f "${EXECUTABLE}" ] 
then
        Error "Executable file '${EXECUTABLE}' could not be found."
        exit 2
fi
shift

EXEC_DIR=$(dirname ${EXECUTABLE})
echo "${EXEC_DIR}"
${EXECUTABLE} ; \
 google-pprof --text ${EXECUTABLE} ${EXEC_DIR}/profiling_info.out > prof.txt ; \
 google-pprof --pdf ${EXECUTABLE} ${EXEC_DIR}/profiling_info.out > prof.pdf

Success "Result written to 'prof.txt'"
Success "Result written to 'prof.pdf'"
