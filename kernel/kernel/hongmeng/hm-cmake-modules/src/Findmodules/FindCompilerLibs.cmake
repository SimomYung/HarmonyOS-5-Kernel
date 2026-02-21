# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: FindCompilerLibs cmake module
# Author: Huawei OS Kernel Lab
# Create: Thu Jun 03 14:05:17 2021

# NOTE that this file should be installed by toolchain

#[=======================================================================[.rst:
FindCompilerLibs

Variables defined by the module
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Result variables
""""""""""""""""

CompilerLibs_SHARED_LIBRARY
CompilerLibs_STATIC_LIBRARY

Imported Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``CompilerLibs::CompilerLibs_shared``
and ``CompilerLibs::CompilerLibs_static``, if compiler runtime libraries(libgcc or
libclang_rt.builtins) are found.

#]=======================================================================]

# Provide gcc_print_filename in GNU and iTrustee toolchain

if (NOT ("${CMAKE_C_COMPILER_ID}" STREQUAL ""))
    include(FindCompilerLibs_${CMAKE_C_COMPILER_ID})
else()
    function(gcc_print_filename outvar name)
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
    endfunction(gcc_print_filename)
endif()

gcc_print_filename(LibGcc_STATIC_LIBRARY_FILE     libgcc.a)
gcc_print_filename(LibClangRT_STATIC_LIBRARY_FILE libclang_rt.builtins.a)
if (LibClangRT_STATIC_LIBRARY_FILE)
    set(CompilerLibs_STATIC_LIBRARY "${LibClangRT_STATIC_LIBRARY_FILE}")
elseif (LibGcc_STATIC_LIBRARY_FILE)
    set(CompilerLibs_STATIC_LIBRARY "${LibGcc_STATIC_LIBRARY_FILE}")
endif()

gcc_print_filename(LibGcc_SHARED_LIBRARY_FILE     libgcc_s.so)
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "LLVM")
    if (CONFIG_HMBUILD_TOOLCHAIN_BISHENG_MOBILE_CPU)
        # bisheng mobile cpu does not provide libclang_rt and libunwind shared library
        gcc_print_filename(LibUnwind_SHARED_LIBRARY_FILE  libunwind.a)
        gcc_print_filename(LibClangRT_SHARED_LIBRARY_FILE libclang_rt.builtins.a)
    else()
        gcc_print_filename(LibUnwind_SHARED_LIBRARY_FILE  libunwind.so)
        gcc_print_filename(LibClangRT_SHARED_LIBRARY_FILE libunwind.so)
    endif()
else()
    gcc_print_filename(LibUnwind_SHARED_LIBRARY_FILE  libunwind_s.so)
    gcc_print_filename(LibClangRT_SHARED_LIBRARY_FILE libclang_rt.builtins_s.so)
endif()
if (LibClangRT_SHARED_LIBRARY_FILE AND LibUnwind_SHARED_LIBRARY_FILE)
    if ("${CMAKE_C_COMPILER_ID}" STREQUAL "LLVM")
        if (CONFIG_HMBUILD_TOOLCHAIN_BISHENG_MOBILE_CPU)
            set(CompilerLibs_SHARED_LIBRARY "-Wl,--push-state,--as-needed -lclang_rt.builtins -lunwind -Wl,--pop-state")
        else()
            set(CompilerLibs_SHARED_LIBRARY "-Wl,--push-state,--as-needed -lunwind -Wl,--pop-state")
        endif()
    else()
        set(CompilerLibs_SHARED_LIBRARY "-Wl,--push-state,--as-needed -lclang_rt.builtins_s -lunwind_s -Wl,--pop-state")
    endif()
elseif (LibGcc_SHARED_LIBRARY_FILE)
    set(CompilerLibs_SHARED_LIBRARY "-Wl,--push-state,--as-needed -lgcc_s -Wl,--pop-state")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CompilerLibs
                                  REQUIRED_VARS
                                    CompilerLibs_SHARED_LIBRARY
                                    CompilerLibs_STATIC_LIBRARY
                                    NAME_MISMATCHED)

if (CompilerLibs_FOUND AND NOT TARGET CompilerLibs::CompilerLibs_shared)
    add_library(CompilerLibs::CompilerLibs_shared INTERFACE IMPORTED)
    set_target_properties(CompilerLibs::CompilerLibs_shared PROPERTIES
                          INTERFACE_LINK_LIBRARIES "${CompilerLibs_SHARED_LIBRARY}")
endif()

if (CompilerLibs_FOUND AND NOT TARGET CompilerLibs::CompilerLibs_static)
    add_library(CompilerLibs::CompilerLibs_static STATIC IMPORTED)
    set_target_properties(CompilerLibs::CompilerLibs_static PROPERTIES
                          IMPORTED_LOCATION ${CompilerLibs_STATIC_LIBRARY}
                          IMPORTED_LINK_INTERFACE_LANGUAGES C)
endif()

if (NOT TARGET CompilerLibs::CompilerLibs_include)
    gcc_print_filename(COMPILERLIBS_INCLUDE include)
    add_library(CompilerLibs::CompilerLibs_include INTERFACE IMPORTED)
    set_target_properties(CompilerLibs::CompilerLibs_include PROPERTIES
                          INTERFACE_INCLUDE_DIRECTORIES ${COMPILERLIBS_INCLUDE}
                         )
endif()
# vim:ts=4:sw=4:expandtab
