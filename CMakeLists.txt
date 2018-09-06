#=================================================================
# CMake options ==================================================
#=================================================================
cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE on)
project(BangEditor C CXX)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Include guard ==================================================
#=================================================================
if (DEFINED BANG_EDITOR_BUILT)
    return()
endif()
set(BANG_EDITOR_BUILT ON)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Check input params and set some paths ==========================
#=================================================================
set(BANG_EDITOR_ROOT          "${CMAKE_CURRENT_LIST_DIR}")
set(BANG_EDITOR_LIBRARIES_DIR "${BANG_EDITOR_ROOT}/Libraries/${CMAKE_BUILD_TYPE}")
set(BANG_EDITOR_BINARIES_DIR  "${BANG_EDITOR_ROOT}/Binaries")
set(BANG_ENGINE_ROOT          "${BANG_EDITOR_ROOT}/Bang")
set(BANG_ENGINE_LIBRARIES_DIR "${BANG_ENGINE_ROOT}/Libraries/${CMAKE_BUILD_TYPE}")
set(BANG_ENGINE_INCLUDE_DIR   "${BANG_ENGINE_ROOT}/include")
set(EXECUTABLE_OUTPUT_PATH    "${BANG_EDITOR_BINARIES_DIR}")
set(LIBRARY_OUTPUT_PATH       "${BANG_EDITOR_LIBRARIES_DIR}")

if (NOT DEFINED BANG_ENGINE_ROOT_DEFINE)
    file(RELATIVE_PATH BANG_ENGINE_ROOT_DEFINE ${EXECUTABLE_OUTPUT_PATH} ${BANG_ENGINE_ROOT})
endif()

if (NOT DEFINED BANG_EDITOR_ROOT_DEFINE)
    file(RELATIVE_PATH BANG_EDITOR_ROOT_DEFINE ${EXECUTABLE_OUTPUT_PATH} ${BANG_EDITOR_ROOT})
endif()

include(${BANG_ENGINE_ROOT}/Build/CMakeIncludes/CMakeCheckInputParams.cmake REQUIRED)
include(${BANG_ENGINE_ROOT}/Build/CMakeIncludes/CMakeMacros.cmake REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Bang CMake =====================================================
#=================================================================
add_subdirectory("${BANG_ENGINE_ROOT}" "${CMAKE_CURRENT_BINARY_DIR}/buildEditor")
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# General variables ==============================================
#=================================================================
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(EDITOR_SRC_DIR "${SRC_DIR}/Editor")
set(BANG_EDITOR_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(LIBRARY_OUTPUT_PATH ${BANG_EDITOR_LIBRARIES_DIR})

# Output params
message("BangEditor ================================")
message("BUILD_SHARED_LIBS:      ${BUILD_SHARED_LIBS}")
message("CMAKE_BUILD_TYPE:       ${CMAKE_BUILD_TYPE}")
message("CMAKE_BINARY_DIR:       ${CMAKE_BINARY_DIR}")
message("EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")
message("LIBRARY_OUTPUT_PATH:    ${LIBRARY_OUTPUT_PATH}")
message("PROJECT_SOURCE_DIR:     ${PROJECT_SOURCE_DIR}")
message("SRC_DIR:                ${SRC_DIR}")
message("===========================================")

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DGPROF)
    link_libraries("profiler") # gperftools
endif()

#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Add misc files =================================================
#=================================================================
file(GLOB_RECURSE OTHER_FILES "${BANG_EDITOR_INCLUDE_DIR}/BangEditor/*.h"
                              "${BANG_EDITOR_ROOT}/Assets/Shaders/*.frag"
                              "${BANG_EDITOR_ROOT}/Assets/Shaders/*.geom"
                              "${BANG_EDITOR_ROOT}/Assets/Shaders/*.vert"
                              "${BANG_EDITOR_ROOT}/Assets/Shaders/*.glsl"
                              "${BANG_EDITOR_ROOT}/Build/BuildPackage/CMakeLists.txt")
add_misc_files(${OTHER_FILES})
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# BangEngine sources =============================================
#=================================================================
file(GLOB_RECURSE EDITOR_SRC_FILES "${EDITOR_SRC_DIR}/*.cpp")
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Executables ====================================================
#=================================================================
add_library(BangEditorObjects OBJECT ${EDITOR_SRC_FILES} ${OTHER_FILES})

set (CMAKE_POSITION_INDEPENDENT_CODE OFF)

set (COMPILE_GAME OFF)
if (${COMPILE_GAME})
    add_executable(GameTemplate ${SRC_DIR}/MainGame.cpp)
endif()
add_library(BangEditorLib $<TARGET_OBJECTS:BangEditorObjects>)
add_executable(BangEditor ${SRC_DIR}/Main.cpp)

add_bang_compilation_flags(BangEditorObjects)
add_bang_compilation_flags(BangEditorLib)
add_bang_compilation_flags(BangEditor)
if (${COMPILE_GAME})
    add_bang_compilation_flags(GameTemplate)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT DEFINED FORCE_NO_PIE)
    set(FORCE_NO_PIE ON)
