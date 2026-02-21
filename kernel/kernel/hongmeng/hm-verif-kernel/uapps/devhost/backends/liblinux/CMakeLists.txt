cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(lnxbase C ASM)

# import cmakeng
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)

# temporary hack for global include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lnxbase)

# temporary hack for lds library path
add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}/scripts")
add_link_options("-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/lnxbase/lnxbase.map")

# temporary hack for -nodefaultlibs link
add_link_options(-nodefaultlibs)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)
include(HongmengCMakeNG/HMbuild)
