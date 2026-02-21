# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: keymgr lib
# Author: Huawei OS Kernel Lab
# Create: Thu Nov 24 20:22:52 2022

include(HMFindUtils)

FindHMLib(KeymgrLib KeymgrLib::KeymgrLib
          INCLUDE libkeymgr/init.h
          STATICLIB keymgr
          QUIET "${KeymgrLib_FIND_QUIETLY}")

find_package_handle_standard_args(KeymgrLib
                                  REQUIRED_VARS
                                  KeymgrLib_static_FOUND)

# vim:ts=4:sw=4:expandtab
