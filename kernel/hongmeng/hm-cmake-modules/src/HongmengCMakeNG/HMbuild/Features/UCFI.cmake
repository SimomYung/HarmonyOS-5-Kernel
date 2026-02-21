# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild features: detect if UCFI is enabled
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 29 16:26:54 2021

# Check if cflags or specs generate ucfi-enabled object file

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_UCFI_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckUCFI.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_UCFI_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckUCFI.c in ${CMAKE_MODULE_PATH}")
endif()

# For Clang, CMAKE_C_COMPILE_OPTIONS_TARGET and CMAKE_C_COMPILER_TARGET are set
# For gcc, they are empty
set(__HMBUILD_FEATURE_CHECK_UCFI_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${CMAKE_C_FLAGS} -S ${__HMBUILD_FEATURE_CHECK_UCFI_TESTER}")
hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_UCFI_CMD}\" in ${__HMBUILD_FEATURE_UCFI_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_UCFI_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_UCFI_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_UCFI_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_UCFI_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_UCFI_RESULT
        )
unset(__HMBUILD_FEATURE_CHECK_UCFI_TESTER CACHE)

hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_UCFI_OUTPUT=${__HMBUILD_FEATURE_CHECK_UCFI_OUTPUT}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_UCFI_ERROR=${__HMBUILD_FEATURE_CHECK_UCFI_ERROR}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_UCFI_RESULT=${__HMBUILD_FEATURE_CHECK_UCFI_RESULT}")

if (NOT ("${__HMBUILD_FEATURE_CHECK_UCFI_RESULT}" EQUAL 0))
    hmcmakeng_warn("Failed to check feature UCFI: \
            ${__HMBUILD_FEATURE_CHECK_UCFI_OUTPUT}\
            ${__HMBUILD_FEATURE_CHECK_UCFI_ERROR}")
endif()

# See hm-security-source/git/tools/gcc-plugins/ucfi.cpp
# __function_tag_ symbol in asm is key to ucfi.
file(STRINGS "${__HMBUILD_FEATURE_UCFI_TEMPDIR}/HMFeatureCheckUCFI.s"
             __HMBUILD_FEATURE_CHECK_UCFI_ASM_CONTENT
             REGEX
             ".*ucfi_fail.*")

if ("${__HMBUILD_FEATURE_CHECK_UCFI_ASM_CONTENT}" STREQUAL "")
  set(__HMBUILD_FEATURE_UCFI_VALUE "FALSE")
else()
  set(__HMBUILD_FEATURE_UCFI_VALUE "TRUE")
endif()

# vim:ts=4:sw=4:expandtab
