# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Add dynamic library to target
# Author: Huawei OS Kernel Lab
# Create: Thu Sep 02 09:00:50 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Targets)
include(HongmengCMakeNG/HMbuild/Functions/SetLibVersion)
include(HongmengCMakeNG/HMbuild/PostBuild/Utils)

function(hmbuild_dynamic_library_filename outvar basename)
    set(${outvar}
            "${CMAKE_SHARED_LIBRARY_PREFIX}${basename}${CMAKE_SHARED_LIBRARY_SUFFIX}"
            PARENT_SCOPE)
endfunction(hmbuild_dynamic_library_filename)

function(hmbuild_add_dynamic_library target)
    hmbuild_target_output_name_of(_output_name "${target}")
    if (NOT _output_name)
        set(_output_name "${target}")
    endif()

    # Suffix should be a part of soname, append output_name with it
    set(output_name       ${_output_name}${CONFIG_HMBUILD_OUTPUT_SUFFIX})
    set(output_name_devel ${output_name}.unstripped)

    hmbuild_dynamic_library_filename(filename_release ${output_name})
    hmbuild_dynamic_library_filename(filename_devel   ${output_name_devel})
    hmcmakeng_debug("hmbuild_add_dynamic_library: filename_release=${filename_release}, filename_devel=${filename_devel}")
    hmbuild_libversion_config_of(version "${target}")
    hmcmakeng_debug("hmbuild_add_dynamic_library: version_major=${version_major}, version_minor=${version_minor}")

    hmbuild_cache_val_of(srcs "src_${target}")
    if (srcs)
        add_library(${target} SHARED ${srcs})
    else()
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${target}_null.c "")
        add_library(${target} SHARED ${target}_null.c)
    endif()

    hmbuild_config_target(${target}
        PROPERTY HMBUILD_CMAKETARGET_RAW ${target})

    target_compile_options(${target} PRIVATE "-D__hmbuild_target_is_shared__")
    set_target_properties("${target}" PROPERTIES
                          OUTPUT_NAME "${output_name}"
                          SOVERSION "${version_major}"
                          VERSION "${version_major}.${version_minor}")

    hmbuild_target_set_cmakename("${target}" "${target}")

    set(filepath_devel "${CMAKE_CURRENT_BINARY_DIR}/${filename_devel}")

    hmbuild_postbuild(${target} postbuild_tail_target)

    set(RELEASE_FILES_LIST $<TARGET_FILE:${target}> $<TARGET_SONAME_FILE:${target}> $<TARGET_LINKER_FILE:${target}>)
    add_custom_target(__build_${target} ALL DEPENDS ${postbuild_tail_target})
    set_target_properties(__build_${target} PROPERTIES
                          "hmprop_output_release" "${RELEASE_FILES_LIST}"
                          "hmprop_output_devel" "${filepath_devel}")

    # For dynamic library, add builtin_fpic to it
    hmbuild_target_link_builtin(${target} FPIC)
    hmcmakeng_debug("hmbuild_add_dynamic_library: ${target} depends on builtin target ${builtin_tgt}")

    hmbuild_config_target(${target}
        PROPERTY HMBUILD_CMAKETARGET_FINAL __build_${target})
endfunction(hmbuild_add_dynamic_library)

# vim:ts=4:sw=4:expandtab
