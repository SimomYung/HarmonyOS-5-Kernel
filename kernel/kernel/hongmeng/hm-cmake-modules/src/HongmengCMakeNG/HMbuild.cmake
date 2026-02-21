# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild
# Author: Huawei OS Kernel Lab
# Create: Tue May 25 14:37:26 2021

include(HongmengCMakeNG/Log)
# This is the first setup phase, should depends on nothing.
include(HongmengCMakeNG/HMbuild/Lang)
include(HongmengCMakeNG/HMbuild/Archive)
include(HongmengCMakeNG/HMbuild/Rules)
include(HongmengCMakeNG/HMbuild/Cache)
# Include Cflags early, befure feature detection
include(HongmengCMakeNG/HMbuild/Cflags)
include(HongmengCMakeNG/HMbuild/Ldflags)
include(HongmengCMakeNG/HMbuild/Targets)
include(HongmengCMakeNG/HMbuild/Features)

include(HongmengCMakeNG/HMbuild/Cmd/src)
include(HongmengCMakeNG/HMbuild/Cmd/lib)
include(HongmengCMakeNG/HMbuild/Cmd/exec)
include(HongmengCMakeNG/HMbuild/Cmd/cflags)
include(HongmengCMakeNG/HMbuild/Cmd/inclib)
include(HongmengCMakeNG/HMbuild/Cmd/subdir)
include(HongmengCMakeNG/HMbuild/Cmd/output)
include(HongmengCMakeNG/HMbuild/Cmd/findpkg)
include(HongmengCMakeNG/HMbuild/Cmd/install)
include(HongmengCMakeNG/HMbuild/Cmd/include)
include(HongmengCMakeNG/HMbuild/Cmd/linklib)
include(HongmengCMakeNG/HMbuild/Cmd/installinc)
include(HongmengCMakeNG/HMbuild/Cmd/installfindmodule)
include(HongmengCMakeNG/HMbuild/Cmd/libversion)
include(HongmengCMakeNG/HMbuild/Cmd/excludecflags)

include(HongmengCMakeNG/HMbuild/Functions/AddBuiltinLib)

# Put dependencies after Features, find pakcages according to Features.
# e.g. Find ulibs_kcfi and add __hmcfi__ definition through ulibs_kcfi
# interface settings.
include(HongmengCMakeNG/HMbuild/Dependencies)
include(HongmengCMakeNG/HMbuild/Builtin)

# cmake is slow.
# by manually checking, all these include files contains
# global guard at first line:
#   include_guard(GLOBAL)
#
if (NOT (DEFINED __HMBUILD_HEADERS_PREPARED))

# Fix warning 'Manually-specified variables were not used by the project'
if (NOT BUILD_USE_HMBUILD)
    hmcmakeng_debug("BUILD_USE_HMBUILD is not set")
endif()

function(hmbuild_process_cmd)
    cmake_parse_arguments (PARSE_ARGV 0 _dispatcher "" "CMD" "ARG")
    string(REGEX REPLACE "-" ";" cmdlist "${_dispatcher_CMD}")
    list(GET cmdlist 0 cmd)

    hmcmakeng_trace("hmbuild_process_cmd: CMDLIST=${cmdlist} cmd=${cmd}")

    # CMake 3.18 provides cmake_language which allow us use
    # cmake_language(CALL "hmbuild_cmd_${cmd}"), but 3.16 not.
    # Use if ("${cmd}" IN_LIST "${HMBUILD_CMD_LIST}") checks
    # if a command is available needs set(HMBUILD_CMD_LIST "lib;src")

    # Do not quote "${cmdlist}" to keep it as a list
    if ("${cmd}" STREQUAL "lib")
        hmbuild_cmd_lib(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}")
    elseif("${cmd}" STREQUAL "exec")
        hmbuild_cmd_exec(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}")
    elseif("${cmd}" STREQUAL "src")
        hmbuild_cmd_src(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}")
    elseif("${cmd}" STREQUAL "install")
        hmbuild_cmd_install(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}")
    elseif("${cmd}" STREQUAL "cflags")
        hmbuild_cmd_cflags(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "excludecflags")
        hmbuild_cmd_excludecflags(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "subdir")
        hmbuild_cmd_subdir(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "include")
        hmbuild_cmd_include(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "linklib")
        hmbuild_cmd_linklib(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "output")
        hmbuild_cmd_output(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "inclib")
        hmbuild_cmd_inclib(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "findpkg")
        hmbuild_cmd_findpkg(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "installinc")
        hmbuild_cmd_installinc(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "installfindmodule")
        hmbuild_cmd_installfindmodule(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    elseif("${cmd}" STREQUAL "libversion")
        hmbuild_cmd_libversion(CMDLIST ${cmdlist} ARG "${_dispatcher_ARG}" CMD "${_dispatcher_CMD}")
    else()
        hmcmakeng_error("Command ${cmd} not available")
    endif()
endfunction(hmbuild_process_cmd)

# This is the final init phase. It knows everything:
# language enabled, feature status, packages.
macro(hmbuild_setup_global_init)
    if (NOT DEFINED HMCMAKENG_HMBUILD_GLOBAL_INITED)
        set(HMCMAKENG_HMBUILD_GLOBAL_INITED "1")

        # Globally add sysroot and link dir. Not suggested by morden cmake but make sence.
        include_directories(SYSTEM $<TARGET_PROPERTY:HMBase::Sysroot,INTERFACE_INCLUDE_DIRECTORIES>)
        link_directories(AFTER $<TARGET_PROPERTY:HMBase::Sysroot,INTERFACE_LINK_DIRECTORIES>)
        add_compile_definitions($<TARGET_PROPERTY:HMBase::Sysroot,INTERFACE_COMPILE_DEFINITIONS>)
    endif()
endmacro(hmbuild_setup_global_init)

# Call this function *AFTER* HMBUILD_DIR_PREFIX setting
function(hmbuild_dir_init)
    hmbuild_cache_clear()
endfunction(hmbuild_dir_init)

# Finish __HMBUILD_HEADERS_PREPARED guard.
set(__HMBUILD_HEADERS_PREPARED "1")
endif()

# MAIN:
# Check existance of HMbuild
set(HMCMAKENG_HMBUILD_LIST ${CMAKE_CURRENT_SOURCE_DIR}/HMbuild)
set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING
        "Always set CMAKE_BUILD_TYPE to RELEASE in HMbuild because we have Kconfig" FORCE)
