#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE "${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/iSMA.LocalIO")


# set include directories
list(APPEND iSMA.LocalIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Core)
list(APPEND iSMA.LocalIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Include)
list(APPEND iSMA.LocalIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/HAL/Include)
list(APPEND iSMA.LocalIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/PAL/Include)
list(APPEND iSMA.LocalIO_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})
list(APPEND iSMA.LocalIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/iSMA.LocalIO)


# source files
set(iSMA.LocalIO_SRCS

    # class library source files
    isma_localio_native_iSMA_LocalIO_AO.cpp
    isma_localio_native_iSMA_LocalIO_DI.cpp
    isma_localio_native_iSMA_LocalIO_DO.cpp
    isma_localio_native_iSMA_LocalIO_TO.cpp
    isma_localio_native_iSMA_LocalIO_UI.cpp
    isma_localio_native_iSMA_LocalIO_LocalIOController.cpp
    isma_localio_native.cpp

)

foreach(SRC_FILE ${iSMA.LocalIO_SRCS})
    set(iSMA.LocalIO_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(iSMA.LocalIO_SRC_FILE ${SRC_FILE}
        PATHS 

            # class library source files
            ${BASE_PATH_FOR_THIS_MODULE}

            # core source files
            ${PROJECT_SOURCE_DIR}/src/iSMA.LocalIO

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${iSMA.LocalIO_SRC_FILE}") # debug helper
    list(APPEND iSMA.LocalIO_SOURCES ${iSMA.LocalIO_SRC_FILE})
endforeach()


include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(iSMA.LocaLIO DEFAULT_MSG iSMA.LocalIO_INCLUDE_DIRS iSMA.LocalIO_SOURCES)
