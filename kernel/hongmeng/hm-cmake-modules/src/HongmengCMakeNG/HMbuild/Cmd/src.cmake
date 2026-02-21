# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'src-' command
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 10:11:38 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/AddSrc)

# src-target-CONFIG_XXX: xxx.c yyy.c
function(hmbuild_cmd_src)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG")
    hmcmakeng_debug("hmbuild_cmd_src, CMDLIST=${_cmd_CMDLIST}, ARG=${_cmd_ARG}")

    list(SUBLIST _cmd_CMDLIST 1 -1  cmd_parts)

    foreach(part IN LISTS cmd_parts)
        if ("${part}" STREQUAL "y")
            set(cond "${part}")
        elseif ("${part}" MATCHES "^CONFIG_.*")
            set(cond "${part}")
        elseif (NOT DEFINED target)
            set(target "${part}")
        else()
            hmcmakeng_error("Unknown option in 'src' command")
        endif()
    endforeach()

    hmcmakeng_debug("target=${target}, cond=${cond}")
    hmbuild_add_src(TARGET      ${target}
                    CONDITION   ${cond}
                    SOURCES     ${_cmd_ARG})

endfunction()

# vim:ts=4:sw=4:expandtab
