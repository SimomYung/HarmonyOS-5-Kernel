# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: C configuration interface for Hongmeng platform
# Author: Huawei OS Kernel Lab
# Create: Thu Jul 29 10:20:08 2021

# Hmbuild just use kconfig control build and should not
# be affected by cmake environment variables
set(ENV{LDFLAGS} "")
set(ENV{CFLAGS} "")
set(ENV{CXXFLAGS} "")

# vim:ts=4:sw=4:expandtab
