cmake_minimum_required(VERSION 2.8)

project(BangEditor C CXX)

include(CMakeConfig.txt OPTIONAL)

# Compile Bang, if BANG_PROJECT_ROOT is set
if(NOT "${BANG_PROJECT_ROOT}" STREQUAL "")
	set(BANG_INSTALL_SCRIPT "${BANG_PROJECT_ROOT}/scripts/install.sh")
	if(EXISTS "${BANG_INSTALL_SCRIPT}")
	   message("Installing Bang using ${BANG_INSTALL_SCRIPT}")
	   execute_process(COMMAND "${BANG_INSTALL_SCRIPT}")
	else()
	   message("Can't find ${BANG_INSTALL_SCRIPT} to install. Not installing Bang.")
	endif()
endif()

set(CMAKE_BINARY_DIR bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

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
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
	link_libraries("Bangd")
else ()
	link_libraries("Bang")
endif ()


add_executable(BangEditor
${SRC_DIR}/Main.cpp
)

