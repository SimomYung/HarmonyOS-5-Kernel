cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(devhost_ext LANGUAGES NONE)

# add local cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts")

# import cmakeng
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)

# temporary hack for global include
include_directories(include)
include_directories(include/api)
include_directories(include/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}/../backends/liblinux/scripts")

####### Import configs from Kconfig
set(CONFIG_NET_IO_URING ${KCONFIG_CONFIG_NET_IO_URING} CACHE BOOL "net io_uring")

# devhost need export for dlopen
add_link_options(-Wl,--export-dynamic)

# temporary hack for -nostdlib link
add_link_options(-nodefaultlibs)

# temporary hack for lds library path
add_link_options(-L${CMAKE_CURRENT_SOURCE_DIR})
include(HongmengCMakeNG/HMbuild)
