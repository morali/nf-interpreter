#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# native code directory
# set(BASE_PATH_FOR_THIS_MODULE "${BASE_PATH_FOR_CLASS_LIBRARIES_MODULES}")

# set include directories
list(APPEND iSMA.BACnet_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_app)
list(APPEND iSMA.BACnet_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_object)
list(APPEND iSMA.BACnet_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_core_inc)
# list(APPEND iSMA.BACnet_INCLUDE_DIRS ${BASE_PATH_FOR_THIS_MODULE})
# list(APPEND iSMA.BACnet_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet)

# source files
set(iSMA.BACnet_SRCS
    
    # bacnet_app    
    BACnetThread.c
    bip-init.c

    # bacnet_core
    abort.c
    address.c
    apdu.c
    bacaddr.c
    bacapp.c
    bacdcode.c
    bacerror.c
    bacint.c
    bacreal.c
    bacstr.c
    bip.c
    bvlc.c
    dcc.c
    iam.c
    lighting.c
    memcopy.c
    proplist.c
    reject.c
    rp.c
    rpm.c
    npdu.c
    tsm.c
    whois.c
    wp.c
    wpm.c
    version.c
    
    #bacnet_handlers
    dlenv.c
    h_cov.c
    h_whois.c
    h_npdu.c
    h_iam.c
    h_rp.c
    h_rpm.c
    h_wp.c
    h_wpm.c
    noserv.c
    txbuf.c
    s_iam.c
    
    #bacnet_object
    # bacfile.c
    device.c
    # lc.c
)

foreach(SRC_FILE ${iSMA.BACnet_SRCS})
    set(iSMA.BACnet_SRC_FILE SRC_FILE-NOTFOUND)
    find_file(iSMA.BACnet_SRC_FILE ${SRC_FILE}
        PATHS 
    
            # TODO: this needs to be changed so it's not platform dependent
            ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_app
            ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_core
            ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_handlers
            ${PROJECT_SOURCE_DIR}/targets/FreeRTOS/GC5/bacnet/bacnet_object

        CMAKE_FIND_ROOT_PATH_BOTH
    )
    # message("${SRC_FILE} >> ${iSMA.BACnet_SRC_FILE}") # debug helper
    list(APPEND iSMA.BACnet_SOURCES ${iSMA.BACnet_SRC_FILE})
endforeach()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(iSMA.BACnet DEFAULT_MSG iSMA.BACnet_INCLUDE_DIRS iSMA.BACnet_SOURCES)