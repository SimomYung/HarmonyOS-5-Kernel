# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: hmbuild cmd installfindmodule
# Author: Huawei OS Kernel Lab
# Create: Wed Aug 11 22:17:02 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/InstallFindModule)

# installfindmodule: file
function(hmbuild_cmd_installfindmodule)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_installfindmodule, CMDLIST=${_cmd_CMDLIST}, ARG=${_cmd_ARG}")

    hmbuild_installfindmodule(${_cmd_ARG})
endfunction(hmbuild_cmd_installfindmodule)

# vim:ts=4:sw=4:expandtab
