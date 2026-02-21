# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMCmakeng: helper macro for finding packages
# Author: Huawei OS Kernel Lab
# Create: Tue Jun 22 08:07:19 2021

include(FindPackageHandleStandardArgs)

function(_FindHMLib_normalize_searchpath outvar val)
    set(_p "${CMAKE_SYSROOT}/${val}")
    file(TO_CMAKE_PATH "${_p}" p)
    set(${outvar} "${p}" PARENT_SCOPE)
endfunction(_FindHMLib_normalize_searchpath)

# searchbase should be /usr/include or /usr/include32
# Append paths use ARGN. outvar is set to a list of searchpaths
function(FindHMLib_searchpath outvar searchbase)
    cmake_parse_arguments("_search" "" "" "PREFIXES;SUBDIRS" ${ARGN})

    foreach(prefix ${_search_PREFIXES})
        foreach(subdir ${_search_SUBDIRS})
            _FindHMLib_normalize_searchpath(p "${prefix}/${searchbase}/${subdir}")
            list(APPEND path_list "${p}")
        endforeach()

        # Finally add searchbase to searchpath
        _FindHMLib_normalize_searchpath(p "${prefix}/${searchbase}")
        list(APPEND path_list "${p}")
    endforeach()
    set(${outvar} ${path_list} PARENT_SCOPE)
endfunction(FindHMLib_searchpath)

function(FindHMLib_find_file outvar file searchbase)
    if (NOT DEFINED ${outvar})
        FindHMLib_searchpath(searchpathlist "${searchbase}" ${ARGN})
        find_file(${outvar} "${file}"
                  NO_DEFAULT_PATH
                  HINTS ${searchpathlist})
    endif()
endfunction(FindHMLib_find_file)

function(FindHMLib_find_path outvar file searchbase)
    if (NOT DEFINED ${outvar})
        FindHMLib_searchpath(searchpathlist "${searchbase}" ${ARGN})
        find_path(${outvar} "${file}"
                  NO_DEFAULT_PATH
                  HINTS ${searchpathlist})
    endif()
endfunction(FindHMLib_find_path)

function(FindHMLib_find_hmvariant_suffix outvar)
    string(REGEX REPLACE " " ";" hmfindutils_conf_list "${CONFIG_HMBUILD_HMFINDUTILS_CONF}")

    foreach(p ${hmfindutils_conf_list})
        string(REGEX REPLACE "-[^\-]*$" "" conf_findutils_name ${p})
        string(REGEX MATCH "-[^\-]*$" conf_findutils_suffix ${p})

        if ("${conf_findutils_name}" STREQUAL "${CMAKE_FIND_PACKAGE_NAME}" AND
            NOT "${conf_findutils_suffix}" STREQUAL "")
            string(REGEX REPLACE "^-" "" conf_findutils_suffix ${conf_findutils_suffix})
            set(${outvar} ${conf_findutils_suffix} PARENT_SCOPE)
            return()
        endif()
    endforeach()
endfunction(FindHMLib_find_hmvariant_suffix)

macro(FindHMLib_rename_lib_filename hmfind_hmvariant_suffix filename lib_suffix)
    if (DEFINED ${hmfind_hmvariant_suffix})
        string(REGEX REPLACE "${lib_suffix}$"
                             "_${${hmfind_hmvariant_suffix}}${lib_suffix}"
                             ${filename}
                             ${${filename}})
    endif()
endmacro(FindHMLib_rename_lib_filename)

