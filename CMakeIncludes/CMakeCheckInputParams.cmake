if (NOT DEFINED CMAKE_CHECK_INPUT_PARAMS_BUILT)

    set(CMAKE_CHECK_INPUT_PARAMS_BUILT ON)

    if ((NOT DEFINED BANG_PROJECT_ROOT) OR
        (NOT EXISTS "${BANG_PROJECT_ROOT}"))
        message(FATAL_ERROR "Please, set the correct Bang root path using \"Scripts/setBangPath.sh\". Current path: " ${BANG_PROJECT_ROOT})
    endif ()

    include(${BANG_PROJECT_ROOT}/CMakeIncludes/CMakeCheckInputParams.cmake REQUIRED)

endif (NOT DEFINED CMAKE_CHECK_INPUT_PARAMS_BUILT)
