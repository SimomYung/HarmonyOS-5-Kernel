# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding libhmseharmony
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 14 16:40:59 2023

include(HMFindUtils)

FindHMLib(HMSeHarmonyLib HMSeHarmonyLib::HMSeHarmonyLib
          INCLUDE libhmseharmony/services.h
          INCLUDE libhmseharmony/policycap.h
          INCLUDE libhmseharmony/avc.h
          INCLUDE libhmseharmony/flask.h
          INCLUDE libhmseharmony/permissions.h
          INCLUDE libhmseharmony/perm_file.h
          INCLUDE libhmseharmony/network.h
          STATICLIB hmseharmony
          SHAREDLIB hmseharmony
	  QUIET "${HMSeHarmonyLib_FIND_QUIETLY}")

  find_package_handle_standard_args(HMSeHarmonyLib
                                    REQUIRED_VARS
                                    HMSeHarmonyLib_static_FOUND
                                    HMSeHarmonyLib_shared_FOUND
                                    NAME_MISMATCHED)
# vim:ts=4:sw=4:expandtab
