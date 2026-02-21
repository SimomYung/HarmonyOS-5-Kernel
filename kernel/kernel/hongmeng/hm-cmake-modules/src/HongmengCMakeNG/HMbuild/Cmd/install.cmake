# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'install-' command
# Author: Huawei OS Kernel Lab
# Create: Thu May 27 09:55:26 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Install)
include(HongmengCMakeNG/HMbuild/Functions/Install)

# install-mylib-y: destdir
function(hmbuild_cmd_install)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG")
    hmcmakeng_debug("hmbuild_cmd_install, CMDLIST=${_cmd_CMDLIST}, ARG=${_cmd_ARG}")

    list(GET        _cmd_CMDLIST 1      target)
    list(SUBLIST    _cmd_CMDLIST 2 -1   options)

    foreach(o IN LISTS options)
        if ("${o}" STREQUAL "y")
            set(cond "${o}")
        elseif ("${o}" MATCHES "^CONFIG_.*")
            set(cond "${o}")
        else()
            hmcmakeng_error("Unknown option in 'install' command")
        endif()
    endforeach()

    if (NOT (DEFINED CONFIG_HMBUILD_INSTALL_DIR_${_cmd_ARG}))
        hmcmakeng_error("Dest directory CONFIG_HMBUILD_INSTALL_DIR_${_cmd_ARG} not defined")
    endif()

    hmbuild_append_install_list(TARGET      ${target}
                                DESTDIRVAR  ${_cmd_ARG}
                                CONDITION   ${cond})

endfunction(hmbuild_cmd_install)

# vim:ts=4:sw=4:expandtab
