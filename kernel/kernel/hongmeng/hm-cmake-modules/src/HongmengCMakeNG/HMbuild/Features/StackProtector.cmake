# Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: HMbuild features: detect if stack protector is enabled
# Author: Huawei OS Kernel Lab
# Create: Fri Sep 16 17:44:46 2022

# Check if cflags or specs generate stack-protector-enabled object file

include(HongmengCMakeNG/Log)

find_file(__HMBUILD_FEATURE_CHECK_SP_TESTER
          "HongmengCMakeNG/HMbuild/Features/HMFeatureCheckSP.c" PATHS
          ${CMAKE_MODULE_PATH}
          NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

if (NOT __HMBUILD_FEATURE_CHECK_SP_TESTER)
    hmcmakeng_error("Failed to find HMFeatureCheckSP.c in ${CMAKE_MODULE_PATH}")
endif()

MACRO(__HMBUILD_EXEC_CHECK_SP_CMD cmd)
    hmcmakeng_debug("Exec cmd: \"${cmd}\" in ${__HMBUILD_FEATURE_StackProtector_TEMPDIR}")
    # Use sh -c because CMAKE_C_FLAGS would be treated as one argument.
    execute_process(
        COMMAND sh -c "${cmd}"
        WORKING_DIRECTORY ${__HMBUILD_FEATURE_StackProtector_TEMPDIR}
        OUTPUT_VARIABLE __HMBUILD_FEATURE_CHECK_SP_OUTPUT
        ERROR_VARIABLE __HMBUILD_FEATURE_CHECK_SP_ERROR
        RESULT_VARIABLE __HMBUILD_FEATURE_CHECK_SP_RESULT
        )
    unset(__HMBUILD_FEATURE_CHECK_SP_TESTER CACHE)

    hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SP_OUTPUT=${__HMBUILD_FEATURE_CHECK_SP_OUTPUT}")
    hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SP_ERROR=${__HMBUILD_FEATURE_CHECK_SP_ERROR}")
    hmcmakeng_debug("__HMBUILD_FEATURE_CHECK_SP_RESULT=${__HMBUILD_FEATURE_CHECK_SP_RESULT}")

    if (NOT ("${__HMBUILD_FEATURE_CHECK_SP_RESULT}" EQUAL 0))
        hmcmakeng_warn("Failed to check feature SP: \
            ${__HMBUILD_FEATURE_CHECK_SP_OUTPUT}\
            ${__HMBUILD_FEATURE_CHECK_SP_ERROR}")
    endif()
ENDMACRO()

# For Clang, CMAKE_C_COMPILE_OPTIONS_TARGET and CMAKE_C_COMPILER_TARGET are set
# For gcc, they are empty
set(__HMBUILD_FEATURE_CHECK_SP_CMD
        "${CMAKE_C_COMPILER} ${CMAKE_C_COMPILE_OPTIONS_TARGET}${CMAKE_C_COMPILER_TARGET} \
         ${CMAKE_C_FLAGS} -O0 -c ${__HMBUILD_FEATURE_CHECK_SP_TESTER}")
hmcmakeng_debug("Exec cmd: \"${__HMBUILD_FEATURE_CHECK_SP_CMD}\" in ${__HMBUILD_FEATURE_StackProtector_TEMPDIR}")

__HMBUILD_EXEC_CHECK_SP_CMD("${__HMBUILD_FEATURE_CHECK_SP_CMD}")

# when -fstack-protector-strong or -fstack-protector is add,
# __stack_chk_fail will be linked and used, otherwise won't.
set(__HMBUILD_FEATURE_CHECK_SP_CMD
	"${CMAKE_NM} HMFeatureCheckSP.o")

__HMBUILD_EXEC_CHECK_SP_CMD("${__HMBUILD_FEATURE_CHECK_SP_CMD}")

string(REGEX MATCHALL ".*__stack_chk_fail.*"
                      __HMBUILD_FEATURE_CHECK_SP_OBJ_SYM
                      "${__HMBUILD_FEATURE_CHECK_SP_OUTPUT}")

if ("${__HMBUILD_FEATURE_CHECK_SP_OBJ_SYM}" STREQUAL "")
  set(__HMBUILD_FEATURE_StackProtector_VALUE "FALSE")
else()
  set(__HMBUILD_FEATURE_StackProtector_VALUE "TRUE")
endif()

# vim:ts=4:sw=4:expandtab
