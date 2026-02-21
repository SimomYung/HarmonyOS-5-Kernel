# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'libversion-' command
# Author: Huawei OS Kernel Lab
# Create: Tue Sep 14 09:58:59 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/SetLibVersion)

function(hmbuild_libversion_parse_version_number outvar version)
    string(REGEX REPLACE "^([0-9]).*$"   "\\1"  major_num "${version}")
    string(REGEX REPLACE "^[0-9]\.(.*)$"   "\\1"  minor_num "${version}")
    set(${outvar}_major "${major_num}" PARENT_SCOPE)
    set(${outvar}_minor "${minor_num}" PARENT_SCOPE)
endfunction(hmbuild_libversion_parse_version_number)

function(hmbuild_libversion_append_version target version)
    # check version
    if(NOT version MATCHES "^[0-9]\.[0-9]$")
        hmcmakeng_error("hmbuild_append_lib_version: ${version} version format error")
    endif()

    hmbuild_libversion_parse_version_number(number ${version})

    hmbuild_append_lib_version("${target}"
                                MAJOR "${number_major}"
                                MINOR "${number_minor}")
endfunction(hmbuild_libversion_append_version)

# libversion-[target]: [major.minor]
# libversion-mylib: 1.2
function(hmbuild_cmd_libversion)
    cmake_parse_arguments (PARSE_ARGV 0 _libversion "" "" "CMDLIST;ARG;CMD")
    hmcmakeng_debug("hmbuild_cmd_libversion, CMDLIST=${_libversion_CMDLIST}, ARG=${_libversion_ARG}, CMD=${_libversion_CMD}")

    list(SUBLIST _libversion_CMDLIST 1 -1  parts)
    foreach(p IN LISTS parts)
        set(target "${p}")
    endforeach()

    if (NOT DEFINED target)
        return()
    endif()

    hmbuild_libversion_append_version("${target}" "${_libversion_ARG}")
endfunction()

# vim:ts=4:sw=4:expandtab
