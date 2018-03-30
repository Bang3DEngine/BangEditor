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
set(BANG_ENGINE_ROOT "${EDITOR_ROOT}/Bang")
set(PKG_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BangEditor")
file(MAKE_DIRECTORY ${PKG_DIRECTORY})
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Build Bang package =============================================
#=================================================================
include(${BANG_ENGINE_ROOT}/Scripts/BuildPackage/CMakeLists.txt REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Build BangEditor ===============================================
#=================================================================
set(CMAKE_BINARY_DIR ${PKG_DIRECTORY})
set(EXECUTABLE_OUTPUT_PATH ${PKG_DIRECTORY})

# Build editor !
include(${EDITOR_ROOT}/CMakeLists.txt REQUIRED)
#=================================================================
#=================================================================
#=================================================================

project(BuildPackageBangEditor C CXX)

#=================================================================
# Copy/Remove editor directories =================================
#=================================================================
# Assets Copy
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

# Remove useless directories
set(ENGINE_INCLUDE_DIR "${PKG_DIRECTORY}/include")
# LIBRARY_OUTPUT_PATH defined by bang build package CMake
add_custom_command(
  OUTPUT REMOVE_INCLUDE_BANG_EDITOR_PKG_DIR
  COMMAND rm -rf ${ENGINE_INCLUDE_DIR} ${LIBRARY_OUTPUT_DIRECTORY}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Removing ${ENGINE_INCLUDE_DIR} and ${LIBRARY_OUTPUT_DIRECTORY}..."
  VERBATIM
  DEPENDS COPY_DIRS CREATE_PKG_DIRS
)
add_custom_target(COPY_EDITOR_PKG_DIRS ALL DEPENDS
    COPY_ASSETS_BANG_EDITOR_PKG_DIR
    REMOVE_INCLUDE_BANG_EDITOR_PKG_DIR
)
#=================================================================
#=================================================================
#=================================================================
