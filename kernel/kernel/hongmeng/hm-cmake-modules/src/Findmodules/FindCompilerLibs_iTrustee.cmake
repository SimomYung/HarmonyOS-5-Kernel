# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Use find to locate libgcc for iTrustee toolchain
# Author: Huawei OS Kernel Lab
# Create: Mon Aug 02 20:31:23 2021

function(gcc_print_filename outvar name)
    execute_process(
        COMMAND sh -c "${CMAKE_GNU_C_COMPILER} --print-file-name=${name}"
        OUTPUT_VARIABLE stdout_output
        ERROR_VARIABLE  stderr_output
        RESULT_VARIABLE result
    )

    if (NOT (${result} EQUAL 0))
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
        return()
    endif()

    if (NOT (EXISTS ${stdout_output}))
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
    endif()

    string(STRIP "${stdout_output}" ${outvar})
    set(${outvar} "${${outvar}}" PARENT_SCOPE)
endfunction(gcc_print_filename)

# vim:ts=4:sw=4:expandtab
