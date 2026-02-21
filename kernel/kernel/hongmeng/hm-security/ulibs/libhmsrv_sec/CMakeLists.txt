# Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
# Description: cmake for libhmsrv_sec
# Author: Huawei OS Kernel Lab
# Create: Tue Nov 09 10:21:13 2021

# temporary hack for lds library path
add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}/../link_script")

include(HongmengCMakeNG/HMbuild)
