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
set(BANG_EDITOR_LIBRARIES_DIR "${BANG_EDITOR_ROOT}/Libraries")
set(BANG_EDITOR_BINARIES_DIR  "${BANG_EDITOR_ROOT}/Binaries")
set(BANG_ENGINE_ROOT          "${BANG_EDITOR_ROOT}/Bang")
set(BANG_ENGINE_LIBRARIES_DIR "${BANG_ENGINE_ROOT}/Libraries")
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
                              "${BANG_EDITOR_ROOT}/Build/BuildPackage/CMakeLists.txt")
add_misc_files(${OTHER_FILES})
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# BangEngine sources =============================================
#=================================================================
set(EDITOR_SRC_FILES
${SRC_DIR}/Behaviours/BehaviourCreator.cpp
${SRC_DIR}/Behaviours/BehaviourTracker.cpp
${SRC_DIR}/Behaviours/EditorBehaviourManager.cpp
${SRC_DIR}/Console/Console.cpp
${SRC_DIR}/Editor.cpp
${SRC_DIR}/EditorApplication.cpp
${SRC_DIR}/EditorCamera.cpp
${SRC_DIR}/EditorClipboard.cpp
${SRC_DIR}/EditorDialog.cpp
${SRC_DIR}/EditorFileTracker.cpp
${SRC_DIR}/EditorIconManager.cpp
${SRC_DIR}/EditorMeshFactory.cpp
${SRC_DIR}/EditorPaths.cpp
${SRC_DIR}/EditorResources.cpp
${SRC_DIR}/EditorSettings.cpp
${SRC_DIR}/EditSceneGameObjects.cpp
${SRC_DIR}/EditorWindow.cpp
${SRC_DIR}/Explorer/Explorer.cpp
${SRC_DIR}/Explorer/ExplorerItem.cpp
${SRC_DIR}/Explorer/ExplorerItemFactory.cpp
${SRC_DIR}/Explorer/ModelExplorerItem.cpp
${SRC_DIR}/Explorer/PrefabExplorerItem.cpp
${SRC_DIR}/Hierarchy/Hierarchy.cpp
${SRC_DIR}/Hierarchy/HierarchyItem.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/ComponentInspectorWidget.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/ComponentInspectorWidgetFactory.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWAudioListener.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWAudioSource.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWBehaviourContainer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWCamera.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWDirectionalLight.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWLight.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWMeshRenderer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWPointLight.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWPostProcessEffect.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWRectTransform.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWRenderer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWTransform.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWUIImageRenderer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWUITextRenderer.cpp
${SRC_DIR}/Inspector/FileInspectorWidgets/FileInspectorWidget.cpp
${SRC_DIR}/Inspector/FileInspectorWidgets/FileInspectorWidgetFactory.cpp
${SRC_DIR}/Inspector/FileInspectorWidgets/FIWMaterial.cpp
${SRC_DIR}/Inspector/FileInspectorWidgets/FIWTexture.cpp
${SRC_DIR}/Inspector/Inspector.cpp
${SRC_DIR}/Inspector/InspectorWidget.cpp
${SRC_DIR}/Inspector/UIInputColor.cpp
${SRC_DIR}/Inspector/UIInputFile.cpp
${SRC_DIR}/Inspector/UIInputVector.cpp
${SRC_DIR}/GameBuilder.cpp
${SRC_DIR}/Project/Project.cpp
${SRC_DIR}/Project/ProjectManager.cpp
${SRC_DIR}/Project/QtProjectManager.cpp
${SRC_DIR}/Scenes/EditorSceneManager.cpp
${SRC_DIR}/Scenes/EditorScene.cpp
${SRC_DIR}/Scenes/ScenePlayer.cpp
${SRC_DIR}/Scenes/SceneOpenerSaver.cpp
${SRC_DIR}/UIScene/UISceneContainer.cpp
${SRC_DIR}/UIScene/UISceneDebugStats.cpp
${SRC_DIR}/UIScene/UISceneEditContainer.cpp
${SRC_DIR}/UIScene/UISceneImage.cpp
${SRC_DIR}/UIScene/UIScenePlayContainer.cpp
${SRC_DIR}/UIScene/UISceneToolbar.cpp
${SRC_DIR}/SelectionGizmos/ComponentsSelectionGizmo.cpp
${SRC_DIR}/SelectionGizmos/SelectionGizmo.cpp
${SRC_DIR}/SelectionGizmos/SelectionGizmosManager.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/RotateGizmo.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/RotateGizmoAxis.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/ScaleGizmo.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/ScaleGizmoAxis.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/TransformGizmo.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/TransformGizmoAxis.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/TranslateGizmo.cpp
${SRC_DIR}/SelectionGizmos/TransformGizmos/TranslateGizmoAxis.cpp
${SRC_DIR}/SelectionGizmos/RectTransformGizmos/RectTransformAnchorSelectionGizmo.cpp
${SRC_DIR}/SelectionGizmos/RectTransformGizmos/RectTransformCornerSelectionGizmo.cpp
${SRC_DIR}/SelectionGizmos/RectTransformGizmos/RectTransformSelectionGizmo.cpp
${SRC_DIR}/ShortcutManager.cpp
${SRC_DIR}/MenuBar.cpp
${SRC_DIR}/MenuItem.cpp
${SRC_DIR}/UIContextMenu.cpp
${SRC_DIR}/UITabContainer.cpp
${SRC_DIR}/UITabHeader.cpp
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Executables ====================================================
#=================================================================
add_library(BangEditorObjects OBJECT ${EDITOR_SRC_FILES})

set(CMAKE_POSITION_INDEPENDENT_CODE OFF)

add_executable(Game ${SRC_DIR}/MainGame.cpp)
add_executable(BangEditor $<TARGET_OBJECTS:BangEditorObjects> ${SRC_DIR}/Main.cpp)

add_bang_compilation_flags(BangEditorObjects)
add_bang_compilation_flags(BangEditor)
add_bang_compilation_flags(Game)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT DEFINED FORCE_NO_PIE)
    set(FORCE_NO_PIE ON)
