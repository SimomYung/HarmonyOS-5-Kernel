# Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
# Description: Cmake module for finding hm-libvsync
# Author: Huawei OS Kernel Lab
# Create: Thu Aug 05 16:30:05 2021

include(HMFindUtils)

# INCLUDE_SUBDIRS causes -Iusr/include/libvsync is added
# to cflags to make sure this package is referenced explicitly.
FindHMLib(LibVSync LibVSync::LibVSync
          INCLUDE vsync/atomic.h
          INCLUDE_SUBDIRS libvsync
          QUIET "${LibVSync_FIND_QUIETLY}")

# vim:ts=4:sw=4:expandtab
