# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: HMbuild hmbuild_add_library function
# Author: Huawei OS Kernel Lab
# Create: Wed May 26 13:20:30 2021

include_guard(GLOBAL)

include(HongmengCMakeNG/Log)
include(HongmengCMakeNG/PreHmbuild/InterfaceTarget)
include(HongmengCMakeNG/HMbuild/Targets)
include(HongmengCMakeNG/HMbuild/Cache)
include(HongmengCMakeNG/HMbuild/Builtin)

function(hmbuild_builtin_library_filename outvar basename)
    set(${outvar}
            "${CMAKE_STATIC_LIBRARY_PREFIX}${basename}${CMAKE_STATIC_LIBRARY_SUFFIX}"
            PARENT_SCOPE)
endfunction(hmbuild_builtin_library_filename)

function(hmbuild_target_link_builtin target)
    cmake_parse_arguments(PARSE_ARGV 0 _linkbuiltin "FPIC" "" "")
    hmbuild_target_cmakename_of(cmake_target ${target})

    if(_linkbuiltin_FPIC)
        hmbuild_fpic_builtin_target_name(builtin_name)
    else()
        hmbuild_builtin_target_name(builtin_name)
    endif()

    hmbuild_get_interface_target_property(linkWhole ${target} HMBUILD_LINK_WHOLE_BUILTIN)
    hmbuild_target_cmakename_of(builtin_tgt ${builtin_name})
    # Fpic always link with whole target
    if (linkWhole OR _linkbuiltin_FPIC)
        target_link_libraries(${cmake_target} PRIVATE  
                              "-Wl,--whole-archive $<TARGET_LINKER_FILE:${builtin_tgt}> -Wl,--no-whole-archive")
    else()
        target_link_libraries(${cmake_target} PRIVATE ${builtin_tgt})
    endif()

    set_property(TARGET ${cmake_target} PROPERTY LINK_DEPENDS
                 "$<TARGET_FILE:${builtin_tgt}>")

    add_dependencies(${cmake_target} ${builtin_tgt})
endfunction(hmbuild_target_link_builtin)

function(hmbuild_add_builtin_library)
    cmake_parse_arguments(PARSE_ARGV 0 _addbuiltin "FPIC" "" "")

    if(_addbuiltin_FPIC)
        hmbuild_fpic_builtin_target_name(target)
        set(_output_name  "builtin_fpic")
    else()
        hmbuild_builtin_target_name(target)
        set(_output_name  "builtin")
    endif()
    hmcmakeng_debug("hmbuild_add_builtin_library: target=${target} FPIC=${_addbuiltin_FPIC}")

    set(output_name      ${_output_name}${CONFIG_HMBUILD_OUTPUT_SUFFIX})
    set(outputname_devel ${output_name}.dev)
    set(targetname_devel ${target}.dev)
    hmbuild_builtin_library_filename(filename_release   ${output_name})

    hmcmakeng_debug("hmbuild_add_builtin_library:targetname_devel=${targetname_devel}")

    hmbuild_cache_val_of(srcs "src_${target}")
    if (srcs)
        add_library(${targetname_devel} STATIC EXCLUDE_FROM_ALL ${srcs})
    else()
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${target}_null.c "")
        add_library(${targetname_devel} STATIC EXCLUDE_FROM_ALL ${target}_null.c)
    endif()
    if(_addbuiltin_FPIC)
	target_compile_options(${targetname_devel} PRIVATE "${CMAKE_C_COMPILE_OPTIONS_PIC}")
	target_compile_options(${targetname_devel} PRIVATE "-D__hmbuild_target_is_shared__")
    else()
	target_compile_options(${targetname_devel} PRIVATE "-D__hmbuild_target_is_static__")
    endif()

    set_target_properties("${targetname_devel}" PROPERTIES OUTPUT_NAME "${outputname_devel}")

    hmbuild_target_set_cmakename("${target}" "${targetname_devel}")
    hmbuild_config_target(${target} PROPERTY HMBUILD_CMAKETARGET_RAW ${targetname_devel})
endfunction(hmbuild_add_builtin_library)

function(__hmbuild_append_target_builtin targets)
    hmcmakeng_debug("__hmbuild_append_builtin_list: targets=${targets}")
    foreach(t ${targets})
        hmbuild_cache_append_to(targets_builtin "${t}")
    endforeach()
endfunction(__hmbuild_append_target_builtin)

function(hmbuild_add_all_builtin_targets)
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()
    hmbuild_add_builtin_library()
    hmbuild_add_builtin_library(FPIC)

    # Append to hmcache list
    hmbuild_builtin_all_target_name(target_list)
    __hmbuild_append_target_builtin("${target_list}")
endfunction(hmbuild_add_all_builtin_targets)

function(hmbuild_add_subdir_builtin_dependency subdir)
    cmake_parse_arguments(PARSE_ARGV 0 _addbuiltindep "FPIC" "" "")

    if(_addbuiltindep_FPIC)
        hmbuild_fpic_builtin_target_name(builtinlib)
        hmbuild_fpic_builtin_target_name(subdir_builtinlib SUBDIR "${subdir}")
    else()
        hmbuild_builtin_target_name(builtinlib)
        hmbuild_builtin_target_name(subdir_builtinlib SUBDIR "${subdir}")
    endif()

    hmbuild_target_cmakename_of(tgt         "${builtinlib}")
    hmbuild_target_cmakename_of(subdir_tgt  "${subdir_builtinlib}" SUBDIR "${subdir}")

    if ((NOT tgt) OR (NOT (TARGET ${tgt})))
        hmcmakeng_error("add subdir: builtinlib=${builtinlib}, cmakename is '${tgt}' is not a target")
    endif()

    set_property(TARGET ${tgt} APPEND PROPERTY STATIC_LIBRARY_OPTIONS
                 "$<TARGET_FILE:${subdir_tgt}>")
    set_property(TARGET ${tgt} APPEND PROPERTY LINK_DEPENDS
                 "$<TARGET_FILE:${subdir_tgt}>")

    add_dependencies(${tgt} ${subdir_tgt})
    hmcmakeng_trace("target_link_libraries: tgt=${tgt}, subdir_tgt=${subdir_tgt}, subdir_builtinlib=${subdir_builtinlib} subdir=${subdir}")
endfunction(hmbuild_add_subdir_builtin_dependency)

function(hmbuild_add_all_builtin_subdir subdir)
    if (NOT (DEFINED HMBUILD_MAKE_BINARIES))
        return()
    endif()
    hmbuild_add_subdir_builtin_dependency(${subdir})
    hmbuild_add_subdir_builtin_dependency(${subdir} FPIC)
endfunction(hmbuild_add_all_builtin_subdir)
# vim:ts=4:sw=4:expandtab
