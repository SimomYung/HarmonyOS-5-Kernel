# Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: HMbuild post-build module Strip
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 8 10:47:33 2022

function(hmbuild_postbuild_strip target tail_target_var)
    hmbuild_target_output_name_of(_output_name ${target})
    if (NOT _output_name)
        set(_output_name ${target})
    endif()

    hmbuild_target_cmakename_of(cmake_target ${target})

    set(copy_src "$<TARGET_FILE:${cmake_target}>")
    get_target_property(_type ${cmake_target} TYPE)
    if (_type STREQUAL "EXECUTABLE")
        hmbuild_exec_name(filename_release ${_output_name} "${CONFIG_HMBUILD_OUTPUT_SUFFIX}" "")
        set(copy_dst "${CMAKE_CURRENT_BINARY_DIR}/${filename_release}")
        set(strip_dst "${copy_dst}")
    elseif(_type STREQUAL "SHARED_LIBRARY")
        hmbuild_dynamic_library_filename(filename_devel ${_output_name}${CONFIG_HMBUILD_OUTPUT_SUFFIX}.unstripped)
        # the release file name of shared library is same with the default output of cmake_target: `$<TARGET_FILE:${cmake_target}>`
        # So after copy out to the ${filepath_devel} file, we just strip the default output.
        # The cmake_target name of elf is with suffix "unstripped", but this is NOT TRUE for shared library.
        # The deep reason is related with the version handling of shared library.
        set(copy_dst "${CMAKE_CURRENT_BINARY_DIR}/${filename_devel}")
        set(strip_dst "${copy_src}") # unstripped source file have the same name with release file
    endif()
    if (_type STREQUAL "EXECUTABLE" OR _type STREQUAL "SHARED_LIBRARY")
        add_custom_target(${cmake_target}.strip ALL
                          COMMAND cmake -E copy ${copy_src} ${copy_dst}
                          COMMAND ${CMAKE_STRIP} -D ${CONFIG_HMBUILD_STRIP_OPTION} ${strip_dst}
                          DEPENDS ${${tail_target_var}})
        set(${tail_target_var} ${cmake_target}.strip PARENT_SCOPE)
    endif()
endfunction(hmbuild_postbuild_strip)

hmbuild_postbuild_strip(${target} tail_target)

# vim:ts=4:sw=4:expandtab
