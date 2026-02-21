# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: keymgr sysk lib
# Author: Huawei OS Kernel Lab
# Create: Thu Nov 24 20:22:52 2022

include(HMFindUtils)

FindHMLib(KeymgrLib_sysk KeymgrLib::KeymgrLib_sysk
          INCLUDE libkeymgr/init.h
          STATICLIB keymgr_sysk
          QUIET "${KeymgrLib_sysk_FIND_QUIETLY}")

find_package_handle_standard_args(KeymgrLib_sysk
                                  REQUIRED_VARS
                                  KeymgrLib_sysk_static_FOUND
                                  NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
