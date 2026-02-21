# Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
# Description: HMbuild features: detect if ffixed-x18 is enabled
# Author: Huawei OS Kernel Lab
# Create: Thu Aug 22 16:26:54 2024

# Check if cflags or specs generate shadow-stack-enabled object file

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_FFIXED_X18_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckFFIXED_X18.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_FFIXED_X18_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckFFIXED_X18.c in ${CMAKE_MODULE_PATH}")
endif()

set(__HMBUILD_FEATURE_CHECK_FFIXED_X18_CMD
    "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET} ${CMAKE_C_COMPILER_TARGET} ${CMAKE_C_FLAGS} -g -O2 \
    -Werror -nostartfiles -nodefaultlibs -Wl,--fatal-warnings -shared -o libCheckFFIXED_X18.so \
    ${__HMBUILD_FEATURE_CHECK_FFIXED_X18_TESTER}")
hmcmakeng_status("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_FFIXED_X18_CMD}\" in ${__HMBUILD_FEATURE_FFIXED_X18_TEMPDIR}")

# Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
execute_process(
        COMMAND sh -c "${__HMBUILD_FEATURE_CHECK_FFIXED_X18_CMD}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_FFIXED_X18_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_FFIXED_X18_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_FFIXED_X18_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_FFIXED_X18_RESULT
        )
unset(__HMBUILD_FEATURE_CHECK_FFIXED_X18_TESTER CACHE)

hmcmakeng_status("__HMBUILD_FEATURE_CHECK_FFIXED_X18_OUTPUT=${__HMBUILD_FEATURE_CHECK_FFIXED_X18_OUTPUT}")
hmcmakeng_status("__HMBUILD_FEATURE_CHECK_FFIXED_X18_ERROR=${__HMBUILD_FEATURE_CHECK_FFIXED_X18_ERROR}")
hmcmakeng_status("__HMBUILD_FEATURE_CHECK_FFIXED_X18_RESULT=${__HMBUILD_FEATURE_CHECK_FFIXED_X18_RESULT}")

# Check if -ffixed-x18 is supported
if ("${__HMBUILD_FEATURE_CHECK_FFIXED_X18_RESULT}" EQUAL 0)
    set(__HMBUILD_FEATURE_FFIXED_X18_VALUE "FALSE")
else()
    string(REPLACE "\n" ";" ERROR_ITEM_LIST ${__HMBUILD_FEATURE_CHECK_FFIXED_X18_ERROR})
    foreach(item ${ERROR_ITEM_LIST})
        if ((${item} MATCHES "error: inline asm clobber list contains reserved registers: X18") OR
            (${item} MATCHES "error: linker command failed with exit code 1") OR
            (${item} MATCHES "mrs.*x18,.*daif") OR
            (${item} MATCHES "^[ ]*\^"))
            hmcmakeng_debug("MATCHES expect error: ${item}")
        else()
            hmcmakeng_error("MATCHES unexpect error ${item}")
        endif()
    endforeach()
    set(__HMBUILD_FEATURE_FFIXED_X18_VALUE "TRUE")
endif()
