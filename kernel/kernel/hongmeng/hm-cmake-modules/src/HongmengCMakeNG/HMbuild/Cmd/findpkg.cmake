# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild findpkg command
# Author: Huawei OS Kernel Lab
# Create: Tue Aug 10 23:39:22 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/FindPkg)

# findpkg: xxx
# findpkg is a simple wrapper for find_package.
# provide this command to make sure find_package is invoked
# *AFTER* Kconfig is parsed because HMFindUtils read some
# HMFIND_XXX variables which is created by HMbuild based on
# Kconfig options.

function(hmbuild_cmd_findpkg)
    cmake_parse_arguments(PARSE_ARGV 0 _findpkg "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_findpkg: CMD=${_findpkg_CMD}")

    hmbuild_findpkg("${_findpkg_ARG}")
endfunction(hmbuild_cmd_findpkg)

# vim:ts=4:sw=4:expandtab
