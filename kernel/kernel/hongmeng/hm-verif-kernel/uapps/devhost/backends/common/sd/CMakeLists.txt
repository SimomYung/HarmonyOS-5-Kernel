cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(sd_proxy C ASM)

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)
include(HongmengCMakeNG/HMbuild)

set(UDK_LIBS_INSTALL_PATH ${CONFIG_HMBUILD_INSTALL_DIR_BASE_LIBDIR}/modules)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SYSROOT}/lib/udk/share/cmake/Modules")
include(UDKDriverBundle)

hmbuild_get_interface_target_property(drv sd.proxy HMBUILD_CMAKETARGET_RAW)
udk_driver_make_bundle(${drv})

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)