endif()
if (${FORCE_NO_PIE})
    if (${COMPILE_GAME})
        set_target_properties(GameTemplate PROPERTIES LINK_FLAGS "-no-pie")
    endif()
    set_target_properties(BangEditorLib PROPERTIES LINK_FLAGS "-no-pie")
endif()

get_target_property(BANG_ENGINE_INCLUDE_DIRS_0 BangEngineObjects INCLUDE_DIRECTORIES)
get_target_property(BANG_ENGINE_INCLUDE_DIRS_1 BangLib           INCLUDE_DIRECTORIES)
set(BANG_ENGINE_INCLUDE_DIRS "")
list(APPEND BANG_ENGINE_INCLUDE_DIRS "${BANG_ENGINE_INCLUDE_DIRS_0}")
list(APPEND BANG_ENGINE_INCLUDE_DIRS "${BANG_ENGINE_INCLUDE_DIRS_1}")
list(APPEND BANG_ENGINE_INCLUDE_DIRS "${DEPENDENCIES_INCLUDE_DIRS}")

# Inspector lib
set(BANG_INSPECTOR_LIB_SOURCES "")
file(GLOB_RECURSE BANG_INSPECTOR_LIB_SOURCES "${EDITOR_SRC_DIR}/Inspector/*.cpp")
add_library(BangInspectorLib ${BANG_INSPECTOR_LIB_SOURCES})
target_include_directories(BangInspectorLib PUBLIC ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
add_bang_compilation_flags(BangInspectorLib)
remove_from_list_another_list(EDITOR_SRC_FILES ${BANG_INSPECTOR_LIB_SOURCES})

# Gizmos lib
set(BANG_GIZMOS_LIB_SOURCES "")
file(GLOB_RECURSE BANG_GIZMOS_LIB_SOURCES "${EDITOR_SRC_DIR}/Gizmos/*.cpp")
add_library(BangGizmosLib ${BANG_GIZMOS_LIB_SOURCES})
target_include_directories(BangGizmosLib PUBLIC ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
add_bang_compilation_flags(BangGizmosLib)

# UndoRedo lib
set(BANG_UNDO_REDO_LIB_SOURCES "")
file(GLOB_RECURSE BANG_UNDO_REDO_LIB_SOURCES "${EDITOR_SRC_DIR}/UndoRedo/*.cpp")
add_library(BangUndoRedoLib ${BANG_UNDO_REDO_LIB_SOURCES})
target_include_directories(BangUndoRedoLib PUBLIC ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
add_bang_compilation_flags(BangUndoRedoLib)
remove_from_list_another_list(EDITOR_SRC_FILES ${BANG_UNDO_REDO_LIB_SOURCES})

target_compile_definitions(BangEditor        PRIVATE -DBANG_ENGINE_ROOT="${BANG_ENGINE_ROOT_DEFINE}")
target_compile_definitions(BangEditor        PRIVATE -DBANG_EDITOR_ROOT="${BANG_EDITOR_ROOT_DEFINE}")
target_include_directories(BangEditor        PRIVATE ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
target_compile_definitions(BangEditorObjects PRIVATE -DBANG_ENGINE_ROOT="${BANG_ENGINE_ROOT_DEFINE}")
target_compile_definitions(BangEditorObjects PRIVATE -DBANG_EDITOR_ROOT="${BANG_EDITOR_ROOT_DEFINE}")
target_include_directories(BangEditorObjects PRIVATE ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})

if (${COMPILE_GAME})
    target_compile_definitions(GameTemplate PRIVATE -DBANG_ENGINE_ROOT="${BANG_ENGINE_ROOT_DEFINE}")
    target_compile_definitions(GameTemplate PRIVATE -DBANG_EDITOR_ROOT="${BANG_EDITOR_ROOT_DEFINE}")
    target_include_directories(GameTemplate PRIVATE ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
    target_link_libraries(GameTemplate PRIVATE ${DEPENDENCIES_LIBS})
    target_link_libraries(GameTemplate PRIVATE BangLib)
endif()

add_dependencies(BangEditorObjects BangLib BuildDependencies)
target_link_libraries(BangEditorLib PUBLIC BangInspectorLib)
target_link_libraries(BangEditorLib PUBLIC BangUndoRedoLib)
target_link_libraries(BangEditorLib PUBLIC BangGizmosLib)
target_link_libraries(BangEditor PUBLIC ${DEPENDENCIES_LIBS})
target_link_libraries(BangEditor PUBLIC BangLib)
target_link_libraries(BangEditor PUBLIC BangEditorLib)
add_clang_tidy(BangEditorObjects)
add_clang_tidy(BangEngineObjects)

#=================================================================
#=================================================================
#=================================================================
