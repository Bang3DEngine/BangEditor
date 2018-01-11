cmake_minimum_required(VERSION 2.8)

project(BangEditor C CXX)

set(CMAKE_BINARY_DIR bin)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wl,-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=leak")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=float-divide-by-zero")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=integer-divide-by-zero")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=return")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=null")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wl,-O2")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PLEASE_SET_PATH_MSG "Please, set the correct Bang root path using \"scripts/set-bang-path.sh\"")

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/CMakeConfig.txt")
	message(FATAL_ERROR "Can't find \"CMakeConfig.txt\". ${PLEASE_SET_PATH_MSG}")
endif ()

include(CMakeConfig.txt REQUIRED)

# Compile Bang, if BANG_PROJECT_ROOT is set
if (NOT "${BANG_PROJECT_ROOT}" STREQUAL "")
	set(BANG_COMPILE_SCRIPT "${BANG_PROJECT_ROOT}/scripts/compile.sh")
        set(BANG_CLEAN_SCRIPT "${BANG_PROJECT_ROOT}/scripts/clean.sh")
	if (NOT EXISTS "${BANG_COMPILE_SCRIPT}")
	   message(FATAL_ERROR "Can't find ${BANG_COMPILE_SCRIPT} to compile Bang. ${PLEASE_SET_PATH_MSG}")
	endif ()
endif ()

message("Compiling Bang using ${BANG_COMPILE_SCRIPT}")
add_custom_target(CompileBang ALL "${BANG_COMPILE_SCRIPT}" "${CMAKE_BUILD_TYPE}")

set(SRC_DIR "src")

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_definitions(-DGPROF)
    link_libraries("profiler") # gperftools
endif()

set(INCLUDE_ROOT "${CMAKE_SOURCE_DIR}/include")
include_directories("${INCLUDE_ROOT}")
include_directories("${BANG_PROJECT_ROOT}/include")
add_definitions(-DBANG_PROJECT_ROOT="${BANG_PROJECT_ROOT}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wint-to-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--export-dynamic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

macro(add_info_files)
     foreach(_it ${ARGN})
         if(NOT IS_DIRECTORY ${_it})
             get_filename_component(name ${_it} NAME)
             if(NOT ${_it} MATCHES "^/\\\\..*$;~$")
                set_source_files_properties(${_it} PROPERTIES HEADER_FILE_ONLY TRUE)
             endif()
         endif()
     endforeach()
endmacro()
file(GLOB_RECURSE OTHER_FILES "${INCLUDE_ROOT}/BangEditor/*.h")
set(OTHER_FILES README.md "${OTHER_FILES}")
add_info_files(${OTHER_FILES})

message("Building BangEditor in ${CMAKE_BUILD_TYPE} mode...")
set(BANG_LIBS_DIR "${BANG_PROJECT_ROOT}/bin/${CMAKE_BUILD_TYPE}/lib")
set(BANG_LIB_NAME "libBang.so")
link_libraries("${BANG_LIBS_DIR}/${BANG_LIB_NAME}")

add_executable(BangEditor
${SRC_DIR}/Behaviours/BehaviourCreator.cpp
${SRC_DIR}/Behaviours/BehaviourTracker.cpp
${SRC_DIR}/Behaviours/EditorBehaviourManager.cpp
${SRC_DIR}/Console/Console.cpp
${SRC_DIR}/Editor.cpp
${SRC_DIR}/EditorApplication.cpp
${SRC_DIR}/EditorCamera.cpp
${SRC_DIR}/EditorComponentFactory.cpp
${SRC_DIR}/EditorDialog.cpp
${SRC_DIR}/EditorIconManager.cpp
${SRC_DIR}/EditorPaths.cpp
${SRC_DIR}/EditorSettings.cpp
${SRC_DIR}/EditorUITab.cpp
${SRC_DIR}/EditorWindow.cpp
${SRC_DIR}/Explorer/Explorer.cpp
${SRC_DIR}/Explorer/ExplorerItem.cpp
${SRC_DIR}/Hierarchy/Hierarchy.cpp
${SRC_DIR}/Hierarchy/HierarchyItem.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/ComponentInspectorWidget.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/ComponentInspectorWidgetFactory.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWBehaviourContainer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWCamera.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWDirectionalLight.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWLight.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWMeshRenderer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWPointLight.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWRenderer.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgets/CIWTransform.cpp
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
${SRC_DIR}/Scenes/SceneTab.cpp
${SRC_DIR}/Scenes/UISceneContainer.cpp
${SRC_DIR}/TransformGizmos/RotateGizmo.cpp
${SRC_DIR}/TransformGizmos/RotateGizmoAxis.cpp
${SRC_DIR}/TransformGizmos/ScaleGizmo.cpp
${SRC_DIR}/TransformGizmos/ScaleGizmoAxis.cpp
${SRC_DIR}/TransformGizmos/SelectionGizmo.cpp
${SRC_DIR}/TransformGizmos/TransformGizmo.cpp
${SRC_DIR}/TransformGizmos/TransformGizmoAxis.cpp
${SRC_DIR}/TransformGizmos/TranslateGizmo.cpp
${SRC_DIR}/TransformGizmos/TranslateGizmoAxis.cpp
${SRC_DIR}/MenuBar.cpp
${SRC_DIR}/MenuItem.cpp
${SRC_DIR}/UIContextMenu.cpp

${SRC_DIR}/Main.cpp

${OTHER_FILES}
)
add_dependencies(BangEditor CompileBang)

add_executable(Game ${SRC_DIR}/MainGame.cpp)
add_dependencies(Game CompileBang)

