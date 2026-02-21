# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'exec-' command
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 10:11:38 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/AddExec)

# exec-static-y: execname
# exec-dynamic-CONFIG_XXX: execname
function(hmbuild_cmd_exec)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG")

    list(GET     _cmd_CMDLIST 1     type)
    list(SUBLIST _cmd_CMDLIST 2 -1  options)

    set(_avail_types "static;dynamic")
    if (NOT (type IN_LIST _avail_types))
        hmcmakeng_error("hmbuild_cmd_exec: type ${type} invalid")
    endif()

    foreach(o IN LISTS options)
        if ("${o}" STREQUAL "y")
            set(cond "${o}")
        elseif ("${o}" MATCHES "^CONFIG_.*")
            set(cond "${o}")
        else()
            hmcmakeng_error("Unknown option in 'lib' command")
        endif()
    endforeach()

    hmcmakeng_debug("options=${options}, cond=${cond}")
    hmbuild_append_exec_list(TARGET      ${_cmd_ARG}
                             TYPE        ${type}
                             CONDITION   ${cond})

endfunction(hmbuild_cmd_exec)

# vim:ts=4:sw=4:expandtab
