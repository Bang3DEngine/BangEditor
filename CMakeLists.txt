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
set(EDITOR_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(BANG_PROJECT_ROOT "${EDITOR_ROOT}/Bang")
set(EDITOR_ROOT_RELATIVE_TO_EXEC "${EDITOR_ROOT}")
file(RELATIVE_PATH EDITOR_ROOT_RELATIVE_TO_EXEC ${CMAKE_BINARY_DIR} ${EDITOR_ROOT})
add_definitions(-DEDITOR_ROOT_RELATIVE_TO_EXEC="${EDITOR_ROOT_RELATIVE_TO_EXEC}")
include(${BANG_PROJECT_ROOT}/CMakeIncludes/CMakeCheckInputParams.cmake REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# CMake Includes =================================================
#=================================================================
include(${BANG_PROJECT_ROOT}/CMakeIncludes/CMakeMacros.cmake REQUIRED)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Bang CMake =====================================================
#=================================================================
include(${BANG_PROJECT_ROOT}/CMakeLists.txt)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# General variables ==============================================
#=================================================================
project(BangEditor C CXX)

set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(INCLUDE_ROOT "${CMAKE_CURRENT_LIST_DIR}/include")

# Extra make flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie")

# Output params
message("BangEditor ================================")
message("BUILD_SHARED_LIBS: " ${BUILD_SHARED_LIBS})
message("CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
message("EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH})
message("LIBRARY_OUTPUT_PATH: " ${LIBRARY_OUTPUT_PATH})
message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message("SRC_DIR: " ${SRC_DIR})
message("===========================================")

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DGPROF)
    link_libraries("profiler") # gperftools
endif()

#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Includes =======================================================
#=================================================================
include_directories("${INCLUDE_ROOT}")
include_directories("${BANG_PROJECT_ROOT}/include")
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Flags ==========================================================
#=================================================================
include(${BANG_PROJECT_ROOT}/CMakeIncludes/CMakeFlags.cmake)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
# Add misc files =================================================
#=================================================================
file(GLOB_RECURSE OTHER_FILES "${INCLUDE_ROOT}/BangEditor/*.h")
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

${SRC_DIR}/Main.cpp

${OTHER_FILES}
)
#=================================================================
#=================================================================
#=================================================================

#=================================================================
#Executables =====================================================
#=================================================================

link_libraries(BangLib ${ALL_DEPENDENCIES_LIB_FILES})

add_executable(BangEditor ${EDITOR_SRC_FILES})
add_dependencies(BangEditor BangLib)

if (${COMPILE_GAME})
    add_executable(Game ${SRC_DIR}/MainGame.cpp)
    add_dependencies(Game BangLib)
endif()

#=================================================================
#=================================================================
#=================================================================
