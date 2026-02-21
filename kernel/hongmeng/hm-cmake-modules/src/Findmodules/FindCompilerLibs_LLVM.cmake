# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Use find to locate libgcc for LLVM toolchain
# Author: Huawei OS Kernel Lab
# Create: Fri Mar 24 17:02:50 2023

function(gcc_print_filename outvar name)
    execute_process(
        COMMAND sh -c "${CMAKE_C_COMPILER} \
                       --target=${CMAKE_C_COMPILER_TARGET} \
                       --sysroot=${CMAKE_SYSROOT} \
                       --print-file-name=${name}"
        OUTPUT_VARIABLE stdout_output
        ERROR_VARIABLE  stderr_output
        RESULT_VARIABLE result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if ((NOT (${result} EQUAL 0)) OR (NOT (EXISTS ${stdout_output})))
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
        return()
    endif()

    set(${outvar} "${stdout_output}" PARENT_SCOPE)
endfunction(gcc_print_filename)

# vim:ts=4:sw=4:expandtab
