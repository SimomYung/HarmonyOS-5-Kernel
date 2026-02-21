# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: CMakeLists.txt for libvdso.so
# Author: Huawei OS Kernel Lab
# Create: Wed Mar 23 14:29:02 2022

cmake_minimum_required(VERSION 3.16)
project(hm-vdso LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

if (DEFINED CONFIG_AARCH64)
    set(ARCH "aarch64")
elseif (DEFINED CONFIG_ARM)
    set(ARCH "arm")
endif()

string(FIND "${CMAKE_C_FLAGS}" "-mabi=ilp32" ilp32)

# find ilp32 option in CFLAGS
if(NOT ilp32 EQUAL -1)
    set(ABI_CFLAGS "-mabi=ilp32")
endif()

set(FIND_INCLUDE_PATH "${CMAKE_SYSROOT}/usr/${CONFIG_HMBUILD_FINDPATH_INCLUDE}")
if (CONFIG_XOM)
set(LDS_SUFFIX "_xom")
elseif (CONFIG_HKIP_SEC_REGION_PROT_VDSO)
set(LDS_SUFFIX "_xom")
endif ()

set(CREATE_LDSCRIPT "${CMAKE_C_COMPILER} -E -P -C -U${ARCH} -D__ASSEMBLY__ -DLINKER_SCRIPT ${ABI_CFLAGS} ${CMAKE_SOURCE_DIR}/arch/${ARCH}/vdso/vdso${LDS_SUFFIX}.lds.S \
            -include ${CMAKE_BINARY_DIR}/include/generated/autoconf.h -I${FIND_INCLUDE_PATH} \
            -o vdso.lds")


hmbuild_get_interface_target_property(vdso_target vdso HMBUILD_CMAKETARGET_RAW)
add_custom_target(vdso.lds ALL
        COMMAND bash -c ${CREATE_LDSCRIPT})
add_dependencies(${vdso_target} vdso.lds)

hmbuild_get_interface_target_property(final_target vdso HMBUILD_CMAKETARGET_FINAL)
get_target_property(output_release ${final_target} "hmprop_output_release")
get_target_property(output_devel   ${final_target} "hmprop_output_devel")

install(PROGRAMS ${output_release}
    DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_BASE_BINDIR}"
    COMPONENT "libraries")

install(PROGRAMS ${output_devel}
    DESTINATION "/debug/${CONFIG_HMBUILD_INSTALL_DIR_BASE_BINDIR}"
    COMPONENT "libraries")
