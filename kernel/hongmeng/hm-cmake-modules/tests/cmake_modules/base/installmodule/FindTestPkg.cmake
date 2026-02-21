# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMFindModule: find hmsrv_sec
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 22 17:06:52 2021

include(HMFindUtils)

FindHMLib(TestPkg TestPkg::TestPkg
          INCLUDE_SUBDIRS test_pkg
          INCLUDE test_pkg.h
          STATICLIB libtest_pkg.a
          SHAREDLIB test_pkg
          QUIET "${TestPkg_FIND_QUIETLY}")

find_package_handle_standard_args(TestPkg
                                    REQUIRED_VARS
                                    TestPkg_static_FOUND
                                    TestPkg_shared_FOUND
                                    NAME_MISMATCHED)

# vim:ts=4:sw=4:expandtab
