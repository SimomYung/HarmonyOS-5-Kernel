# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: HMbuild post-build module XOM
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 8 10:47:33 2022

function(hmbuild_postbuild_xom target tail_target_var)
    if (NOT HMBUILD_FEATURE_XOM OR NOT "${CMAKE_C_FLAGS}" MATCHES ".*\\-\\-xom\\-march64.*")
        return()
    endif()

    hmbuild_target_cmakename_of(cmake_target ${target})

    get_target_property(_type ${cmake_target} TYPE)
    hmbuild_postbuild_get_arg("XOM" "TOOL_PATH" XOM_ELF)
    if (NOT XOM_ELF)
        return()
    endif()
    if (_type STREQUAL "EXECUTABLE" OR _type STREQUAL "SHARED_LIBRARY")
        add_custom_target(${cmake_target}.xom ALL
                      COMMAND ${BASH} -c "${XOM_ELF} $<TARGET_FILE:${cmake_target}>"
                      DEPENDS ${${tail_target_var}})
        set(${tail_target_var} ${cmake_target}.xom PARENT_SCOPE)
    endif()
endfunction(hmbuild_postbuild_xom)

hmbuild_postbuild_xom(${target} tail_target)

# vim:ts=4:sw=4:expandtab
