# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Check if ubsan feature is enabled by cflags
# Author: Huawei OS Kernel Lab
# Create: Sat Jul 31 15:38:00 2021

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_UBSAN_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckKASAN_UBSAN.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_UBSAN_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckKASAN_UBSAN.c in ${CMAKE_MODULE_PATH}")
endif()

set(__HMBUILD_FEATURE_CHECK_UBSAN_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${CMAKE_C_FLAGS} -S ${__HMBUILD_FEATURE_CHECK_UBSAN_TESTER}")
hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_UBSAN_CMD}\" in ${__HMBUILD_FEATURE_UBSAN_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_UBSAN_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_UBSAN_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_UBSAN_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_UBSAN_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_UBSAN_RESULT
        )
unset(__HMBUILD_FEATURE_CHECK_UBSAN_TESTER CACHE)

hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_UBSAN_OUTPUT=${__HMBUILD_FEATURE_CHECK_UBSAN_OUTPUT}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_UBSAN_ERROR=${__HMBUILD_FEATURE_CHECK_UBSAN_ERROR}")
hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_UBSAN_RESULT=${__HMBUILD_FEATURE_CHECK_UBSAN_RESULT}")

if (NOT ("${__HMBUILD_FEATURE_CHECK_UBSAN_RESULT}" EQUAL 0))
    hmcmakeng_warn("Failed to check feature UBSAN: \
            ${__HMBUILD_FEATURE_CHECK_UBSAN_OUTPUT}\
            ${__HMBUILD_FEATURE_CHECK_UBSAN_ERROR}")
endif()

# See hm-security-source/git/tools/gcc-plugins/kcfi.cpp
# __function_tag_ symbol in asm is key to kcfi.
file(STRINGS "${__HMBUILD_FEATURE_UBSAN_TEMPDIR}/HMFeatureCheckKASAN_UBSAN.s"
             __HMBUILD_FEATURE_CHECK_UBSAN_ASM_CONTENT
             REGEX
             ".*__ubsan_.*")

if ("${__HMBUILD_FEATURE_CHECK_UBSAN_ASM_CONTENT}" STREQUAL "")
  set(__HMBUILD_FEATURE_UBSAN_VALUE "FALSE")
else()
  set(__HMBUILD_FEATURE_UBSAN_VALUE "TRUE")
endif()
