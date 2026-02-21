# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: hmbuild cmd installinc
# Author: Huawei OS Kernel Lab
# Create: Wed Aug 11 17:26:42 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/InstallInc)

# installinc-subdir: file-or-dir
function(hmbuild_cmd_installinc)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_installinc, CMDLIST=${_cmd_CMDLIST}, ARG=${_cmd_ARG}")

    if ("installinc" STREQUAL "${_cmd_CMDLIST}")
        set(subdir "")
    else()
        string(REGEX REPLACE  "^installinc-(.*)$" "\\1" subdir ${_cmd_CMD})
    endif()

    hmbuild_append_installinc_list(HEADER ${_cmd_ARG}
                                   SUBDIR ${subdir})
endfunction(hmbuild_cmd_installinc)

# vim:ts=4:sw=4:expandtab
