cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(udk_modules C ASM)

# import cmakeng
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)
include(HongmengCMakeNG/HMbuild)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS HMbuild)
