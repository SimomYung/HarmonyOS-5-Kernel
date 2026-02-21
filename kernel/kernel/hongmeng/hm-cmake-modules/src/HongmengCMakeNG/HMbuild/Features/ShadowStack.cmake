# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild features: detect if ShaowdStack is enabled
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 29 16:26:54 2021

# Check if cflags or specs generate shadow-stack-enabled object file

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_SS_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckSS.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_SS_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckSS.c in ${CMAKE_MODULE_PATH}")
endif()

set(__HMBUILD_FEATURE_CHECK_SS_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${CMAKE_C_FLAGS} -Wattributes -Werror -c ${__HMBUILD_FEATURE_CHECK_SS_TESTER}")
hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_SS_CMD}\" in ${__HMBUILD_FEATURE_SS_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_SS_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_ShadowStack_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_SS_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_SS_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_SS_RESULT
        )
unset(__HMBUILD_FEATURE_CHECK_SS_TESTER CACHE)

hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SS_OUTPUT=${__HMBUILD_FEATURE_CHECK_SS_OUTPUT}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SS_ERROR=${__HMBUILD_FEATURE_CHECK_SS_ERROR}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SS_RESULT=${__HMBUILD_FEATURE_CHECK_SS_RESULT}")

# Check if nosstack attribute is supported
if (NOT ("${__HMBUILD_FEATURE_CHECK_SS_RESULT}" EQUAL 0))
    set(__HMBUILD_FEATURE_ShadowStack_VALUE "FALSE")
else()
    set(__HMBUILD_FEATURE_ShadowStack_VALUE "TRUE")
endif()

# vim:ts=4:sw=4:expandtab
