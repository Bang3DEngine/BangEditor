#!/bin/bash

cd "$(dirname "$0")"
cd ..

#Path to include-what-you-use directory
IWYU_DIR=$1

# Before:
# sudo apt-get install iwyu 

mkdir -p buildIWYU
cd buildIWYU

echo "Executing cmake to export compile commands..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .

echo "Executing IWYU, this may take several minutes...Check output doing 'tail -f $(pwd)/iwyuout.txt"
python ${IWYU_DIR}/iwyu_tool.py -j4 -p . -- --check_also=*.h > ./iwyuout.txt

# If you want to fix the includes:
echo "Fixing includes using 'fix_includes.py'..."
python ${IWYU_DIR}/fix_includes.py -b --nocomments < ./iwyuout.txt
