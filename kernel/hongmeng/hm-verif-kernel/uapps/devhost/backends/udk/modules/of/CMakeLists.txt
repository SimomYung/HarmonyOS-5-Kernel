include(HongmengCMakeNG/HMbuild)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SYSROOT}/lib/udk/share/cmake/Modules")
include(UDKDriverBundle)

hmbuild_get_interface_target_property(drv of_udk HMBUILD_CMAKETARGET_RAW)
udk_driver_make_bundle(${drv})
target_link_options(${drv} PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/../scripts/map/udk-drivers.map")

hmbuild_get_interface_target_property(drv of.proxy HMBUILD_CMAKETARGET_RAW)
udk_driver_make_bundle(${drv})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)
