# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: hmbuild_findpkg function
# Author: Huawei OS Kernel Lab
# Create: Tue Aug 10 23:47:34 2021

include_guard(GLOBAL)

# Require hmbuild_find_package
include(HongmengCMakeNG/HMbuild/Dependencies)

function(hmbuild_findpkg pkgname)
    find_package(${pkgname} QUIET)
endfunction(hmbuild_findpkg)

# vim:ts=4:sw=4:expandtab
