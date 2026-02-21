# Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
# Description: Cmake module for finding libopenssl
# Author: Huawei OS Kernel Lab
# Create: Mon Jan 6 10:24:00 2023

include(HMFindUtils)
FindHMLib(OpensslLib OpensslLib::OpensslLib
          STATICLIB crypto
          SHAREDLIB crypto
          QUIET "${HMOpensslLib_FIND_QUIETLY}")

set(OpensslLib_FIND_QUIETLY "${HMOpensslLib_FIND_QUIETLY}")
find_package_handle_standard_args(OpensslLib
                                  REQUIRED_VARS
                                  OpensslLib_static_FOUND
                                  OpensslLib_shared_FOUND)