if (NOT EXISTS ${HMCMAKENG_HMBUILD_LIST})
    hmcmakeng_error("${HMCMAKENG_HMBUILD_LIST} not found")
endif()

# Setup: call macros at global namespace
# NOTE that in setup phase we should use macro instead of
# function for global namespace
# Order is important:
#  Feature detection needs to know which language is enabled befure calling checker. For headers
#    only building, some feature detection needs to be disabled. hmbuild_setup_lang <-- hmbuild_setup_features
#  find_package (hmbuild_setup_deps_base in hmbuild_setup_global_init) requires feature detection
#    because it needs to know which feature is enabled to support suffixing. hmbuild_setup_features <-- hmbuild_setup_global_init
#  find_package (hmbuild_setup_deps_base in hmbuild_setup_global_init) requires HMFIND_XX to be set
#    hmbuild_setup_find_package <-- hmbuild_setup_global_init
hmbuild_setup_features()
hmbuild_setup_find_package()
hmbuild_setup_deps_base()
hmbuild_setup_global_init()

# Relative path from source dir to current source dir
# See hmbuild_cache_namespace. We must protect the rule that
# "${HMBUILD_DIR_PREFIX}${_cache_SUBDIR}_" == ${HMBUILD_DIR_PREFIX of sub directory}
# Note that HMBUILD_DIR_PREFIX may contain '/', which will be replaced
# with '_' in hmbuild_cache_namespace.
file(RELATIVE_PATH _curr_dir_relpath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
if ("" STREQUAL "${_curr_dir_relpath}")
    set(HMBUILD_DIR_PREFIX "${_curr_dir_relpath}")
    hmcmakeng_status("Entering top level directory")
    set(HMBUILD_DIR_PREFIX_VERBOSE "top level directory")
else()
    set(HMBUILD_DIR_PREFIX "${_curr_dir_relpath}_")
    hmcmakeng_status("Entering directory: ${_curr_dir_relpath}")
    set(HMBUILD_DIR_PREFIX_VERBOSE "directory ${_curr_dir_relpath}")
endif()

# Init for each directory
hmbuild_dir_init()

# Start processing HMbuild
file(STRINGS ${HMCMAKENG_HMBUILD_LIST} hmbuildcontent REGEX "^[^#]+$")
foreach(c ${hmbuildcontent})
    string(REGEX REPLACE "^([^:]*):.*$"         "\\1" cmd "${c}")
    string(REGEX REPLACE "^[^:]*:[ \t]*(.*)$"   "\\1" arg "${c}")
    hmbuild_process_cmd(CMD "${cmd}"
                        ARG "${arg}")
endforeach()

# Generate build system

# Add builtin library before adding subdirs because
# during adding subdirs it require builtin static
# target is already set, so subdir can append INTERFACE
# dependency to it.
hmbuild_add_all_builtin_targets()
hmbuild_subdirs_add_all()
hmcmakeng_status("Finish parsing HMbuild in ${HMBUILD_DIR_PREFIX_VERBOSE}")

hmbuild_lib_add_all_static_targets()
hmbuild_lib_add_all_dynamic_targets()
hmbuild_exec_add_all_targets()

hmbuild_install_all_targets()
hmbuild_install_all_includes()

hmbuild_exclude_all()
# vim:ts=4:sw=4:expandtab
