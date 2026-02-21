# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Wed Nov 03 10:40:02 2021

include(HongmengCMakeNG/PreHmbuild)
include(HongmengCMakeNG/PreSecuritybuild)
include(HongmengCMakeNG/HMbuild)
include(HongmengCMakeNG/SecurityDescription)

hmbuild_builtin_target_name(builtin_tgt)
hmbuild_get_interface_target_property(net ${builtin_tgt} HMBUILD_CMAKETARGET_RAW)
if (NOT CONFIG_HMBUILD_MAKE_HEADERS OR CONFIG_SERURITY_JSON)
set (HMSD_OPTIONS "")
if (CONFIG_POSIX_CAPS)
  set (HMSD_OPTIONS ${HMSD_OPTIONS} CAPABILITY)
endif()
if (CONFIG_SEHARMONY)
  set (HMSD_OPTIONS ${HMSD_OPTIONS} SEHARMONY)
endif()
hmsd_generate(
    TARGET ${net}
    DIRECTORY security/hmsd
    ${HMSD_OPTIONS}
    TREND ${CONFIG_HMSD_TREND}
)
endif()
