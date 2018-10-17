#!/bin/bash

cd "$(dirname "$0")"

for f in $(find ../include/ ../src/ ../Bang/include/ ../Bang/src -type f)
do
    echo "Formatting '$f'..."
    clang-format -i $f
done


