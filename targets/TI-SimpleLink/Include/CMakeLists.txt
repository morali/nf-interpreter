#
# Copyright (c) 2019 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# append include directory for target TI SimpleLink
list(APPEND TARGET_TI_SimpleLink_COMMON_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")

# make var global
set(TARGET_TI_SimpleLink_COMMON_INCLUDE_DIRS ${TARGET_TI_SimpleLink_COMMON_INCLUDE_DIRS} CACHE INTERNAL "make global")

# add header with target platform definitions
configure_file("${CMAKE_SOURCE_DIR}/CMake/TI_SimpleLink_target_os.h.in"
               "${CMAKE_BINARY_DIR}/targets/TI-SimpleLink/${TI_BOARD}/target_os.h" @ONLY)
