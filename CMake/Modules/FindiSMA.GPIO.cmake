#
# Copyright (c) 2017 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
set(BASE_PATH_FOR_THIS_MODULE "${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}/iSMA.GPIO")


# set include directories
list(APPEND iSMA.GPIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Core)
list(APPEND iSMA.GPIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/CLR/Include)
list(APPEND iSMA.GPIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/HAL/Include)
list(APPEND iSMA.GPIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/PAL/Include)
list(APPEND iSMA.GPIO_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})
list(APPEND iSMA.GPIO_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/iSMA.GPIO)


# source files
set(iSMA.GPIO_SRCS

    # class library source files
    isma_gpio_native_iSMA_GPIO_DIPSwitch.cpp
    isma_gpio_native_iSMA_GPIO_DIPSwitchController.cpp
    isma_gpio_native_iSMA_GPIO_LED.cpp
    isma_gpio_native_iSMA_GPIO_LEDController.cpp
    isma_gpio_native.cpp

)

foreach(SRC_FILE ${iSMA.GPIO_SRCS})
    set(iSMA.GPIO_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(iSMA.GPIO_SRC_FILE ${SRC_FILE}
        PATHS 

            # class library source files
            ${BASE_PATH_FOR_THIS_MODULE}

            # core source files
            ${PROJECT_SOURCE_DIR}/src/iSMA.GPIO

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${iSMA.GPIO_SRC_FILE}") # debug helper
    list(APPEND iSMA.GPIO_SOURCES ${iSMA.GPIO_SRC_FILE})
endforeach()


include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(iSMA.GPIO DEFAULT_MSG iSMA.GPIO_INCLUDE_DIRS iSMA.GPIO_SOURCES)
