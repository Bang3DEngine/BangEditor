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
set(EDITOR_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(BANG_ENGINE_ROOT "${EDITOR_ROOT}/Bang")
set(BANG_ENGINE_ROOT_DEFINE ".")
set(BANG_EDITOR_ROOT_DEFINE ".")
set(PKG_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/BangEditor")
file(MAKE_DIRECTORY ${PKG_DIRECTORY})
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Build Bang package =============================================
#=================================================================
include(${BANG_ENGINE_ROOT}/Build/CMakeLists.txt REQUIRED)
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
# Copy directories ===============================================
#=================================================================
set(ENGINE_ASSETS_DIR_NAME "Assets")
set(ENGINE_ASSETS_DIR_SRC "${EDITOR_ROOT}/${ENGINE_ASSETS_DIR_NAME}")
set(ENGINE_ASSETS_DIR_DST "${PKG_DIRECTORY}")
set(ENGINE_INCLUDE_DIR   "${PKG_DIRECTORY}/include")
set(ENGINE_LIBRARIES_DIR "${PKG_DIRECTORY}/Libraries")

# Assets Copy
add_custom_command(
  OUTPUT COPY_ASSETS_BANG_EDITOR_PKG_DIR
  COMMAND cp -r ${ENGINE_ASSETS_DIR_SRC} ${ENGINE_ASSETS_DIR_DST}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${ENGINE_ASSETS_DIR_SRC} to ${ENGINE_ASSETS_DIR_DST}..."
  VERBATIM
 )

# Include copy
# add_custom_command(
#   OUTPUT COPY_INCLUDE_BANG_EDITOR_PKG_DIR
#   COMMAND
#   ${CMAKE_COMMAND} -E copy ${ENGINE_INCLUDE_DIR} ${LIBRARY_OUTPUT_DIRECTORY}
#   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
#   COMMENT "Copying ${ENGINE_INCLUDE_DIR}"
#   VERBATIM
#   DEPENDS COPY_DIRS CREATE_PKG_DIRS
# )

# Libraries remove
# add_custom_command(
#   OUTPUT REMOVE_LIBRARIES_BANG_EDITOR_PKG_DIR
#   COMMAND
#   ${CMAKE_COMMAND} -E remove_directory ${ENGINE_LIBRARIES_DIR}
#   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
#   COMMENT "Removing ${ENGINE_LIBRARIES_DIR}"
#   VERBATIM
#   DEPENDS COPY_DIRS CREATE_PKG_DIRS
# )

# Libraries copy
set(EDITOR_LIBRARIES_DIR_NAME "Libraries")
set(EDITOR_LIBRARIES_DIR_SRC "${LIBRARY_OUTPUT_PATH}")
set(EDITOR_LIBRARIES_DIR_DST "${PKG_DIRECTORY}/${EDITOR_LIBRARIES_DIR_NAME}")
add_custom_command(
  OUTPUT COPY_LIBRARIES_BANG_EDITOR_PKG_DIR
  COMMAND
  ${CMAKE_COMMAND} -E copy ${EDITOR_LIBRARIES_DIR_SRC}/libbang_static.a ${EDITOR_LIBRARIES_DIR_DST}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${EDITOR_LIBRARIES_DIR_SRC} to ${EDITOR_LIBRARIES_DIR_DST}..."
  VERBATIM
)

add_custom_target(COPY_EDITOR_PKG_DIRS ALL
    DEPENDS BangEditor
    DEPENDS COPY_ASSETS_BANG_EDITOR_PKG_DIR
    #DEPENDS COPY_INCLUDE_BANG_EDITOR_PKG_DIR
    #DEPENDS REMOVE_LIBRARIES_BANG_EDITOR_PKG_DIR
    DEPENDS COPY_LIBRARIES_BANG_EDITOR_PKG_DIR
)


#=================================================================
#=================================================================
#=================================================================
