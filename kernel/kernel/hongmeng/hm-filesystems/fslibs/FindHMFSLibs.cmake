# Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
# Description: Cmake modules for finding hm fslibs
# Author: Huawei OS Kernel Lab
# Create: Wed Jun 02 10:10:05 2021

include(HMFindUtils)

FindHMLib(HMFSLib HMFSLibs::HMFSLib
          INCLUDE vfs.h
          STATICLIB libhmsrv_fs.a
          SHAREDLIB hmsrv_fs
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

FindHMLib(HMFSKernLib HMFSLibs::HMFSKernLib
          INCLUDE vfs.h
          STATICLIB libhmsrv_kernfs.a
          SHAREDLIB hmsrv_kernfs
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

FindHMLib(HMFsrvLib HMFSLibs::HMFsrvLib
          INCLUDE vfs.h
          STATICLIB libvfs.a
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

FindHMLib(HMFSLdsoLib HMFSLibs::HMFSLdsoLib
          INCLUDE vfs.h fs_ldso.h
          STATICLIB libhmsrv_ldso.a
          QUIET "${HMFSLibs_FIND_QUIETLY}"
          )

# Lib (package name) and Libs (name of find module)...
set(HMFSKernLib_FIND_QUIETLY "${HMFSLibs_FIND_QUIETLY}")
set(HMFSLib_FIND_QUIETLY "${HMFSLibs_FIND_QUIETLY}")
set(HMFSLdsoLib_FIND_QUIETLY "${HMFSLibs_FIND_QUIETLY}")

find_package_handle_standard_args(HMFSLdsoLib
                                    REQUIRED_VARS
                                    HMFSLdsoLib_static_FOUND
                                    NAME_MISMATCHED)

find_package_handle_standard_args(HMFSKernLib
                                    REQUIRED_VARS
                                    HMFSKernLib_static_FOUND
                                    HMFSKernLib_shared_FOUND
                                    NAME_MISMATCHED)

find_package_handle_standard_args(HMFSLib
                                    REQUIRED_VARS
                                    HMFSLib_static_FOUND
                                    HMFSLib_shared_FOUND
                                    NAME_MISMATCHED)


find_package_handle_standard_args(HMFSLibs
                                    REQUIRED_VARS
                                    HMFSLib_FOUND
                                    HMFSKernLib_FOUND
                                    HMFSLdsoLib_FOUND
                                    NAME_MISMATCHED)
