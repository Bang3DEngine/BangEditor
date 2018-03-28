#=================================================================
# CMake options ==================================================
#=================================================================
cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE on)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Include guard ==================================================
#=================================================================
if (DEFINED BANG_EDITOR_PKG_BUILT)
    return()
endif()
set(BANG_EDITOR_PKG_BUILT ON)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Set variables (I) ==============================================
#=================================================================
set(EDITOR_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
if (NOT ${BUILD_SHARED_LIBS})
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
endif()
include(${EDITOR_ROOT}/CMakeIncludes/CMakeSetBangProjectRoot.cmake REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Check input params =============================================
#=================================================================
include(${CMAKE_CURRENT_LIST_DIR}/../../CMakeIncludes/CMakeCheckInputParams.cmake REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Build Bang package =============================================
#=================================================================
set(PKG_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BangEditorPackage")
set(BANG_PROJECT_ROOT_DEFINE ".")
include(${BANG_PROJECT_ROOT}/Scripts/BuildPackage/CMakeLists.txt REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Build BangEditor ===============================================
#=================================================================
set(CMAKE_BINARY_DIR ${PKG_DIRECTORY})
set(EXECUTABLE_OUTPUT_PATH ${PKG_DIRECTORY})

if (${BUILD_SHARED_LIBS})
    set(LIBRARY_OUTPUT_DIRECTORY_NAME "Libraries")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PKG_DIRECTORY}/${LIBRARY_OUTPUT_DIRECTORY_NAME}")
    set(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

    # Set RPATH! Important so that the executables search for shared libs
    # in the correct relative directory when running
    set(RELATIVE_RPATH "$ORIGIN/${LIBRARY_OUTPUT_DIRECTORY_NAME}:$ORIGIN")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH ${RELATIVE_RPATH})
else()

endif()

# Build editor !
include(${EDITOR_ROOT}/CMakeLists.txt REQUIRED)
#=================================================================
#=================================================================
#=================================================================

project(BuildPackageBangEditor C CXX)

#=================================================================
# Copy dependencies ==============================================
#=================================================================
if (${BUILD_SHARED_LIBS})
    set(BANG_EDITOR_EXE_PATH ${EXECUTABLE_OUTPUT_PATH}/BangEditor)
    add_custom_command(
      OUTPUT COPY_BANG_EDITOR_PKG_DEPS
      COMMAND /bin/sh ${CMAKE_CURRENT_LIST_DIR}/copyBinaryDependencies.sh
                      ${BANG_EDITOR_EXE_PATH} ${LIBRARY_OUTPUT_PATH}
      WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
      COMMENT "Copying dependencies of ${BANG_EDITOR_EXE_PATH} to ${LIBRARY_OUTPUT_PATH}..."
      VERBATIM
     )
    add_custom_target(COPY_BANG_EDITOR_PKG_DEPS_TGT ALL DEPENDS
        BangEditor Game
        COPY_BANG_EDITOR_PKG_DEPS)
endif()
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Copy editor directories ========================================
#=================================================================
# Assets
set(ENGINE_ASSETS_DIR_NAME "Assets")
set(ENGINE_ASSETS_DIR_SRC "${EDITOR_ROOT}/${ENGINE_ASSETS_DIR_NAME}")
set(ENGINE_ASSETS_DIR_DST "${PKG_DIRECTORY}")
add_custom_command(
  OUTPUT COPY_ASSETS_BANG_EDITOR_PKG_DIR
  COMMAND cp -r ${ENGINE_ASSETS_DIR_SRC} ${ENGINE_ASSETS_DIR_DST}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${ENGINE_ASSETS_DIR_SRC} to ${ENGINE_ASSETS_DIR_DST}..."
  VERBATIM
 )

# Includes
# set(ENGINE_INCLUDE_DIR_NAME "include")
# set(ENGINE_INCLUDE_DIR_SRC "${EDITOR_ROOT}/${ENGINE_INCLUDE_DIR_NAME}")
# set(ENGINE_INCLUDE_DIR_DST "${PKG_DIRECTORY}")
# add_custom_command(
#   OUTPUT COPY_INCLUDE_BANG_EDITOR_PKG_DIR
#   COMMAND cp -r ${ENGINE_INCLUDE_DIR_SRC} ${ENGINE_INCLUDE_DIR_DST}
#   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
#   COMMENT "Copying ${ENGINE_INCLUDE_DIR_SRC} to ${ENGINE_INCLUDE_DIR_DST}..."
#   VERBATIM
#  )
add_custom_target(COPY_EDITOR_PKG_DIRS ALL DEPENDS
    CREATE_PKG_DIRS
    COPY_ASSETS_BANG_EDITOR_PKG_DIR
    # COPY_INCLUDE_BANG_EDITOR_PKG_DIR
)
#=================================================================
#=================================================================
#=================================================================
