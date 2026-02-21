# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild hmbuild_add_library function
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 13:20:30 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Targets)
include(HongmengCMakeNG/HMbuild/Cache)
include(FindUnixCommands)

function(hmbuild_static_library_filename outvar basename)
    set(${outvar}
            "${CMAKE_STATIC_LIBRARY_PREFIX}${basename}${CMAKE_STATIC_LIBRARY_SUFFIX}"
            PARENT_SCOPE)
endfunction(hmbuild_static_library_filename)

function(hmbuild_is_cflags_contain_fpic target is_fpic)
    hmbuild_cache_val_of(cflags_list cflags_list_${target})
    hmcmakeng_debug("hmbuild_cflags_config_target: cflags_list_${target}=${cflags_list}")
    if ("${cflags_list}" MATCHES "-fPIC")
        set(${is_fpic} true PARENT_SCOPE)
    else()
        set(${is_fpic} false PARENT_SCOPE)
    endif()
endfunction(hmbuild_is_cflags_contain_fpic)

function(hmbuild_add_static_library target)
    hmbuild_target_output_name_of(_output_name "${target}")
    if (NOT _output_name)
        set(_output_name "${target}")
    endif()

    # Yocto check _nonshared.a in package_qa_check_staticdev
    set(output_name        ${_output_name}${CONFIG_HMBUILD_OUTPUT_SUFFIX})
    set(targetname_devel   ${target}.dev)
    set(outputname_devel   ${output_name}.dev)
    set(targetname_release ${target})
    set(outputname_release ${output_name})

    hmbuild_static_library_filename(filename_release   ${output_name})

    hmcmakeng_debug("hmbuild_add_static_library: filename_release=${filename_release}, targetname_devel=${targetname_devel}")

    set(filepath_release "${CMAKE_CURRENT_BINARY_DIR}/${filename_release}")

    hmbuild_cache_val_of(srcs "src_${target}")
    if (srcs)
        add_library(${targetname_devel} STATIC ${srcs})
    else()
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${target}_null.c "")
        add_library(${targetname_devel} STATIC ${target}_null.c)
    endif()

    hmbuild_config_target(${target}
        PROPERTY HMBUILD_CMAKETARGET_RAW ${targetname_devel})

    target_compile_options(${targetname_devel} PRIVATE "-D__hmbuild_target_is_static__")
    set_target_properties("${targetname_devel}" PROPERTIES OUTPUT_NAME "${outputname_devel}")

    hmbuild_target_set_cmakename("${target}" "${targetname_devel}")

    hmbuild_create_regular_archive(${filepath_release} ${targetname_devel})

    add_custom_target(__build_${targetname_release} ALL DEPENDS ${filepath_release})
    set_target_properties(__build_${targetname_release}    PROPERTIES
                          "hmprop_output_release" ${filepath_release})

    # Depending on whether the cflags contains fpic, decide which builtin to use.
    hmbuild_is_cflags_contain_fpic(${target} is_contain_fpic)
    if (is_contain_fpic)
        hmbuild_fpic_builtin_target_name(builtin_name)
    else()
        hmbuild_builtin_target_name(builtin_name)
    endif()
    hmbuild_target_cmakename_of(builtin_tgt ${builtin_name})
    # Do not use target_link_libraries because we want to 'link'
    # static library to another static library, which is unusual.
    # Similar to AddSubdir.cmake, use STATIC_LIBRARY_OPTIONS.
    set_property(TARGET ${targetname_devel} APPEND PROPERTY STATIC_LIBRARY_OPTIONS
                     "$<TARGET_FILE:${builtin_tgt}>")
    set_property(TARGET ${targetname_devel} PROPERTY LINK_DEPENDS
                     "$<TARGET_FILE:${builtin_tgt}>")

    add_dependencies(${targetname_devel} ${builtin_tgt})
    hmcmakeng_debug("hmbuild_add_static_library: ${targetname_devel} depends on builtin target ${builtin_tgt}")

    hmbuild_config_target(${target}
        PROPERTY HMBUILD_CMAKETARGET_FINAL __build_${targetname_release})
endfunction(hmbuild_add_static_library)

# vim:ts=4:sw=4:expandtab
