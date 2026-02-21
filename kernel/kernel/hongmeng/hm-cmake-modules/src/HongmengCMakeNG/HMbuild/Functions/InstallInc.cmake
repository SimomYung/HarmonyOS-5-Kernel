# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: hmbuild installinc related functions
# Author: Huawei OS Kernel Lab
# Create: Wed Aug 11 18:09:43 2021

include_guard(GLOBAL)

function(hmbuild_append_installinc_list)
    cmake_parse_arguments (PARSE_ARGV 0 _installinc "" "HEADER;SUBDIR" "")

    hmcmakeng_debug("hmbuild_installinc: header=${_installinc_HEADER}")
    if (NOT (DEFINED HMBUILD_MAKE_HEADERS))
        return()
    endif()

    hmbuild_cache_append_to(installinc_incs "${_installinc_HEADER}")
    hmbuild_cache_append_to(installinc_inc_subdir_${_installinc_HEADER}
                            "${_installinc_SUBDIR}")
endfunction(hmbuild_append_installinc_list)

function(hmbuild_installinc_from_src outvar inc subdir)
    set(tryinc ${CMAKE_CURRENT_SOURCE_DIR}/${inc})
    if (IS_SYMLINK ${tryinc})
        install(FILES ${tryinc}
                DESTINATION
                  "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/${subdir}"
                COMPONENT "headers")
        set(${outvar} "TRUE" PARENT_SCOPE)
        return()
    elseif(EXISTS ${tryinc})
        if (IS_DIRECTORY ${tryinc})
            install(DIRECTORY ${tryinc}
                    DESTINATION
                      "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/${subdir}"
                    COMPONENT "headers")
            set(${outvar} "TRUE" PARENT_SCOPE)
            return()
        endif()
        install(FILES ${tryinc}
                DESTINATION
                  "${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/${subdir}"
                COMPONENT "headers")
        set(${outvar} "TRUE" PARENT_SCOPE)
        return()
    endif()
    set(${outvar} "FALSE" PARENT_SCOPE)
endfunction(hmbuild_installinc_from_src)

function(hmbuild_installinc_from_binary inc subdir)
    set(tryinc ${CMAKE_CURRENT_BINARY_DIR}/${inc})
    install(CODE "
            if (IS_SYMLINK ${tryinc})
                file(INSTALL DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/${subdir}
                     TYPE FILE FILES ${tryinc})
            elseif (EXISTS ${tryinc})
                if (IS_DIRECTORY ${tryinc})
                    file(INSTALL DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/${subdir}
                         TYPE DIRECTORY FILES ${tryinc})
                else()
                    file(INSTALL DESTINATION ${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/${subdir}
                         TYPE FILE FILES ${tryinc})
                endif()
            else()
                message(FATAL_ERROR \"Install error: ${tryinc} not found\")
            endif()
            "
            COMPONENT "headers")
endfunction(hmbuild_installinc_from_binary)

function(hmbuild_installinc inc)
    hmcmakeng_debug("hmbuild_installinc: inc=${inc}")
    if (NOT (DEFINED HMBUILD_MAKE_HEADERS))
        return()
    endif()

    hmbuild_cache_val_of(subdir installinc_inc_subdir_${inc})
    hmcmakeng_debug("installinc: ${inc} (subdir=${subdir})")

    hmbuild_installinc_from_src(res "${inc}" "${subdir}")
    if (NOT ${res})
        hmcmakeng_debug("installinc from binary dir: ${inc} (subdir=${subdir})")
        hmbuild_installinc_from_binary("${inc}" "${subdir}")
    else()
        hmcmakeng_debug("installinc from source dir: ${inc} (subdir=${subdir})")
    endif()
endfunction(hmbuild_installinc)

function(hmbuild_install_all_includes)
    hmbuild_cache_val_of(includes installinc_incs)
    foreach(inc ${includes})
        hmbuild_installinc(${inc})
    endforeach()
endfunction(hmbuild_install_all_includes)

# vim:ts=4:sw=4:expandtab
