# Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
# Description: Cmake module for finding libhmaudit
# Author: Huawei OS Kernel Lab
# Create: Wed Dec 29 18:04:40 2021

include(HMFindUtils)

FindHMLib(HMAuditLib HMAuditLib::HMAuditLib
          INCLUDE libhmaudit/audit.h
          STATICLIB libhmaudit.a
          SHAREDLIB hmaudit
          QUIET "${HMAuditLib_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
