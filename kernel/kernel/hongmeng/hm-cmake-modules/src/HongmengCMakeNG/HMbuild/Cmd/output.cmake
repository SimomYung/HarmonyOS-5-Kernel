# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild: output command
# Author: Huawei OS Kernel Lab
# Create: Thu Jun 24 09:23:55 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Builtin)
include(HongmengCMakeNG/HMbuild/Functions/SetOutput)

# output-target: xxx.elf

function(hmbuild_cmd_output)
    cmake_parse_arguments (PARSE_ARGV 0 _output "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_output: CMD=${_output_CMD}")

    list(SUBLIST _output_CMDLIST 1 -1  parts)
    foreach(p IN LISTS parts)
        set(target "${p}")
    endforeach()

    if (DEFINED target)
        hmbuild_set_target_output("${target}" "${_output_ARG}")
    endif()
endfunction(hmbuild_cmd_output)

# vim:ts=4:sw=4:expandtab
