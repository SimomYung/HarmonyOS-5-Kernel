include(HongmengCMakeNG/HMbuild)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SYSROOT}/lib/udk/share/cmake/Modules")
include(UDKDriverBundle)

hmbuild_get_interface_target_property(drv udktest_unload HMBUILD_CMAKETARGET_RAW)
udk_driver_make_bundle(${drv})
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)
