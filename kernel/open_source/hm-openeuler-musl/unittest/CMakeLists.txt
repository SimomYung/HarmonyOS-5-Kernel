cmake_minimum_required(VERSION 3.12.1 FATAL_ERROR)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(HM_LIBC ${CMAKE_SOURCE_DIR}/..)

# Project Name
project(hm_libc_ut)
enable_language(C)
enable_language(ASM)

# inlcude general interface of cmake
include(HongMengUT_Common)
include(HongMengCommon)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
if(SUBARCH STREQUAL "be")
	set(CONFIG_BIG_ENDIAN "y")
else()
	set(CONFIG_BIG_ENDIAN "n")
endif()

add_compile_options(-Wno-unused-parameter)
add_compile_options(-fno-short-enums)
add_compile_options(-DUT)

# "hm_unittest.h"
set(UT_HM_HEADERS ${CMAKE_SOURCE_DIR}/include/cmocka)

if (ARCH STREQUAL "arm")
	add_compile_options(-fno-omit-frame-pointer -mapcs)
	add_compile_options(-march=armv7-a)
else()
	add_compile_options(-fomit-frame-pointer)
endif()

# Sub-targets of this project
add_subdirectory(musl)
add_subdirectory(hm)
