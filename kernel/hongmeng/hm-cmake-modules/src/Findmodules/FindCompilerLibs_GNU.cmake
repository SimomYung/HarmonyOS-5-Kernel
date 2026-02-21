# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Use gcc --print-file-name to find libgcc
# Author: Huawei OS Kernel Lab
# Create: Mon Aug 02 20:28:04 2021

function(gcc_print_filename outvar name)

    set(__CMAKE_SAVED_TRY_COMPILE_TARGET_TYPE ${CMAKE_TRY_COMPILE_TARGET_TYPE})
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

    try_compile(_print_filename ${CMAKE_BINARY_DIR}
                ${CMAKE_ROOT}/Modules/CMakeCCompilerABI.c
                COMPILE_DEFINITIONS " --print-file-name=${name}"
                OUTPUT_VARIABLE output_text
                )

    set(CMAKE_TRY_COMPILE_TARGET_TYPE ${__CMAKE_SAVED_TRY_COMPILE_TARGET_TYPE})

    message(DEBUG "---- gcc_print_filename ${name} ----")

    string(REGEX REPLACE "\r?\n" ";" output_lines "${output_text}")
    foreach(line IN LISTS output_lines)
        message(DEBUG "parse output line: -- ${line}")
        if (NOT ("${line}" MATCHES "${name}$"))
            continue()
        endif()
        if (NOT (EXISTS ${line}))
            continue()
        endif()
        get_filename_component(comp_filename ${line} NAME)
        if (NOT (${comp_filename} STREQUAL ${name}))
            continue()
        endif()
        get_filename_component(result ${line} ABSOLUTE)
        set(${outvar} ${result} PARENT_SCOPE)
        return()
    endforeach()
    set(${outvar} NOTFOUND PARENT_SCOPE)
endfunction(gcc_print_filename)

# vim:ts=4:sw=4:expandtab
