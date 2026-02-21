# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: CMakeLists.txt
# Author: Huawei OS Kernel Lab
# Create: Tue May 24 09:31:23 2022

include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

include(HongmengCMakeNG/HMbuild)

if(CONFIG_HMBUILD_PROFILE_DEBUG)
    set(RAW_MUTEX_IMPL "RAW_MUTEX_IMPL_WITH_LOCKID")
    set(RAW_RWLOCK_IMPL "RAW_RWLOCK_IMPL_WITH_LOCKOWNER")
else()
    set(RAW_MUTEX_IMPL "RAW_MUTEX_IMPL_NO_LOCKID")
    set(RAW_RWLOCK_IMPL "RAW_RWLOCK_IMPL_WITHOUT_LOCKOWNER")
endif()

if(CONFIG_HMBUILD_PROFILE_DEBUG)
    set(RAW_MUTEX_LOCKTRACE "RAW_MUTEX_WITH_LOCKTRACE")
else()
    set(RAW_MUTEX_LOCKTRACE "RAW_MUTEX_NO_LOCKTRACE")
endif()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated/include/libhmsync/")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libhmsync/raw_mutex.h.in"
    "${CMAKE_BINARY_DIR}/generated/include/libhmsync/raw_mutex.h" @ONLY
)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/libhmsync/raw_rwlock.h.in"
    "${CMAKE_BINARY_DIR}/generated/include/libhmsync/raw_rwlock.h" @ONLY
)


if (DEFINED CONFIG_HMBUILD_MAKE_HEADERS)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/generated/include/"
            DESTINATION  "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}")
endif()
