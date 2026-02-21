# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding libhmseharmony sysk
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 14 16:40:59 2023

include(HMFindUtils)

FindHMLib(HMSeHarmonyLib_sysk HMSeHarmonyLib::HMSeHarmonyLib_sysk
          INCLUDE libhmseharmony/services.h
          INCLUDE libhmseharmony/policycap.h
          INCLUDE libhmseharmony/avc.h
          INCLUDE libhmseharmony/flask.h
          INCLUDE libhmseharmony/permissions.h
          INCLUDE libhmseharmony/perm_file.h
          INCLUDE libhmseharmony/network.h
          STATICLIB hmseharmony_sysk
          SHAREDLIB hmseharmony_sysk
	  QUIET "${HMSeHarmonyLib_FIND_QUIETLY}")

  find_package_handle_standard_args(HMSeHarmonyLib
                                    REQUIRED_VARS
                                    HMSeHarmonyLib_sysk_static_FOUND
                                    HMSeHarmonyLib_sysk_shared_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
