# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Support hmbuild_cache__ operations
# Author: Huawei OS Kernel Lab
# Create: Sat Jun 05 11:33:08 2021

include_guard(GLOBAL)

function(hmbuild_cache_namespace outvar)
    cmake_parse_arguments(PARSE_ARGV 0 _cache "" "SUBDIR" "")

    # See generation of HMBUILD_DIR_PREFIX. The ending '_' is tricky
    # because we need to avoid a single ending '_' for top level.
    # And we need to replace '/' with '_' in HMBUILD_DIR_PREFIX and
    # _cache_SUBDIR to support multilayer subdir, such as:
    # subdir-y: subdir_a/subdir_b
    string(REGEX REPLACE "/" "_" _hmbuild_dir_prefix "${HMBUILD_DIR_PREFIX}")
    if (_cache_SUBDIR)
        string(REGEX REPLACE "/" "_" _cache_SUBDIR "${_cache_SUBDIR}")
        set(${outvar} "${_hmbuild_dir_prefix}${_cache_SUBDIR}_" PARENT_SCOPE)
    else()
        set(${outvar} "${_hmbuild_dir_prefix}" PARENT_SCOPE)
    endif()
endfunction(hmbuild_cache_namespace)

function(hmbuild_cache_clear)
    cmake_parse_arguments(PARSE_ARGV 0 _cache "" "SUBDIR" "")
    hmbuild_cache_namespace(namespace SUBDIR "${_cache_SUBDIR}")

    get_cmake_property(_varNames VARIABLES)
    foreach (_varName ${_varNames})
        if ("${_varName}" MATCHES "^hmbuild_cache_${namespace}_.*")
            unset(${_varName} CACHE)
        endif()
    endforeach()
endfunction(hmbuild_cache_clear)

function(__hmbuild_cache_append op namespace var item)
    set(varname "hmbuild_cache_${namespace}_${var}")
    set(listval "$CACHE{${varname}}")
    list(${op} listval "${item}")
    set(${varname} ${listval} CACHE INTERNAL "hmcache variable" FORCE)
endfunction(__hmbuild_cache_append)

function(hmbuild_cache_set var val)
    cmake_parse_arguments(PARSE_ARGV 0 _cache "" "SUBDIR" "")
    hmbuild_cache_namespace(namespace SUBDIR "${_cache_SUBDIR}")

    set(varname "hmbuild_cache_${namespace}_${var}")
    set(${varname} ${val} CACHE INTERNAL "hmcache variable" FORCE)
endfunction(hmbuild_cache_set)

function(hmbuild_cache_append_to var item)
    cmake_parse_arguments(PARSE_ARGV 0 _cache "PREPEND" "SUBDIR" "")
    hmbuild_cache_namespace(namespace SUBDIR "${_cache_SUBDIR}")

    if (_cache_PREPEND)
        set(_op "PREPEND")
    else()
        set(_op "APPEND")
    endif()
    __hmbuild_cache_append(${_op} "${namespace}" "${var}" "${item}")
endfunction(hmbuild_cache_append_to)


function(hmbuild_cache_val_of outvar var)
    cmake_parse_arguments(PARSE_ARGV 0 _cache "" "SUBDIR" "")
    hmbuild_cache_namespace(namespace SUBDIR "${_cache_SUBDIR}")

    set(varname "hmbuild_cache_${namespace}_${var}")
    set(${outvar} "$CACHE{${varname}}" PARENT_SCOPE)
endfunction(hmbuild_cache_val_of)

# vim:ts=4:sw=4:expandtab
