# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Parser of HMbuild 'lib-' command
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 10:11:38 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Functions/AddLib)

# lib-static-y: libname
# lib-dynamic-CONFIG_XXX: libname
function(hmbuild_cmd_lib)
    cmake_parse_arguments (PARSE_ARGV 0 _cmd "" "" "CMDLIST;ARG")
    hmcmakeng_debug("hmbuild_cmd_lib, CMDLIST=${_cmd_CMDLIST}, ARG=${_cmd_ARG}")

    list(GET     _cmd_CMDLIST 1     type)
    list(SUBLIST _cmd_CMDLIST 2 -1  options)

    set(_avail_types "static;dynamic")
    if (NOT type IN_LIST _avail_types)
        hmcmakeng_error("hmbuild_cmd_lib: type ${type} invalid")
    endif()

    foreach(o IN LISTS options)
        if ("${o}" STREQUAL "y")
            set(cond "${o}")
        elseif ("${o}" MATCHES "^CONFIG_.*")
            set(cond "${o}")
        else()
            hmcmakeng_error("Unknown option in 'lib' command")
        endif()
    endforeach()

    hmcmakeng_debug("options=${options}, cond=${cond}")
    hmbuild_append_lib_list(TARGET      ${_cmd_ARG}
                            TYPE        ${type}
                            CONDITION   ${cond})

endfunction()

# vim:ts=4:sw=4:expandtab
