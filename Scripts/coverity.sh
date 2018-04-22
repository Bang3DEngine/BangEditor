#!/bin/bash

cd "$(dirname "$0")"
cd ..

COV_TGZ="BangEditor_Coverity.tgz"
TMP_COMPILE_FILE=".tmp.compile.sh"

# Create build script
echo -e "#!/bin/bash \n\n mkdir -p buildCoverity \n cd buildCoverity \n cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON .. \n make -j6" > ${TMP_COMPILE_FILE}
chmod +x ${TMP_COMPILE_FILE}

# Build
cov-build --dir cov-int ./${TMP_COMPILE_FILE}
rm ${TMP_COMPILE_FILE}

# Tar the result
tar -czvf ${COV_TGZ} cov-int
rm -rf cov-int

# Upload:
#curl --form token=aFpZvYVCRnAJE4zuYkTKqw \
#     --form email=victorantondominguez@gmail.com \
#     --form file=@$(pwd)/${COV_TGZ} \
#     --form version="0.0" \
#     --form description="Automated_Coverity_Upload" \
#     https://scan.coverity.com/builds?project=Bang3DEngine%2FBangEditor

