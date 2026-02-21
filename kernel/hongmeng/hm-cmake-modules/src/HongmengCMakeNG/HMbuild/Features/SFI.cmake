# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild features: detect if SFI is enabled
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 29 16:26:54 2021

# Check if cflags or specs generate sfi-enabled object file

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_SFI_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckSFI.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_SFI_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckSFI.c in ${CMAKE_MODULE_PATH}")
endif()

set(__HMBUILD_FEATURE_CHECK_SFI_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${CMAKE_C_FLAGS} -Wattributes -Werror -c ${__HMBUILD_FEATURE_CHECK_SFI_TESTER}")
hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_SFI_CMD}\" in ${__HMBUILD_FEATURE_SFI_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_SFI_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_SFI_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_SFI_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_SFI_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_SFI_RESULT
        )
unset(__HMBUILD_FEATURE_CHECK_SFI_TESTER CACHE)

hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SFI_OUTPUT=${__HMBUILD_FEATURE_CHECK_SFI_OUTPUT}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SFI_ERROR=${__HMBUILD_FEATURE_CHECK_SFI_ERROR}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SFI_RESULT=${__HMBUILD_FEATURE_CHECK_SFI_RESULT}")

# Check if nosfi attribute is supported
if (NOT ("${__HMBUILD_FEATURE_CHECK_SFI_RESULT}" EQUAL 0))
    set(__HMBUILD_FEATURE_SFI_VALUE "FALSE")
else()
    set(__HMBUILD_FEATURE_SFI_VALUE "TRUE")
endif()

# vim:ts=4:sw=4:expandtab
