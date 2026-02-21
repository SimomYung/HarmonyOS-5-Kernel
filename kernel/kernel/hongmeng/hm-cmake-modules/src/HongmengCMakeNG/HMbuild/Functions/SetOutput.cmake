# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild: hmbuild_set_target_output function
# Author: Huawei OS Kernel Lab
# Create: Thu Jun 24 09:33:54 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Targets)

function(hmbuild_set_target_output target output)
    hmcmakeng_debug("hmbuild_set_target_output: target=${target} output=${output}")
    hmbuild_target_set_output_name(${target} "${output}")
endfunction(hmbuild_set_target_output)

# vim:ts=4:sw=4:expandtab
