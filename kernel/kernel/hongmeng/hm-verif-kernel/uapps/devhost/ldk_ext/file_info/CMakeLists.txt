# cmakeng build
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(dhext-file_info LANGUAGES NONE)

add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}")
# import cmakeng
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/HMbuild)
