#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE "${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/iSMA.Log")


# set include directories
list(APPEND iSMA.Log_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Core)
list(APPEND iSMA.Log_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Include)
list(APPEND iSMA.Log_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/HAL/Include)
list(APPEND iSMA.Log_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/PAL/Include)
list(APPEND iSMA.Log_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})
list(APPEND iSMA.Log_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/iSMA.Log)


# source files
set(iSMA.Log_SRCS

    # class library source files
    isma_log_native.cpp
    isma_log_native_iSMA_Log_Log.cpp

)

foreach(SRC_FILE ${iSMA.Log_SRCS})
    set(iSMA.Log_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(iSMA.Log_SRC_FILE ${SRC_FILE}
        PATHS 

            # class library source files
            ${BASE_PATH_FOR_THIS_MODULE}

            # core source files
            ${PROJECT_SOURCE_DIR}/src/iSMA.Log

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${iSMA.Log_SRC_FILE}") # debug helper
    list(APPEND iSMA.Log_SOURCES ${iSMA.Log_SRC_FILE})
endforeach()


include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(iSMA.Log DEFAULT_MSG iSMA.Log_INCLUDE_DIRS iSMA.Log_SOURCES)
