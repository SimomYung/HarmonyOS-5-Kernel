# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Kbuild for HMCmakeNG
# Author: Huawei OS Kernel Lab
# Create: Thu May 20 11:04:32 2021

include(HongmengCMakeNG/Log)

function(findcmd_kbuild_standalone cmd variable)
    find_program(${variable}
                 NAMES ${cmd})
    if (NOT ${variable})
        hmcmakeng_error("${cmd} not found")
    else()
        hmcmakeng_status("${cmd} found at ${${variable}}")
    endif()
endfunction(findcmd_kbuild_standalone)

function(find_cmakeng_kconfig outvar)
    foreach(path IN LISTS CMAKE_MODULE_PATH)
        find_path(cmakengroot "HongmengCMakeNGKconfig"
                  NO_DEFAULT_PATH
                  HINTS "${path}/HongmengCMakeNG/")
        if (cmakengroot)
            hmcmakeng_debug("HongmengCMakeNGKconfig found: ${cmakengroot}")
            break()
        else()
            hmcmakeng_debug("HongmengCMakeNGKconfig not found in ${cmakengroot}")
        endif()
    endforeach()

    if (NOT cmakengroot)
        hmcmakeng_error("HongmengCMakeNGKconfig not found, checked path: ${CMAKE_MODULE_PATH}")
    else()
        set(${outvar} ${cmakengroot}/HongmengCMakeNGKconfig PARENT_SCOPE)
    endif()
endfunction(find_cmakeng_kconfig)

findcmd_kbuild_standalone(kbuild-conf  HMCMAKENG_KBUILD_CONF_EXECUTABLE)
findcmd_kbuild_standalone(kbuild-mconf HMCMAKENG_KBUILD_MCONF_EXECUTABLE)
find_cmakeng_kconfig(cmakeng_kconfig)

hmcmakeng_debug("cmakeng_kconfig is ${cmakeng_kconfig}")

set(HMCMAKENG_KBUILD_DOT_CONFIG             ${CMAKE_BINARY_DIR}/.config)
set(HMCMAKENG_KBUILD_CFLAGS_INCAUTOCONF     "-include ${CMAKE_BINARY_DIR}/include/generated/autoconf.h")

set(ENV{HMCMAKENG_KCONFIG} "${cmakeng_kconfig}")

# Build action always in a standalone directory
# Need set srctree so that kbuild tools can find correct source:relative path
set(ENV{srctree} "${CMAKE_SOURCE_DIR}")
# Force fixing .config
if (NOT EXISTS ${HMCMAKENG_KBUILD_DOT_CONFIG})
    exec_program(${HMCMAKENG_KBUILD_CONF_EXECUTABLE} ARGS --alldefconfig ${CMAKE_SOURCE_DIR}/Kconfig)
endif()
exec_program(${HMCMAKENG_KBUILD_CONF_EXECUTABLE} ARGS --olddefconfig ${CMAKE_SOURCE_DIR}/Kconfig)
exec_program(${HMCMAKENG_KBUILD_CONF_EXECUTABLE} ARGS --syncconfig ${CMAKE_SOURCE_DIR}/Kconfig)

set(HMSD_CONFIGS "")
# Read from .config
file(STRINGS ${HMCMAKENG_KBUILD_DOT_CONFIG} dotconfigcontent REGEX "^CONFIG.*=.*$")
foreach (c ${dotconfigcontent})
    string(REGEX REPLACE "^(CONFIG[^=]*)=.*$" "\\1" k   "${c}")
    string(REGEX REPLACE "^CONFIG[^=]*=(.*)$" "\\1" _v  "${c}")

    if ("${_v}" MATCHES "^\".*\"$")
        string(REGEX REPLACE "^\"(.*)\"$" "\\1" v "${_v}")
    else()
        set(v "${_v}")
    endif()
    set(${k} "${v}")
    set(HMSD_CONFIGS "${HMSD_CONFIGS} ${k}")
endforeach()

if (NOT DEFINED CONFIG_HMBUILD_KCONFIG_INCLUDED)
    hmcmakeng_error("HongmengCMakeNGKconfig is not included in Kconfig. Add 'source \"$(HMCMAKENG_KCONFIG)\"' to Kconfig")
endif()
# vim:ts=4:sw=4:expandtab
