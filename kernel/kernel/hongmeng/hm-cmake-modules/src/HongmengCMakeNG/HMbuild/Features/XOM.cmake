# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: HMbuild features: detect if XOM is enabled
# Author: Huawei OS Kernel Lab
# Create: Wed Jan 12 16:26:54 2022

# This cmake module handle two things:
#   1. Check whether xom is enabled
#      a) User specified xom flags
#      b) xom is work
#      if enabled set __HMBUILD_FEATURE_XOM_VALUE to TRUE else to FALSE.
#   2. Setup the path to xom.elf, if xom is enabled and we are building march64 target.
#      XOMPostBuild cmake module will use it to perform xom post-build process.

# OUTPUT var list:
#   __HMBUILD_FEATURE_XOM_VALUE: (TRUE/FALSE)
#      It will cause exported variable `HMBUILD_FEATURE_XOM` to
#      TRUE or FALSE by function in Features.cmake
#
#   HMBUILD_POSTBUILD_XOM_TOOL_PATH: (string variable)
#      This global variable is set when we need to do xom post process for march64 executables or shared libraries.
#      This is set and get by hmbuild_postbuild_set_arg()/hmbuild_postbuild_get_arg()
#      (see HongmengCMakeNG/HMbuild/PostBuild/Utils.cmake)
#      XOMPostBuild.cmake depends on it to perform xom post-build process.

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/PostBuild/Utils)

set(__HMBUILD_FEATURE_XOM_VALUE "FALSE")
set(__XOM_TESTER_BIN_NAME "HMFeatureCheckXOM.so")

find_file(__HMBUILD_FEATURE_CHECK_XOM_TESTER
              "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckXOM.c" PATHS
              ${CMAKE_MODULE_PATH}
              NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_XOM_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckXOM.c in ${CMAKE_MODULE_PATH}")
endif()

# For Clang, CMAKE_C_COMPILE_OPTIONS_TARGET and CMAKE_C_COMPILER_TARGET are set
# For gcc, they are empty

set(__COMPILE_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${__HMBUILD_FEATURE_CHECK_XOM_TESTER} ${CMAKE_C_FLAGS} -nodefaultlibs -shared -o ${__XOM_TESTER_BIN_NAME}")

set(__HMBUILD_FEATURE_CHECK_XOM_CMD "${__COMPILE_CMD}")

hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_XOM_CMD}\" in ${__HMBUILD_FEATURE_XOM_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_XOM_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_XOM_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_XOM_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_XOM_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_XOM_RESULT
        )

hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_XOM_OUTPUT=${__HMBUILD_FEATURE_CHECK_XOM_OUTPUT}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_XOM_ERROR=${__HMBUILD_FEATURE_CHECK_XOM_ERROR}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_XOM_RESULT=${__HMBUILD_FEATURE_CHECK_XOM_RESULT}")

set(__COMPILE_ERROR_INFO "${__HMBUILD_FEATURE_CHECK_XOM_CMD}\
                          ${__HMBUILD_FEATURE_CHECK_XOM_OUTPUT}\
                          ${__HMBUILD_FEATURE_CHECK_XOM_ERROR}")

if (NOT ("${__HMBUILD_FEATURE_CHECK_XOM_RESULT}" EQUAL 0))
    hmcmakeng_warn("Error in compilation: ${__COMPILE_ERROR_INFO}")
endif()

unset(__HMBUILD_FEATURE_CHECK_XOM_TESTER CACHE)

# Check cflags and find xom.elf
if("${CMAKE_C_FLAGS}" MATCHES ".*\\-\\-xom\\-compat32.*")
    set(__HMBUILD_FEATURE_CHECK_XOM_CMD
            "${CMAKE_READELF} -l ${__XOM_TESTER_BIN_NAME}")
elseif("${CMAKE_C_FLAGS}" MATCHES ".*\\-\\-xom\\-march64.*")
    # setup path to `xom.elf`
    string(REGEX MATCH "\\-\\-xom\\-elf\\-path=([^ \\t\\r\\n\\\\]|\\\\.)+" __XOM_ELF_PATH "${CMAKE_C_FLAGS}")
    if(__XOM_ELF_PATH)
        # Use the path specified to `xom.elf`
        string(REGEX REPLACE "\\-\\-xom\\-elf\\-path=(([^ \\t\\r\\n\\\\]|\\\\.)+)" "\\1" XOM_ELF "${__XOM_ELF_PATH}")
    else()
        # Search the path to `xom.elf`
        find_program(XOM_ELF NAMES "xom.elf")
    endif()
    # if find it, setup the xom feature check command
    if(XOM_ELF)
        set(__HMBUILD_FEATURE_CHECK_XOM_CMD
                "${XOM_ELF} ${__XOM_TESTER_BIN_NAME} \&\& \
                 ${CMAKE_READELF} -l ${__XOM_TESTER_BIN_NAME}")
    else()
        hmcmakeng_status("Failed to find xom.elf. By default, it should be located at ${HMNATIVESDKPATH}/usr/bin")
        unset(__HMBUILD_FEATURE_CHECK_XOM_CMD) # signal to skip following code
    endif()
else()
    # user not specify to enable xom and we do not need post-build process
    unset(__HMBUILD_FEATURE_CHECK_XOM_CMD) # signal to skip following code
endif()

if(__HMBUILD_FEATURE_CHECK_XOM_CMD)
    hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_XOM_CMD}\" in ${__HMBUILD_FEATURE_XOM_TEMPDIR}")
    execute_process(
            COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_XOM_CMD}"
            WORKING_DIRECTORY ${__HMBUILD_FEATURE_XOM_TEMPDIR}
            OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_XOM_OUTPUT
            ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_XOM_ERROR
            RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_XOM_RESULT
            )
    hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_XOM_OUTPUT=${__HMBUILD_FEATURE_CHECK_XOM_OUTPUT}")
    hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_XOM_ERROR=${__HMBUILD_FEATURE_CHECK_XOM_ERROR}")
    hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_XOM_RESULT=${__HMBUILD_FEATURE_CHECK_XOM_RESULT}")

    if (NOT ("${__HMBUILD_FEATURE_CHECK_XOM_RESULT}" EQUAL 0))
        hmcmakeng_status("Xom check command failed: xom feature not support \
                         ${__HMBUILD_FEATURE_CHECK_XOM_OUTPUT}\
                         ${__HMBUILD_FEATURE_CHECK_XOM_ERROR}")
    else()
        # Now check whether xom is work and signal other codes with whether xom post-build process is required
        # 'R' flag is removed for code section and only leave '  E' flags in generated binary is the key of XOM
        string(REGEX MATCH ".*  E .*" __MATCH_RESULT "${__HMBUILD_FEATURE_CHECK_XOM_OUTPUT}")
        if(NOT "${__MATCH_RESULT}" STREQUAL "")
            # xom feature is enabled and it is work
            set(__HMBUILD_FEATURE_XOM_VALUE "TRUE")
            if("${CMAKE_C_FLAGS}" MATCHES ".*\\-\\-xom\\-march64.*")
                # march64 program need post process, set up argument to xom post process module.
                hmbuild_postbuild_set_arg("XOM" "TOOL_PATH" "${XOM_ELF}")
            endif()
        endif()
    endif()
endif()

# vim:ts=4:sw=4:expandtab
