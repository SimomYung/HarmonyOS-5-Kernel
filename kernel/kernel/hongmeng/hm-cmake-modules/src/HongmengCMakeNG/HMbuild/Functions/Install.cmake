# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: hmbuild_install function
# Author: Huawei OS Kernel Lab
# Create: Thu May 27 10:37:05 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Install)

function(hmbuild_install target is_lib)
    hmcmakeng_debug("hmbuild_install: target=${target}, is_lib=${is_lib}")
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()

    hmbuild_cache_val_of(destdirvar install_destdirvar_${target})

    get_target_property(output_release __build_${target} "hmprop_output_release")
    get_target_property(output_devel   __build_${target} "hmprop_output_devel")

    hmcmakeng_status("Install: ${t} (${HMBUILD_DIR_PREFIX_VERBOSE})")

    if (NOT is_lib)
        install(PROGRAMS ${output_release}
            DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_${destdirvar}}"
            COMPONENT "binaries")
        install(PROGRAMS ${output_devel}
            DESTINATION "/debug/${CONFIG_HMBUILD_INSTALL_DIR_${destdirvar}}"
            COMPONENT "binaries")
    else()
        install(FILES ${output_release}
            DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_${destdirvar}}"
            COMPONENT "libraries")
        # static libraries provide no devel library through hmprop_output_devel
        if (NOT ("${output_devel}" MATCHES "NOTFOUND$"))
            install(FILES ${output_devel}
                DESTINATION "/debug/${CONFIG_HMBUILD_INSTALL_DIR_${destdirvar}}"
                COMPONENT "libraries")
        endif()
    endif()

endfunction(hmbuild_install)

function(hmbuild_append_install_list)
    cmake_parse_arguments (PARSE_ARGV 0 _install "" "TARGET;DESTDIRVAR;CONDITION" "")
    hmcmakeng_debug("hmbuild_append_install_list: target=${_install_TARGET}, destdirvar=${_install_DESTDIRVAR} cond=${_install_CONDITION}")

    # Check condition
    hmbuild_check_condition(checkcond "${_install_CONDITION}")
    if (NOT ${checkcond})
        return()
    endif()

    hmbuild_cache_append_to(install_targets "${_install_TARGET}")
    hmbuild_cache_append_to(install_destdirvar_${_install_TARGET} "${_install_DESTDIRVAR}")
endfunction(hmbuild_append_install_list)

function(hmbuild_install_all_targets)
    hmbuild_cache_val_of(targets install_targets)
    hmbuild_cache_val_of(_targets_lib_static targets_lib_static)
    hmbuild_cache_val_of(_targets_lib_dynamic targets_lib_dynamic)
    foreach(t ${targets})
        if((t IN_LIST _targets_lib_static) OR (t IN_LIST _targets_lib_dynamic))
            hmbuild_install(${t} "TRUE")
        else()
            hmbuild_install(${t} "FALSE")
        endif()
    endforeach()
endfunction(hmbuild_install_all_targets)

# vim:ts=4:sw=4:expandtab
