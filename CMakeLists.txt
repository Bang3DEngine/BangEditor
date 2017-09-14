cmake_minimum_required(VERSION 2.8)

project(BangEditor C CXX)

set(CMAKE_BINARY_DIR bin)
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

include_directories(${BANG_PROJECT_ROOT}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
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

set(SRC_DIR "src")

message("Building BangEditor in ${CMAKE_BUILD_TYPE} mode...")
set(BANG_LIBS_DIR "${BANG_PROJECT_ROOT}/bin/${CMAKE_BUILD_TYPE}/lib")
set(BANG_LIB_NAME "libBang.so")
link_libraries("${BANG_LIBS_DIR}/${BANG_LIB_NAME}")

add_executable(BangEditor
${SRC_DIR}/EditorApplication.cpp
${SRC_DIR}/Main.cpp
)

