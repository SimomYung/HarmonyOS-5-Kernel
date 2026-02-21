# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'linklib-' command
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 22 20:01:16 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/AddLinkLib)

# linklib-target-y: library
function(hmbuild_cmd_linklib)
    cmake_parse_arguments (PARSE_ARGV 0 _linklib "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_linklib: CMD=${_linklib_CMD}")

    list(SUBLIST _linklib_CMDLIST 1 -1  parts)
    foreach(p IN LISTS parts)
        if ("${p}" STREQUAL "y")
            set(cond "${p}")
        elseif ("${p}" MATCHES "^CONFIG_.*")
            set(cond "${p}")
        elseif (NOT DEFINED target)
            set(target "${p}")
        endif()
    endforeach()

    if (NOT DEFINED cond)
        set(cond "y")
    endif()

    if (NOT DEFINED target)
        hmcmakeng_error("Command linklib: no target provided")
    endif()

    hmbuild_append_target_linklib("${target}" "${_linklib_ARG}"
                                  CONDITION "${cond}")
endfunction(hmbuild_cmd_linklib)

# vim:ts=4:sw=4:expandtab
