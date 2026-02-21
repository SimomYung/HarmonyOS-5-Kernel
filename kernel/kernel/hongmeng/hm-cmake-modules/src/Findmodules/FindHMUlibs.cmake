# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Cmake module for finding hm ulibs
# Author: Huawei OS Kernel Lab
# Create: Wed Jun 02 10:10:05 2021

include(HMFindUtils)

# FIXME: According to HMBUILD_FEATURE_KCFI, HMBUILD_FEATURE_ShadowStack, ...
# We should find correct modules. Consider file suffix...
FindHMLib(HMUlibs HMUlibs::HMUlibs
          INCLUDE libhmlog/hmlog.h
          STATICLIB libhmulibs.a
          SHAREDLIB hmulibs
          QUIET "${HMUlibs_FIND_QUIETLY}")

# FIXME: Check if found libraries support all featrures ??

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HMUlibs
                                    REQUIRED_VARS
                                    HMUlibs_include_FOUND
                                    HMUlibs_static_FOUND
                                    HMUlibs_shared_FOUND
                                    NAME_MISMATCHED)

if (NOT DEFINED _HMUlibs_sysif_client_modules_list)
    set(_HMUlibs_sysif_client_modules_list
             "hmtracemgr;devmgr;srvmgr;devhost;hguard"
             "sysmgr;auditmgr;freqmgr;pwrmgr;keymgr"
             "fs;uvmm;crypt;test_framework")
endif()

foreach(m IN LISTS _HMUlibs_sysif_client_modules_list)
    find_path(HMUlibs_sysif_${m}_INCLUDE_DIR "libsysif/${m}/api.h"
              NO_DEFAULT_PATH
              HINTS ${HMUlibs_INCLUDE_DIR}/generated/sysif_client/${m}/)

    mark_as_advanced(HMUlibs_sysif_${m}_INCLUDE_DIR)

    set(HMUlibs_sysif_${m}_include_FIND_QUIETLY "TRUE")
    find_package_handle_standard_args(HMUlibs_sysif_${m}_include
                                      REQUIRED_VARS
                                        "HMUlibs_sysif_${m}_INCLUDE_DIR"
                                        NAME_MISMATCHED)
    if (HMUlibs_sysif_${m}_include_FOUND AND NOT (TARGET HMUlibs::HMUlibs_sysif_${m}_include))
        add_library(HMUlibs::HMUlibs_sysif_${m}_include UNKNOWN IMPORTED)
        set_target_properties(HMUlibs::HMUlibs_sysif_${m}_include PROPERTIES
                              INTERFACE_INCLUDE_DIRECTORIES ${HMUlibs_sysif_${m}_INCLUDE_DIR}
                              IMPORTED_LINK_INTERFACE_LANGUAGES C)
        if (TARGET HMUlibs::HMUlibs_include)
            set_property(TARGET HMUlibs::HMUlibs_sysif_${m}_include APPEND PROPERTY
                         $<TARGET_PROPERTY:HMUlibs::HMUlibs_include,INTERFACE_INCLUDE_DIRECTORIES>)
            set_property(TARGET HMUlibs::HMUlibs_sysif_${m}_include APPEND PROPERTY
                         INTERFACE_COMPILE_DEFINITIONS
                         $<TARGET_PROPERTY:HMUlibs::HMUlibs_include,INTERFACE_COMPILE_DEFINITIONS>)
        endif()
    endif()
endforeach()

if (NOT DEFINED _HMUlibs_sysif_server_modules_list)
    set(_HMUlibs_sysif_server_modules_list
             "hmtracemgr;devmgr;srvmgr;devhost;hguard"
             "sysmgr;auditmgr;freqmgr;pwrmgr;keymgr"
             "fs;uvmm;crypt;test_framework")
endif()

if (NOT DEFINED HMUlibs_sysif_serverbase_INCLUDE_DIR)
    find_path(HMUlibs_sysif_serverbase_INCLUDE_DIR "libsysif/base/server_template.h"
              NO_DEFAULT_PATH
              HINTS ${HMUlibs_INCLUDE_DIR}/generated/sysif_base)
    mark_as_advanced(HMUlibs_sysif_serverbase_INCLUDE_DIR)
endif()

foreach(m IN LISTS _HMUlibs_sysif_server_modules_list)
    find_path(HMUlibs_sysif_${m}_server_INCLUDE_DIR "libsysif/${m}/server.h"
              NO_DEFAULT_PATH
              HINTS ${HMUlibs_INCLUDE_DIR}/generated/sysif_server/${m}/)
    mark_as_advanced(HMUlibs_sysif_${m}_server_INCLUDE_DIR)

    set(HMUlibs_sysif_${m}_server_include_FIND_QUIETLY "TRUE")
    find_package_handle_standard_args(HMUlibs_sysif_${m}_server_include
                                      REQUIRED_VARS
                                        "HMUlibs_sysif_${m}_INCLUDE_DIR"
                                        "HMUlibs_sysif_serverbase_INCLUDE_DIR"
                                        NAME_MISMATCHED)

    if (HMUlibs_sysif_${m}_server_include_FOUND AND NOT (TARGET HMUlibs::HMUlibs_sysif_${m}_server_include))
        add_library(HMUlibs::HMUlibs_sysif_${m}_server_include UNKNOWN IMPORTED)
        set_target_properties(HMUlibs::HMUlibs_sysif_${m}_server_include PROPERTIES
                              INTERFACE_INCLUDE_DIRECTORIES
                                "${HMUlibs_sysif_${m}_server_INCLUDE_DIR};${HMUlibs_sysif_serverbase_INCLUDE_DIR}"
                              IMPORTED_LINK_INTERFACE_LANGUAGES C)
        if (TARGET HMUlibs::HMUlibs_include)
            set_property(TARGET HMUlibs::HMUlibs_sysif_${m}_server_include APPEND PROPERTY
                         $<TARGET_PROPERTY:HMUlibs::HMUlibs_include,INTERFACE_INCLUDE_DIRECTORIES>)
            set_property(TARGET HMUlibs::HMUlibs_sysif_${m}_server_include APPEND PROPERTY
                         INTERFACE_COMPILE_DEFINITIONS
                         $<TARGET_PROPERTY:HMUlibs::HMUlibs_include,INTERFACE_COMPILE_DEFINITIONS>)
        endif()
    endif()
endforeach()

# vim:ts=4:sw=4:expandtab
