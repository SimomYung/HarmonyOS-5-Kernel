# Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
# Description: Cmake modules for finding hm fslibs with sysk suffix
# Author: Huawei OS Kernel Lab
# Create: Mon Feb 21 10:19:17 2022

include(HMFindUtils)

FindHMLib(HMFSLib_sysk HMFSLibs::HMFSLib_sysk
          INCLUDE vfs.h
          STATICLIB hmsrv_fs_sysk
          SHAREDLIB hmsrv_fs_sysk
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

FindHMLib(HMFSKernLib_sysk HMFSLibs::HMFSKernLib_sysk
          INCLUDE vfs.h
          STATICLIB hmsrv_kernfs_sysk
          SHAREDLIB hmsrv_kernfs_sysk
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

FindHMLib(HMFSLdsoLib_sysk HMFSLibs::HMFSLdsoLib_sysk
          INCLUDE vfs.h fs_ldso.h
          STATICLIB hmsrv_ldso_sysk
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

# Lib (package name) and Libs (name of find module)...
set(HMFSKernLib_FIND_QUIETLY "${HMFSLibs_FIND_QUIETLY}")
set(HMFSLib_FIND_QUIETLY "${HMFSLibs_FIND_QUIETLY}")
set(HMFSLdsoLib_FIND_QUIETLY "${HMFSLibs_FIND_QUIETLY}")

find_package_handle_standard_args(HMFSLdsoLib_sysk
                                    REQUIRED_VARS
                                    HMFSLdsoLib_sysk_static_FOUND
                                    NAME_MISMATCHED)

find_package_handle_standard_args(HMFSKernLib_sysk
                                    REQUIRED_VARS
                                    HMFSKernLib_sysk_static_FOUND
                                    HMFSKernLib_sysk_shared_FOUND
                                    NAME_MISMATCHED)

find_package_handle_standard_args(HMFSLib_sysk
                                    REQUIRED_VARS
                                    HMFSLib_sysk_static_FOUND
                                    HMFSLib_sysk_shared_FOUND
                                    NAME_MISMATCHED)


find_package_handle_standard_args(HMFSLibs
                                    REQUIRED_VARS
                                    HMFSLib_sysk_FOUND
                                    HMFSKernLib_sysk_FOUND
                                    HMFSLdsoLib_sysk_FOUND
                                    NAME_MISMATCHED)
