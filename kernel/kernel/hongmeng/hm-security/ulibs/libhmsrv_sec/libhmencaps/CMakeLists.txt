# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: cmake for libhmdmverity
# Author: Huawei OS Kernel Lab
# Create: Wed Oct 12 10:53:13 2022

# find gperf
find_program(GPERF_EXECUTABLE gperf)
if(NOT GPERF_EXECUTABLE)
    message(FATAL_ERROR "gperf not found")
endif()

set(GENERATED_SRC ${CMAKE_CURRENT_BINARY_DIR}/encaps_str2int_hash.c)

# use gperf to generate encaps_str2int_hash.c
execute_process(
    COMMAND ${GPERF_EXECUTABLE} --output-file=${GENERATED_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/encaps_str2int.cfg
    RESULT_VARIABLE sed_result
    OUTPUT_VARIABLE sed_output
    ERROR_VARIABLE sed_error
)

# delete #line
execute_process(
    COMMAND sed -i "/^#line/d" ${GENERATED_SRC}
    RESULT_VARIABLE sed_result
    OUTPUT_VARIABLE sed_output
    ERROR_VARIABLE sed_error
)

include(HongmengCMakeNG/HMbuild)