endif()
if (${FORCE_NO_PIE})
    set_target_properties(Game       PROPERTIES LINK_FLAGS "-no-pie")
    set_target_properties(BangEditor PROPERTIES LINK_FLAGS "-no-pie")
endif()

get_target_property(BANG_ENGINE_INCLUDE_DIRS_0 BangEngineObjects INCLUDE_DIRECTORIES)
get_target_property(BANG_ENGINE_INCLUDE_DIRS_1 BangLib           INCLUDE_DIRECTORIES)
set(BANG_ENGINE_INCLUDE_DIRS "${BANG_ENGINE_INCLUDE_DIRS_0} ${BANG_ENGINE_INCLUDE_DIRS_1}")
target_compile_definitions(Game              PRIVATE -DBANG_ENGINE_ROOT="${BANG_ENGINE_ROOT_DEFINE}")
target_compile_definitions(Game              PRIVATE -DBANG_EDITOR_ROOT="${BANG_EDITOR_ROOT_DEFINE}")
target_compile_definitions(BangEditor        PRIVATE -DBANG_ENGINE_ROOT="${BANG_ENGINE_ROOT_DEFINE}")
target_compile_definitions(BangEditor        PRIVATE -DBANG_EDITOR_ROOT="${BANG_EDITOR_ROOT_DEFINE}")
target_compile_definitions(BangEditorObjects PRIVATE -DBANG_ENGINE_ROOT="${BANG_ENGINE_ROOT_DEFINE}")
target_compile_definitions(BangEditorObjects PRIVATE -DBANG_EDITOR_ROOT="${BANG_EDITOR_ROOT_DEFINE}")
target_include_directories(Game              PRIVATE ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
target_include_directories(BangEditor        PRIVATE ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
target_include_directories(BangEditorObjects PRIVATE ${BANG_ENGINE_INCLUDE_DIRS} ${BANG_EDITOR_INCLUDE_DIR})
target_link_libraries(Game       PRIVATE BangLib)
target_link_libraries(BangEditor PRIVATE BangLib)
#=================================================================
#=================================================================
#=================================================================
