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
include(${BANG_ENGINE_ROOT}/Build/BuildPackage/CMakeLists.txt REQUIRED)
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
set(ASSETS_DIR_NAME "Assets")
set(INCLUDE_DIR_NAME "include")
set(LIBRARIES_DIR_NAME "Libraries")
set(BINARIES_DIR_NAME "Binaries")
set(ENGINE_ASSETS_DIR "${ENGINE_ROOT}/${ASSETS_DIR_NAME}")
set(EDITOR_ASSETS_DIR "${EDITOR_ROOT}/${ASSETS_DIR_NAME}")
set(ENGINE_INCLUDE_DIR   "${PKG_DIRECTORY}/${INCLUDE_DIR_NAME}")
set(ENGINE_LIBRARIES_DIR "${PKG_DIRECTORY}/${LIBRARIES_DIR_NAME}")
set(EDITOR_BINARIES_DIR "${EDITOR_ROOT}/${BINARIES_DIR_NAME}")
set(PKG_ASSETS_DIR "${PKG_DIRECTORY}/${ASSETS_DIR_NAME}")
set(PKG_BINARIES_DIR "${PKG_DIRECTORY}/${BINARIES_DIR_NAME}")

# Assets Copy
add_custom_command(
  OUTPUT COPY_PKG_ASSETS_EDITOR
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ENGINE_ASSETS_DIR} ${PKG_ASSETS_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${EDITOR_ASSETS_DIR} ${PKG_ASSETS_DIR}
  COMMAND ${CMAKE_COMMAND} -E remove -f "${PKG_ASSETS_DIR}/EditorSettings.txt"
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${ENGINE_ASSETS_DIR} and ${EDITOR_ASSETS_DIR} to ${PKG_ASSETS_DIRECTORY}..."
  VERBATIM
 )

# Binaries Copy
add_custom_command(
  OUTPUT COPY_PKG_BINARIES_EDITOR
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PKG_BINARIES_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${EDITOR_BINARIES_DIR} ${PKG_BINARIES_DIR}
  COMMAND ${CMAKE_COMMAND} -E rename ${PKG_BINARIES_DIR}/BangEditor ${PKG_DIRECTORY}/BangEditor
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${ENGINE_ASSETS_DIR} to ${PKG_ASSETS_DIR}..."
  VERBATIM
 )

# Include copy
# add_custom_command(
#   OUTPUT COPY_PKG_INCLUDES_EDITOR
#   COMMAND ${CMAKE_COMMAND} -E copy ${ENGINE_INCLUDE_DIR} ${LIBRARY_OUTPUT_DIRECTORY}
#   WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
#   COMMENT "Copying ${ENGINE_INCLUDE_DIR}"
#   VERBATIM
#   DEPENDS COPY_DIRS CREATE_PKG_DIRS
# )

# Libraries remove
# add_custom_command(
#   OUTPUT REMOVE_LIBRARIES_BANG_EDITOR_PKG_DIR
#   COMMAND ${CMAKE_COMMAND} -E remove_directory ${ENGINE_LIBRARIES_DIR}
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
  OUTPUT COPY_PKG_LIBRARIES_EDITOR
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${EDITOR_LIBRARIES_DIR_SRC} ${EDITOR_LIBRARIES_DIR_DST}
  WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
  COMMENT "Copying ${EDITOR_LIBRARIES_DIR_SRC} to ${EDITOR_LIBRARIES_DIR_DST}..."
  VERBATIM
)

add_custom_target(COPY_EDITOR_PKG_DIRS
    DEPENDS BangEditor
    DEPENDS COPY_PKG_ASSETS_EDITOR
    DEPENDS COPY_PKG_BINARIES_EDITOR
    #DEPENDS COPY_PKG_INCLUDES_EDITOR
    #DEPENDS REMOVE_LIBRARIES_BANG_EDITOR_PKG_DIR
    DEPENDS COPY_PKG_LIBRARIES_EDITOR
    DEPENDS PKG_TAR
)

# Tar everything
set (PKG_TAR_PATH "${CMAKE_CURRENT_LIST_DIR}/BangEditor.tar.gz")
add_custom_command(
  OUTPUT PKG_TAR
  COMMAND cd ${CMAKE_CURRENT_LIST_DIR} && ${CMAKE_COMMAND} -E tar "czvf" "${PKG_TAR_PATH}" "${PKG_DIRECTORY}"
  COMMENT "Packing package into a tar ${PKG_TAR_PATH}"
  DEPENDS COPY_EDITOR_PKG_DIRS
  VERBATIM
)

add_custom_target(CREATE_EDITOR_PKG ALL DEPENDS PKG_TAR)


#=================================================================
#=================================================================
#=================================================================
