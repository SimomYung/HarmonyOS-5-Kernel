# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild features: detect if KCFI is enabled
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 29 16:26:54 2021

# Check if cflags or specs generate kcfi-enabled object file

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_KCFI_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckKCFI.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_KCFI_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckKCFI.c in ${CMAKE_MODULE_PATH}")
endif()

# For Clang, CMAKE_C_COMPILE_OPTIONS_TARGET and CMAKE_C_COMPILER_TARGET are set
# For gcc, they are empty
set(__HMBUILD_FEATURE_CHECK_KCFI_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${CMAKE_C_FLAGS} -S ${__HMBUILD_FEATURE_CHECK_KCFI_TESTER}")
hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_KCFI_CMD}\" in ${__HMBUILD_FEATURE_KCFI_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_KCFI_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_KCFI_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_KCFI_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_KCFI_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_KCFI_RESULT
        )
unset(__HMBUILD_FEATURE_CHECK_KCFI_TESTER CACHE)

hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_KCFI_OUTPUT=${__HMBUILD_FEATURE_CHECK_KCFI_OUTPUT}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_KCFI_ERROR=${__HMBUILD_FEATURE_CHECK_KCFI_ERROR}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_KCFI_RESULT=${__HMBUILD_FEATURE_CHECK_KCFI_RESULT}")

if (NOT ("${__HMBUILD_FEATURE_CHECK_KCFI_RESULT}" EQUAL 0))
    hmcmakeng_warn("Failed to check feature KCFI: \
            ${__HMBUILD_FEATURE_CHECK_KCFI_OUTPUT}\
            ${__HMBUILD_FEATURE_CHECK_KCFI_ERROR}")
endif()

# See hm-security-source/git/tools/gcc-plugins/kcfi.cpp
# __function_tag_ symbol in asm is key to kcfi.
file(STRINGS "${__HMBUILD_FEATURE_KCFI_TEMPDIR}/HMFeatureCheckKCFI.s"
             __HMBUILD_FEATURE_CHECK_KCFI_ASM_CONTENT
             REGEX
             ".*__function_tag_.*")

if ("${__HMBUILD_FEATURE_CHECK_KCFI_ASM_CONTENT}" STREQUAL "")
  set(__HMBUILD_FEATURE_KCFI_VALUE "FALSE")
else()
  set(__HMBUILD_FEATURE_KCFI_VALUE "TRUE")
endif()

# vim:ts=4:sw=4:expandtab
