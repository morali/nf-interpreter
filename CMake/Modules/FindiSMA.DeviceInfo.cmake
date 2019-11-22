#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE "${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/iSMA.DeviceInfo")

# set include directories
list(APPEND iSMA.DeviceInfo_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})

# source files
set(iSMA.DeviceInfo_SRCS

    #bacnet_object
    isma_deviceinfo_native.cpp
    isma_deviceinfo_native_iSMA_DeviceInfo_DeviceInfo.cpp
)

foreach(SRC_FILE ${iSMA.DeviceInfo_SRCS})
    set(iSMA.DeviceInfo_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(iSMA.DeviceInfo_SRC_FILE ${SRC_FILE}
        PATHS 
    
            # TODO: this needs to be changed so it's not platform dependent

            # class library source files
            ${BASE_PATH_FOR_THIS_MODULE}

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    #message("${SRC_FILE} >> ${iSMA.DeviceInfo_SRC_FILE}") # debug helper
    list(APPEND iSMA.DeviceInfo_SOURCES ${iSMA.DeviceInfo_SRC_FILE})
endforeach()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(iSMA.DeviceInfo DEFAULT_MSG iSMA.DeviceInfo_INCLUDE_DIRS iSMA.DeviceInfo_SOURCES)