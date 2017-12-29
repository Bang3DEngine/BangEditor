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
${SRC_DIR}/Behaviour.cpp
${SRC_DIR}/BehaviourManager.cpp
${SRC_DIR}/Console.cpp
${SRC_DIR}/Editor.cpp
${SRC_DIR}/EditorApplication.cpp
${SRC_DIR}/EditorCamera.cpp
${SRC_DIR}/EditorDialog.cpp
${SRC_DIR}/EditorIconManager.cpp
${SRC_DIR}/EditorPaths.cpp
${SRC_DIR}/EditorSceneManager.cpp
${SRC_DIR}/EditorScene.cpp
${SRC_DIR}/EditorSettings.cpp
${SRC_DIR}/EditorUITab.cpp
${SRC_DIR}/EditorWindow.cpp
${SRC_DIR}/Explorer/Explorer.cpp
${SRC_DIR}/Explorer/ExplorerItem.cpp
${SRC_DIR}/Hierarchy/Hierarchy.cpp
${SRC_DIR}/Hierarchy/HierarchyItem.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidget.cpp
${SRC_DIR}/Inspector/ComponentInspectorWidgetFactory.cpp
${SRC_DIR}/Inspector/CIWMeshRenderer.cpp
${SRC_DIR}/Inspector/CIWRenderer.cpp
${SRC_DIR}/Inspector/CIWTransform.cpp
${SRC_DIR}/Inspector/FileInspectorWidget.cpp
${SRC_DIR}/Inspector/FileInspectorWidgetFactory.cpp
${SRC_DIR}/Inspector/FIWMaterial.cpp
${SRC_DIR}/Inspector/FIWTexture.cpp
${SRC_DIR}/Inspector/Inspector.cpp
${SRC_DIR}/Inspector/InspectorWidget.cpp
${SRC_DIR}/Inspector/UIInputColor.cpp
${SRC_DIR}/Inspector/UIInputFile.cpp
${SRC_DIR}/Inspector/UIInputVector.cpp
${SRC_DIR}/GameBuilder.cpp
${SRC_DIR}/Project/Project.cpp
${SRC_DIR}/Project/ProjectManager.cpp
${SRC_DIR}/Project/QtProjectManager.cpp
${SRC_DIR}/RotateGizmo.cpp
${SRC_DIR}/RotateGizmoAxis.cpp
${SRC_DIR}/ScaleGizmo.cpp
${SRC_DIR}/ScaleGizmoAxis.cpp
${SRC_DIR}/SceneOpenerSaver.cpp
${SRC_DIR}/SelectionGizmo.cpp
${SRC_DIR}/TransformGizmo.cpp
${SRC_DIR}/TransformGizmoAxis.cpp
${SRC_DIR}/TranslateGizmo.cpp
${SRC_DIR}/TranslateGizmoAxis.cpp
${SRC_DIR}/MenuBar.cpp
${SRC_DIR}/MenuItem.cpp
${SRC_DIR}/UIContextMenu.cpp
${SRC_DIR}/UISceneContainer.cpp

${SRC_DIR}/Main.cpp

${OTHER_FILES}
)

