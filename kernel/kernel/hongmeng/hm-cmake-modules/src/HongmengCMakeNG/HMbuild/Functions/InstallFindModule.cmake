# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Install find module to cmake Module
# Author: Huawei OS Kernel Lab
# Create: Wed Aug 11 22:19:24 2021

include_guard(GLOBAL)

function(hmbuild_installfindmodule file)
    # Directly install findmodule because findmodule
    # is not generated
    hmcmakeng_debug("hmbuild_installfindmodule: file=${file}")
    if (NOT (DEFINED HMBUILD_MAKE_HEADERS))
        return()
    endif()
    install(FILES
              ${CMAKE_CURRENT_SOURCE_DIR}/${file}
            DESTINATION
            "${CONFIG_HMBUILD_INSTALL_DIR_DATADIR}/cmake/Modules"
            COMPONENT
              "headers")
endfunction(hmbuild_installfindmodule)

# vim:ts=4:sw=4:expandtab