macro(FindHMLib modname target)

    # Set LIB_PREFIX to '/' to use libraries from '/lib'
    cmake_parse_arguments("_${modname}_hmfind" "" "INCLUDE;STATICLIB;SHAREDLIB;QUIET" "INCLUDE_PREFIXES;INCLUDE_SUBDIRS;LIB_PREFIXES;LIB_SUBDIRS" ${ARGN})

    FindHMLib_find_hmvariant_suffix(_${modname}_hmfind_hmvariant_suffix)

    if (DEFINED _${modname}_hmfind_INCLUDE_PREFIXES)
        set(${modname}_INCLUDE_PREFIXES "${_${modname}_hmfind_INCLUDE_PREFIXES}")
    else()
        set(${modname}_INCLUDE_PREFIXES "/usr")
    endif()
    if (DEFINED _${modname}_hmfind_LIB_PREFIXES)
        set(${modname}_LIB_PREFIXES "${_${modname}_hmfind_LIB_PREFIXES}")
    else()
        set(${modname}_LIB_PREFIXES "/usr")
    endif()
    if (NOT (DEFINED HMFIND_PATH_INCLUDE))
        set(HMFIND_PATH_INCLUDE "include")
        message(WARNING "${modname}: HMFIND_PATH_INCLUDE not provided, use 'include' by default")
    endif()
    if (NOT (DEFINED HMFIND_PATH_LIB))
        set(HMFIND_PATH_LIB "lib")
        message(WARNING "${modname}: HMFIND_PATH_LIB not provided, use 'lib' by default")
    endif()

    # FIXME: adjust include -> include32 / lib -> {lib32,libilp32}

    if (DEFINED _${modname}_hmfind_INCLUDE)
        FindHMLib_find_path(${modname}_INCLUDE_DIR
                            "${_${modname}_hmfind_INCLUDE}"
                            "${HMFIND_PATH_INCLUDE}"
                            PREFIXES ${${modname}_INCLUDE_PREFIXES}
                            SUBDIRS ${_${modname}_hmfind_INCLUDE_SUBDIRS})
        mark_as_advanced(${modname}_INCLUDE_DIR)

        set(${modname}_include_FIND_QUIETLY "${_${modname}_hmfind_QUIET}")
        find_package_handle_standard_args(${modname}_include
                                          REQUIRED_VARS
                                            "${modname}_INCLUDE_DIR"
                                            NAME_MISMATCHED)

        if (${modname}_include_FOUND AND NOT TARGET ${target}_include)
            add_library(${target}_include UNKNOWN IMPORTED)
            set_target_properties(${target}_include PROPERTIES
                                  INTERFACE_INCLUDE_DIRECTORIES ${${modname}_INCLUDE_DIR}
                                  IMPORTED_LINK_INTERFACE_LANGUAGES C)
        endif()
    endif()

    if (DEFINED _${modname}_hmfind_STATICLIB)
        if ("${_${modname}_hmfind_STATICLIB}" MATCHES
            "^${CMAKE_STATIC_LIBRARY_PREFIX}.*${CMAKE_STATIC_LIBRARY_SUFFIX}$")
            set("_${modname}_STATICLIB_FILENAME" "${_${modname}_hmfind_STATICLIB}")
        else()
            set("_${modname}_STATICLIB_FILENAME"
                "${CMAKE_STATIC_LIBRARY_PREFIX}${_${modname}_hmfind_STATICLIB}${CMAKE_STATIC_LIBRARY_SUFFIX}")
        endif()

        FindHMLib_rename_lib_filename(_${modname}_hmfind_hmvariant_suffix
                                      _${modname}_STATICLIB_FILENAME
                                      ${CMAKE_STATIC_LIBRARY_SUFFIX})

        FindHMLib_find_file(${modname}_STATICLIB
                           "${_${modname}_STATICLIB_FILENAME}"
                           "${HMFIND_PATH_LIB}"
                           PREFIXES ${${modname}_LIB_PREFIXES}
                           SUBDIRS ${_${modname}_hmfind_LIB_SUBDIRS})
        mark_as_advanced(${modname}_STATICLIB)

        set(${modname}_static_FIND_QUIETLY "${_${modname}_hmfind_QUIET}")
        find_package_handle_standard_args(${modname}_static
                                          REQUIRED_VARS
                                            ${modname}_STATICLIB
                                            NAME_MISMATCHED)

        if (${modname}_static_FOUND AND NOT TARGET ${target}_static)
            add_library(${target}_static STATIC IMPORTED)
            set_target_properties(${target}_static PROPERTIES
                                  IMPORTED_LOCATION ${${modname}_STATICLIB})
            if (${modname}_hmfind_include_FOUND)
                set_target_properties(${target}_static PROPERTIES
                                      INTERFACE_INCLUDE_DIRECTORIES ${${modname}_INCLUDE_DIR}
                                      IMPORTED_LINK_INTERFACE_LANGUAGES C)
                # Inherit INTERFACE_COMPILE_DEFINITIONS from inclib
                # By setting INTERFACE_COMPILE_DEFINITIONS from inclib, we can
                # add config options such as __hmkcfi__ and propagates to _static and _shared
                # targets
                set_property(${target}_static APPEND PROPERTY
                             INTERFACE_COMPILE_DEFINITIONS
                             $<TARGET_PROPERTY:${target}_include,INTERFACE_COMPILE_DEFINITIONS>)
            endif()
        endif()
    endif()

    if (DEFINED _${modname}_hmfind_SHAREDLIB)
        if ("${_${modname}_hmfind_SHAREDLIB}" MATCHES
            "^${CMAKE_SHARED_LIBRARY_PREFIX}.*${CMAKE_SHARED_LIBRARY_SUFFIX}$")
            set("_${modname}_SHAREDLIB_FILENAME" "${_${modname}_hmfind_SHAREDLIB}")
        else()
            set("_${modname}_SHAREDLIB_FILENAME"
                "${CMAKE_SHARED_LIBRARY_PREFIX}${_${modname}_hmfind_SHAREDLIB}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        endif()

        FindHMLib_rename_lib_filename(_${modname}_hmfind_hmvariant_suffix
                                      _${modname}_SHAREDLIB_FILENAME
                                      ${CMAKE_SHARED_LIBRARY_SUFFIX})

        FindHMLib_find_file(${modname}_SHAREDLIB
                            "${_${modname}_SHAREDLIB_FILENAME}"
                            "${HMFIND_PATH_LIB}"
                            PREFIXES ${${modname}_LIB_PREFIXES}
                            SUBDIRS ${_${modname}_hmfind_LIB_SUBDIRS})
        mark_as_advanced(${modname}_SHAREDLIB)

        set(${modname}_shared_FIND_QUIETLY "${_${modname}_hmfind_QUIET}")
        find_package_handle_standard_args(${modname}_shared
                                          REQUIRED_VARS
                                            ${modname}_SHAREDLIB
                                            NAME_MISMATCHED)

        if (${modname}_shared_FOUND AND NOT TARGET ${target}_shared)
            add_library(${target}_shared INTERFACE IMPORTED)

            get_filename_component(_${modname}_LINKDIR
                                   "${${modname}_SHAREDLIB}" DIRECTORY)
            set_target_properties(${target}_shared PROPERTIES
                                  INTERFACE_LINK_DIRECTORIES "${_${modname}_LINKDIR}"
                                  INTERFACE_LINK_LIBRARIES "${_${modname}_SHAREDLIB_FILENAME}")

            if (${modname}_hmfind_include_FOUND)
                set_target_properties(${target}_shared PROPERTIES
                                      INTERFACE_INCLUDE_DIRECTORIES ${${modname}_INCLUDE_DIR})
                set_property(${target}_shared APPEND PROPERTY
                             INTERFACE_COMPILE_DEFINITIONS
                             $<TARGET_PROPERTY:${target}_include,INTERFACE_COMPILE_DEFINITIONS>)
            endif()
        endif()
    endif()
endmacro(FindHMLib)

# vim:ts=4:sw=4:expandtab
