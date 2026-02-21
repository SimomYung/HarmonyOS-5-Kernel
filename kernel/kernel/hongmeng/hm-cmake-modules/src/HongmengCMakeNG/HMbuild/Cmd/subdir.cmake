# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild subdir command
# Author: Huawei OS Kernel Lab
# Create: Sat Jun 05 14:50:56 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/AddSubdir)

# subdir-CONFIG_XXX: xxx
function(hmbuild_cmd_subdir)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_src, CMDLIST=${_cmd_CMDLIST}, ARG=${_cmd_ARG}")

    list(SUBLIST _cmd_CMDLIST 1 -1  cmd_parts)

    foreach(part IN LISTS cmd_parts)
        if ("${part}" STREQUAL "y")
            set(cond "${part}")
        elseif ("${part}" MATCHES "^CONFIG_.*")
            set(cond "${part}")
        else()
            hmcmakeng_error("Unknown option in 'subdir' command")
        endif()
    endforeach()

    hmcmakeng_debug("cond=${cond}")
    hmbuild_append_subdir_list(CONDITION    ${cond}
                               SUBDIR       ${_cmd_ARG})
endfunction(hmbuild_cmd_subdir)

# vim:ts=4:sw=4:expandtab
