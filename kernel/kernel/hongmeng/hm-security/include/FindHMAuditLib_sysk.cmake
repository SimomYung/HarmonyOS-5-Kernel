# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake module for finding libhmaudit_sysk
# Author: Huawei OS Kernel Lab
# Create: Sun Jul 10 20:48:17 2022

include(HMFindUtils)

FindHMLib(HMAuditLib_sysk HMAuditLib::HMAuditLib_sysk
          INCLUDE libhmaudit/audit.h
          STATICLIB hmaudit_sysk
          SHAREDLIB hmaudit_sysk
          QUIET "${HMAuditLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
