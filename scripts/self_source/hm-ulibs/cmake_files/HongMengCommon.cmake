set(HW_CMAKE_VERSION_MIN 3.12.1) # Should equal BEST most of time
set(HW_CMAKE_VERSION_BEST 3.25.3)

cmake_minimum_required(VERSION ${HW_CMAKE_VERSION_MIN} FATAL_ERROR)
if (CMAKE_VERSION VERSION_LESS ${HW_CMAKE_VERSION_BEST})
    message(WARNING "Non-Preference cmake version ${CMAKE_VERSION}."
        "Please install cmake-${HW_CMAKE_VERSION_BEST}")
endif()

# NOTE: remove Wsign-compare because hm-network and definition
# of CMSG_NXTHDR in libc-headers through sign-compare in arm building.
# Default flags
add_compile_options(
    -Wall -Werror -Wextra -Wunused -Wpointer-arith -Wmissing-declarations
    -Wmissing-format-attribute -Wno-sign-compare -Wunused-but-set-variable
    -Wuninitialized -fstack-protector-strong
    )

# XXX Executables always have unstripped and stripped versions
set(COMPILER_OPT_RELEASE "-g;-O2")
set(COMPILER_OPT_DEBUG "-g;-O0")

# XXX Enable shared library globally
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)

# XXX Manually set it here, maybe move to platform definition
set(LIB_PREFIX lib)
set(SO_SUFFIX .so)
set(AR_SUFFIX .a)

#####################################
# Generic control flags
#####################################

# XXX Do not use full RPATH because we are cross-compiling
set(CMAKE_SKIP_BUILD_RPATH TRUE)

option(NO_DEFAULT_CRT "Avoid linking crt when checking linkers" TRUE)
if (NO_DEFAULT_CRT)
    set(CMAKE_REQUIRED_FLAGS "-nostdlib")
endif()

set(FORCE_ARM_SHORT_ENUM FALSE CACHE BOOL "Force short enum")

set(NO_DEFAULT_LIBS FALSE CACHE BOOL "Do not link default hm libraries")

set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -rcD <TARGET> <LINK_FLAGS> <OBJECTS>")

set(BINS_INSTALL_DIR bin
    CACHE STRING "Installation directory: binaries")
set(LIBS_INSTALL_DIR lib
    CACHE STRING "Installation directory: libraries")
set(HEADERS_INSTALL_DIR include
    CACHE STRING "Installation directory: headers")
set(SCRIPTS_INSTALL_DIR scripts
    CACHE STRING "Installation directory: scripts")

#####################################
# shadow stack interface
#####################################
set(SSTACK_UNDEF 0)
set(SSTACK_REG 1)
set(SSTACK_MEM 2)
set(SSTACK_INHERIT 3)

set(SSTACK_STATIC_METHOD ${SSTACK_REG} CACHE STRING "")
set(SSTACK_SHARED_METHOD ${SSTACK_MEM})
set(SSTACK_OBJECT_METHOD ${SSTACK_UNDEF})

function(check_sstack_method method)
    if ($(method) LESS ${SSTACK_UNDEF} OR $(method) GREATER ${SSTACK_INHERIT})
        message(FATAL_ERROR "Error: invalid shadow stack method.")
    endif()
endfunction()

function(hm_set_sstack_options_by_method target sstack_method)
    if (${sstack_method} EQUAL ${SSTACK_REG})
        separate_arguments(SECURITY_SSTACK_REG_CFLAGS_LIST UNIX_COMMAND ${SECURITY_SSTACK_REG_CFLAGS})
        hm_append_compiler_flags(${target} ${SECURITY_SSTACK_REG_CFLAGS_LIST})
    elseif (${sstack_method} EQUAL ${SSTACK_MEM})
        separate_arguments(SECURITY_SSTACK_MEM_CFLAGS_LIST UNIX_COMMAND ${SECURITY_SSTACK_MEM_CFLAGS})
        hm_append_compiler_flags(${target} ${SECURITY_SSTACK_MEM_CFLAGS_LIST})
    else ()
         message(FATAL_ERROR "Error: shadow stack method undefine.")
    endif ()
endfunction()

function(hm_set_sstack_method static_method shared_method obj_method)
    check_sstack_method(static_method)
    check_sstack_method(shared_method)
    check_sstack_method(obj_method)

    if (NOT ${static_method} EQUAL ${SSTACK_INHERIT})
        set(SSTACK_STATIC_METHOD ${static_method} PARENT_SCOPE)
    endif()

    if (NOT ${shared_method} EQUAL ${SSTACK_INHERIT})
        set(SSTACK_SHARED_METHOD ${shared_method} PARENT_SCOPE)
    endif()

    if (NOT ${obj_method} EQUAL ${SSTACK_INHERIT})
        set(SSTACK_OBJECT_METHOD ${obj_method} PARENT_SCOPE)
    endif()
endfunction()

######################################
# Post check and set compiler flags
#####################################
include(CheckCCompilerFlag)

function(target_names target target_unstripped target_stripped)
    get_filename_component(name ${target} NAME_WE)
    get_filename_component(ext ${target} EXT)
    set(${target_unstripped} "${name}.unstripped${ext}" PARENT_SCOPE)
    set(${target_stripped}   "${name}${ext}"            PARENT_SCOPE)
endfunction()

macro(check_append_cflag flag var)
    check_c_compiler_flag(${flag} ${var})
    if (${var})
        add_compile_options(${flag})
    endif()
endmacro()

macro(config_libgcc)
    if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        return()
    endif()

    find_c_compiler_component(libgcc LIBGCC)
    if (LIBGCC)
        get_filename_component(_LIBGCC ${LIBGCC} DIRECTORY)
        find_library(libgcc NAMES gcc PATHS ${_LIBGCC})
    endif()
    find_c_compiler_component(libgcc_s LIBGCC_S)
    if (LIBGCC_S)
        get_filename_component(_LIBGCC_S ${LIBGCC_S} DIRECTORY)
        find_library(libgcc_s NAMES gcc_s PATHS ${_LIBGCC_S})
    endif()
endmacro()

macro(config_libclang)
    if (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        return()
    endif()

    find_c_compiler_component(libclang_rt.builtins LIBCLANG)
    if (LIBCLANG)
        get_filename_component(_LIBCLANG ${LIBCLANG} DIRECTORY)
        find_library(libclang NAMES clang_rt.builtins PATHS ${_LIBCLANG})
    endif()
    find_c_compiler_component(libclang_rt.builtins_s LIBCLANG_S)
    if (LIBCLANG_S)
        get_filename_component(_LIBCLANG_S ${LIBCLANG_S} DIRECTORY)
        find_library(libclang_s NAMES clang_rt.builtins_s PATHS ${_LIBCLANG_S})
    endif()
endmacro()

macro(config_objcopy)
    find_c_compiler_component(objcopy objcopy_path)
    get_filename_component(objcopy_abs_path ${objcopy_path} ABSOLUTE)
    set(OBJCOPY_EXECUTABLE ${objcopy_abs_path})
endmacro()

function(find_c_compiler_component comp_name comp_path)
    if (TUNE_CCARGS)
        list(FILTER TUNE_CCARGS INCLUDE REGEX "-march.*")
    endif()
    if (CMAKE_SYSROOT)
        set(search_sysroot "--sysroot=${CMAKE_SYSROOT}")
    endif()
    if (comp_name STREQUAL "libgcc" OR comp_name STREQUAL "libclang_rt.builtins")
        set(cmd_opt "-print-file-name=${comp_name}.a")
    elseif (comp_name STREQUAL "libgcc_s" OR comp_name STREQUAL "libclang_rt.builtins_s")
        set(cmd_opt "-print-file-name=${comp_name}.so")
    else()
        set(cmd_opt "--print-prog-name=${comp_name}")
    endif()
    execute_process(
        COMMAND sh -c "readlink -e $(${CMAKE_C_COMPILER} ${TUNE_CCARGS} ${search_sysroot} ${cmd_opt})"
        RESULT_VARIABLE ret
        OUTPUT_VARIABLE path
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    warn_on(ret "${CMAKE_C_COMPILER} cannot tell where is ${comp_name}")
    warn_on_not(path "The ${comp_name} path is invalid")
    set(${comp_path} ${path} PARENT_SCOPE)
endfunction()

macro(post_check_toolchain)
    config_libgcc()
    config_libclang()
    config_objcopy()

    if (LIBGCC AND LIBGCC_S)
        message(STATUS "Detected compiler runtime:")
        message(STATUS "  ${libgcc}")
        message(STATUS "  ${libgcc_s}")
        set(HM_COMPILER_RT ${libgcc})
        set(HM_COMPILER_DY_RT -lgcc_s)
    elseif (LIBCLANG AND LIBCLANG_S)
        message(STATUS "Detected compiler runtime:")
        message(STATUS "  ${libclang}")
        message(STATUS "  ${libclang_s}")
        set(HM_COMPILER_RT ${libclang})
        set(HM_COMPILER_DY_RT -lclang_rt.builtins_s)
    else()
        message(WARNING "No compiler runtime (libgcc/libclang) found")
    endif()

    # compiler flags only for C
    add_compile_options(-Wmissing-prototypes -Wold-style-definition)

    if (SUBARCH STREQUAL "be")
        check_append_cflag("-mbig-endian" HAS_BIG_ENDIAN)
    else()
        check_append_cflag("-mlittle-endian" HAS_LITTLE_ENDIAN)
    endif()
endmacro()

##############################
# Utility functions an macros
##############################
function(_hm_target_link_options target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        get_target_property(old_link_flags ${target} LINK_FLAGS)
        if (old_link_flags)
            set_target_properties(${target} PROPERTIES
                LINK_FLAGS "${old_link_flags} ${flags}")
        else()
            set_target_properties(${target} PROPERTIES
                LINK_FLAGS "${flags}")
        endif()
    endif()
endfunction()

function(hm_set_lds target lds)
    _hm_target_link_options(${target} -Wl,-T,${lds})
    set_target_properties(${target} PROPERTIES LINK_DEPENDS ${lds})
endfunction()

function(hmuapps_set_lds target lds)
    hm_set_lds(${target} ${lds})
endfunction()

macro(fail_on_not condition comment)
    if (NOT ${condition})
        message(FATAL_ERROR ${comment})
    endif()
endmacro()

macro(fail_on condition comment)
    if (${condition})
        message(FATAL_ERROR ${comment})
    endif()
endmacro()

macro(warn_on_not condition comment)
    if (NOT ${condition})
        message(WARNING ${comment})
    endif()
endmacro()

macro(warn_on condition comment)
    if (${condition})
        message(WARNING ${comment})
    endif()
endmacro()

function(hm_append_compiler_flags target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            target_compile_options(${target} PRIVATE ${f})
        endforeach()
    endif()
endfunction()

function(hmuapps_append_compiler_flags target)
    hm_append_compiler_flags(${target} ${ARGN})
endfunction()

function(hm_append_definitions target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            target_compile_definitions(${target} PRIVATE ${f})
        endforeach()
    endif()
endfunction()

function(hmuapps_append_definitions target)
    hm_append_definitions(${target} ${ARGN})
endfunction()

# XXX SO: 12521452
function(hm_append_definitions_auto target prefix)
    get_cmake_property(_vars VARIABLES)
    string(REGEX MATCHALL "(^|;)${prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}")
    list(SORT _vars)
    foreach (_v ${_matchedVars})
        get_property(_type CACHE "${_v}" PROPERTY TYPE)
        if (NOT "${_type}" STREQUAL "STATIC")
            if ("${_type}" STREQUAL "STRING")
                hm_append_definitions(${target} "${_v}=${${_v}}")
            elseif ("${_type}" STREQUAL "BOOL" AND ${_v})
                hm_append_definitions(${target} ${_v})
            endif()
        endif()
    endforeach()
endfunction()

function(hm_include_directories target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            target_include_directories(${target} PRIVATE ${f})
        endforeach()
    endif()
endfunction()

function(hmuapps_include_directories target)
    hm_include_directories(${target} ${ARGN})
endfunction()

function(hm_include_dirs target)
    cmake_parse_arguments(
        ARG
        "PRIVATE;INTERNAL;PUBLIC"
        ""
        "DIRS"
        ${ARGN}
        )
    list(LENGTH ARG_DIRS num_flags)
    if (NOT num_flags GREATER 0)
        return()
    endif()

    if (ARG_PRIVATE)
        foreach (f ${ARG_DIRS})
            target_include_directories(${target} PRIVATE ${f})
        endforeach()
    elseif (ARG_INTERNAL)
        foreach (f ${ARG_DIRS})
            target_include_directories(${target} PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${f}>)
        endforeach()
    elseif (ARG_PUBLIC)
    endif()
endfunction()

# XXX Remove this after usages are pruned
function(hm_append_linker_flags target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            string(REPLACE " " "," wlf ${f})
            _hm_target_link_options(${target} -Wl,${wlf})
        endforeach()
    endif()
endfunction()

function(hm_target_linker_flags target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            _hm_target_link_options(${target} ${f})
        endforeach()
    endif()
endfunction()

function(hmuapps_append_linker_flags target)
    hm_append_linker_flags(${target} ${ARGN})
endfunction()

function(hm_append_link_libraries target)
    set(flags ${ARGN})
    list(LENGTH flags num_flags)
    if (num_flags GREATER 0)
        foreach (f ${flags})
            target_link_libraries(${target} PRIVATE ${f})
        endforeach()
    endif()
endfunction()

function(hmuapps_append_link_libraries target)
    hm_append_link_libraries(${target} ${ARGN})
endfunction()

function(hm_import_product target)
    cmake_parse_arguments(
        PRODUCT
        "SHARED;STATIC;OBJECT;INTERFACE"
        "LOCATION"
        "TYPE"
        ${ARGN}
        )
    if (TARGET ${target})
        # XXX Silently ignore duplicated imports
        return()
    endif()
    if (PRODUCT_TYPE STREQUAL "library")
        if (PRODUCT_SHARED)
            add_library(${target} SHARED IMPORTED)
            set_target_properties(${target} PROPERTIES IMPORTED_LOCATION ${PRODUCT_LOCATION})
        elseif (PRODUCT_STATIC)
            add_library(${target} STATIC IMPORTED)
            set_target_properties(${target} PROPERTIES IMPORTED_LOCATION ${PRODUCT_LOCATION})
        elseif (PRODUCT_OBJECT)
            add_library(${target} OBJECT IMPORTED)
            set_target_properties(${target} PROPERTIES IMPORTED_OBJECTS ${PRODUCT_LOCATION})
        elseif (PRODUCT_INTERFACE)
            add_library(${target} INTERFACE IMPORTED)
            set_target_properties(${target} PROPERTIES IMPORTED_LIBNAME ${PRODUCT_LOCATION})
        endif()
    elseif(PRODUCT_TYPE STREQUAL "executable")
        add_executable(${target} IMPORTED)
        set_target_properties(${target} PROPERTIES IMPORTED_LOCATION ${PRODUCT_LOCATION})
    endif()
endfunction()

macro(hm_rootfs_uapps_add)
    foreach (app ${ARGN})
        set(hm_rootfs_uapps "${hm_rootfs_uapps};${app}" PARENT_SCOPE)
        set(hm_rootfs_uapps "${hm_rootfs_uapps};${app}")
    endforeach()
endmacro()

macro(hm_builtin_uapps_add)
    foreach (app ${ARGN})
        set(hm_builtin_uapps "${hm_builtin_uapps};${app}" PARENT_SCOPE)
        set(hm_builtin_uapps "${hm_builtin_uapps};${app}")
    endforeach()
endmacro()

function(hm_default_include target)
    target_include_directories(
        ${target}
        # PRIVATE ${HM_VERIFY_KERNEL}/kernel/include/uapi
        # PRIVATE ${HM_VERIFY_KERNEL}/arch/${ARCH}/include/uapi
        # PRIVATE ${HM_VERIFY_KERNEL}/ulibs/include
        # PRIVATE ${HM_VERIFY_KERNEL}/ulibs/include/arch/${ARCH}
        PRIVATE ${HMSDKINCLUDE}
        # PRIVATE ${KERNEL_BINARY_DIR}/include
        )
endfunction()

set(HMSDKBASELIB "${HMSDKSYSROOTPATH}/lib" CACHE STRING "Dependent directory : lib")

function(uapps_default_link target)
    cmake_parse_arguments(
        HMUAPPS
        "DYNAMIC"
        ""
        "LINKGROUP;OBJECTS;WHOLEARCHIVE;LIBRARIES"
        ${ARGN}
        )
    hm_import_product(libhwsecurec STATIC TYPE library LOCATION ${HMSDKLIB}/libhwsecurec.a)

    if (NOT NO_DEFAULT_LIBS)
        if (HMUAPPS_DYNAMIC)
            hm_import_product(libc_shared INTERFACE TYPE library LOCATION c)
            target_link_libraries(${target} PRIVATE libc_shared)

            hm_import_product(hmulibs INTERFACE TYPE library LOCATION hmulibs)
            hm_import_product(hmsrv_fs INTERFACE TYPE library LOCATION hmsrv_fs)
            hm_import_product(hmsrv_net INTERFACE TYPE library LOCATION hmsrv_net)
            hm_import_product(hwsecurec INTERFACE TYPE library LOCATION hwsecurec)
            target_link_libraries(${target} PRIVATE hmulibs hmsrv_fs hmsrv_net hwsecurec)
        else()
            hm_import_product(libulibs STATIC TYPE library LOCATION ${HMSDKLIB}/libhmulibs.a)
            hm_import_product(libhmc STATIC TYPE library LOCATION ${HMSDKBASELIB}/libhmc.a)
            list(APPEND HMUAPPS_LINKGROUP "libulibs;libhwsecurec;libhmc;${HM_COMPILER_RT}")
        endif()
    endif()

    target_link_libraries(${target}
        PRIVATE ${HMUAPPS_OBJECTS}
        PRIVATE -Wl,--whole-archive ${HMUAPPS_WHOLEARCHIVE} -Wl,--no-whole-archive
        PRIVATE ${HMUAPPS_LIBRARIES}
        PRIVATE -Wl,--start-group
                ${HMUAPPS_LINKGROUP}
                -Wl,--end-group
        )
    foreach (objs ${HMUAPPS_OBJECTS})
        target_link_libraries(${objs} PRIVATE ${HMUAPPS_LIBRARIES})
    endforeach()
endfunction()

# Libraries only link when shared
function(hm_library_default_link target)
    cmake_parse_arguments(
        HMLIB
        ""
        ""
        "OBJECTS;WHOLEARCHIVE;LIBRARIES"
        ${ARGN}
        )

    target_link_libraries(${target}
        PRIVATE ${HMLIB_OBJECTS}
        PRIVATE -Wl,--whole-archive ${HMLIB_WHOLEARCHIVE} -Wl,--no-whole-archive
        PRIVATE ${HMLIB_LIBRARIES}
        )
    foreach (objs ${HMLIB_OBJECTS})
        target_link_libraries(${objs} PRIVATE ${HMLIB_LIBRARIES})
    endforeach()
endfunction()

execute_process(
    COMMAND sh -c "readlink -f $(${CMAKE_C_COMPILER} --sysroot=${HMSDKSYSROOTPATH} -print-file-name=libclang_rt.builtins_s.so)"
    OUTPUT_VARIABLE LLVM_RT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
execute_process(
    COMMAND sh -c "if test -f ${LLVM_RT} && file ${LLVM_RT} | grep -q ELF; then echo y; else echo n; fi"
    OUTPUT_VARIABLE USE_LLVM_RT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

#################################################
# Interfaces to create executables and libraries
#################################################
function(hm_add_executable target)
    cmake_parse_arguments(
        HMUAPPS
        "DYNAMIC;DEBUG;NO_INSTALL;DISABLE_KASAN;DISABLE_XOM;DISABLE_SSTACK;SSTACK_REG;SSTACK_MEM"
        "RPATH;"
        "SOURCES;COMPILER_FLAGS;PRIVATE_INCLUDES;LINKGROUP;OBJECTS;WHOLEARCHIVE;LIBRARY_PATHS;LIBRARIES;LINKER_SCRIPT"
        ${ARGN}
        )

    add_executable(${target} ${HMUAPPS_SOURCES})
    hm_target_linker_flags(${target} "-Wl,-z,noexecstack")
    # Target-speicific compiler flags (CFLAGS).
    if (HMUAPPS_DEBUG)
        hm_append_compiler_flags(${target} ${COMPILER_OPT_DEBUG})
    else()
        if (HMUAPPS_DYNAMIC)
            # Kasan cflags
            if (CONFIG_KASAN_RUNTIME)
                if (CONFIG_ENABLE_KASAN)
                    if (NOT HMUAPPS_DISABLE_KASAN)
                        separate_arguments(KASAN_CFLAGS_LIST UNIX_COMMAND ${KASAN_CMAKE_CFLAGS})
                        target_compile_options(${target} PRIVATE ${KASAN_CFLAGS_LIST})
                    endif()
                endif()
            endif()

        endif()
        hm_append_compiler_flags(${target} ${COMPILER_OPT_RELEASE})
    endif()
    hm_append_compiler_flags(${target} ${HMUAPPS_COMPILER_FLAGS})

    # set ucfi cflags compilation options
    if (CONFIG_UCFI)
        separate_arguments(UCFI_CFLAGS_LIST UNIX_COMMAND ${UCFI_CMAKE_CFLAGS})
        hm_append_compiler_flags(${target} ${UCFI_CFLAGS_LIST})
    endif()

    # set shadow stack compilation options
    if (CONFIG_SHADOW_STACK)
        if (HMUAPPS_SSTACK_REG)
            separate_arguments(SECURITY_SSTACK_REG_CFLAGS_LIST UNIX_COMMAND ${SECURITY_SSTACK_REG_CFLAGS})
            hm_append_compiler_flags(${target} ${SECURITY_SSTACK_REG_CFLAGS_LIST})
        elseif (HMUAPPS_SSTACK_MEM)
            separate_arguments(SECURITY_SSTACK_MEM_CFLAGS_LIST UNIX_COMMAND ${SECURITY_SSTACK_MEM_CFLAGS})
            hm_append_compiler_flags(${target} ${SECURITY_SSTACK_MEM_CFLAGS_LIST})
        elseif (NOT HMUAPPS_DISABLE_SSTACK)
            if (HMUAPPS_DYNAMIC)
                hm_set_sstack_options_by_method(${target} ${SSTACK_SHARED_METHOD})
            else ()
                hm_set_sstack_options_by_method(${target} ${SSTACK_STATIC_METHOD})
            endif ()
        endif()
    endif()

    hm_append_definitions_auto(${target} "CONFIG")
    hm_default_include(${target})

    # For dynamically linked elfs
    if (HMUAPPS_DYNAMIC)
        set(uapps_link_arg "DYNAMIC")
        hm_target_linker_flags(${target}
            "-Wl,--dynamic-linker,/lib/hmld.so.elf,--export-dynamic;")
        # kasan ldflags
        if (CONFIG_KASAN_RUNTIME)
            if (CONFIG_ENABLE_KASAN)
                if (NOT HMUAPPS_DISABLE_KASAN)
                    hm_target_linker_flags(${target} -nodefaultlibs)
                    target_link_libraries(${target} PRIVATE ${KASAN_CMAKE_LIBRARYS})
                    if(USE_LLVM_RT STREQUAL "y")
                        target_link_libraries(${target} PRIVATE -lclang_rt.builtins_s -lunwind_s)
                    endif()
                endif()
            endif()
        endif()

        # ucfi ldflags
        if (CONFIG_UCFI)
            hm_target_linker_flags(${target} -nodefaultlibs)
            target_link_libraries(${target} PRIVATE ${UCFI_CMAKE_LIBRARYS})
            if(USE_LLVM_RT STREQUAL "y")
                target_link_libraries(${target} PRIVATE -lclang_rt.builtins_s -lunwind_s)
            endif()
        endif()
    endif()

    if (HMUAPPS_PRIVATE_INCLUDES)
        hm_include_directories(${target} ${HMUAPPS_PRIVATE_INCLUDES})
    endif()
    # XXX Endian flags should be introduced from toolchains
    # or driving build systems (kbuild/yocto)
    if (HMUAPPS_LIBRARY_PATHS)
        foreach (p ${HMUAPPS_LIBRARY_PATHS})
            hm_target_linker_flags(${target} -L${p})
        endforeach()
    endif()
    if (HMUAPPS_LINKER_SCRIPT)
        hm_set_lds(${target} ${HMUAPPS_LINKER_SCRIPT})
    endif()
    uapps_default_link(${target}
        ${uapps_link_arg}
        OBJECTS ${HMUAPPS_OBJECTS}
        WHOLEARCHIVE ${HMUAPPS_WHOLEARCHIVE}
        LINKGROUP ${HMUAPPS_LINKGROUP}
        LIBRARIES ${HMUAPPS_LIBRARIES}
        )

    target_names(${target} hmuapp_unstripped hmuapp_stripped)
    add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}> $<TARGET_FILE_DIR:${target}>/${hmuapp_unstripped}
        COMMAND ${CMAKE_STRIP} -D ${CONFIG_STRIP_OPTION} $<TARGET_FILE:${target}>
        )
    if (NOT HMUAPPS_NO_INSTALL)
        install(TARGETS ${target}
            COMPONENT bins_install
            RUNTIME DESTINATION ${BINS_INSTALL_DIR}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE
            )
        install(FILES $<TARGET_FILE_DIR:${target}>/${hmuapp_unstripped}
            COMPONENT bins_install
            DESTINATION ${BINS_INSTALL_DIR}
            PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ GROUP_EXECUTE
            WORLD_READ WORLD_EXECUTE
            )
    endif()
    # NOTE : xom for test
    if (NOT HMUAPPS_DISABLE_XOM)
	    if ((CONFIG_XOM STREQUAL "y") AND (NOT HMUAPPS_LINKER_SCRIPT))
		    if (HMUAPPS_DYNAMIC)
			    xom_binary(${target} dynamic executable linker)
		    else()
			    xom_binary(${target} static executable nolinker)
		    endif()
	    endif()
    endif()
endfunction()

function(hm_add_library target)
    cmake_parse_arguments(
        HMLIB
        "STATIC;SHARED;OBJECT;DEBUG;NO_INSTALL;DISABLE_XOM;DISABLE_SSTACK;SSTACK_REG;SSTACK_MEM"
        "OUTPUT_NAME;LIBS_INSTALL_PATH;HEADERS_INSTALL_PATH;"
        "SOURCES;COMPILER_FLAGS;PRIVATE_INCLUDES;PUBLIC_INCLUDES;INTERNAL_INCLUDES;LIBRARIES;LIBRARY_PATHS;OBJECTS;WHOLEARCHIVE;LINKER_SCRIPT;RPATH;"
        ${ARGN}
        )

    if (HMLIB_STATIC)
        add_library(${target} STATIC ${HMLIB_SOURCES})
    elseif (HMLIB_SHARED)
        add_library(${target} SHARED ${HMLIB_SOURCES})
        hm_append_compiler_flags(${target} -fPIC)
    elseif (HMLIB_OBJECT)
        add_library(${target} OBJECT ${HMLIB_SOURCES})
    endif()

    # set ucfi cflags compilation options
    if (CONFIG_UCFI)
        separate_arguments(UCFI_CFLAGS_LIST UNIX_COMMAND ${UCFI_CMAKE_CFLAGS_TAGONLY})
        hm_append_compiler_flags(${target} ${UCFI_CFLAGS_LIST})
    endif()

    # set shadow stack compilation options
    if (CONFIG_SHADOW_STACK)
        if (HMLIB_SSTACK_REG)
            separate_arguments(SECURITY_SSTACK_REG_CFLAGS_LIST UNIX_COMMAND ${SECURITY_SSTACK_REG_CFLAGS})
            hm_append_compiler_flags(${target} ${SECURITY_SSTACK_REG_CFLAGS_LIST})
        elseif (HMLIB_SSTACK_MEM)
            separate_arguments(SECURITY_SSTACK_MEM_CFLAGS_LIST UNIX_COMMAND ${SECURITY_SSTACK_MEM_CFLAGS})
            hm_append_compiler_flags(${target} ${SECURITY_SSTACK_MEM_CFLAGS_LIST})
        elseif (NOT HMLIB_DISABLE_SSTACK)
            if (HMLIB_SHARED)
                hm_set_sstack_options_by_method(${target} ${SSTACK_SHARED_METHOD})
            elseif (HMLIB_STATIC)
                hm_set_sstack_options_by_method(${target} ${SSTACK_STATIC_METHOD})
            elseif (HMLIB_OBJECT)
                hm_set_sstack_options_by_method(${target} ${SSTACK_OBJECT_METHOD})
            endif()
        endif()
    endif()

    if (HMLIB_SHARED)
        get_target_property(output_name ${target} OUTPUT_NAME)
        if ("${output_name}" STREQUAL "output_name-NOTFOUND")
            if (HMLIB_OUTPUT_NAME)
                set(output_name ${HMLIB_OUTPUT_NAME})
            else()
                set(output_name ${target})
            endif()
        endif()
        set_target_properties(${target} PROPERTIES
            OUTPUT_NAME ${output_name}
            PREFIX ${LIB_PREFIX}
            SUFFIX ${SO_SUFFIX}
            )
        set(unstripped_library "${LIB_PREFIX}${target}.unstripped${SO_SUFFIX}")
    endif()

    # Target-speicific compiler flags (CFLAGS).
    if (HMLIB_DEBUG)
        hm_append_compiler_flags(${target} ${COMPILER_OPT_DEBUG})
    else()
        hm_append_compiler_flags(${target} ${COMPILER_OPT_RELEASE})
    endif()
    hm_append_compiler_flags(${target} ${HMLIB_COMPILER_FLAGS})

    hm_append_definitions_auto(${target} "CONFIG")

    # Simply include private headers.
    # Those headers should not be visible to other targets.
    hm_include_directories(${target} ${HMLIB_PRIVATE_INCLUDES})

    # Mark the internal headers as INTERFACE.
    # Other targets in the same build can automatically include this directory.
    # Note that public headers are also internal headers.
    if (HMLIB_INTERNAL_INCLUDES)
        hm_include_dirs(${target} INTERNAL DIRS ${HMLIB_INTERNAL_INCLUDES})
    endif()

    # Settings for shared libraries
    if (HMLIB_SHARED)
        if (HMLIB_LIBRARY_PATHS)
            foreach (p ${HMLIB_LIBRARY_PATHS})
                hm_target_linker_flags(${target} -L${p})
            endforeach()
        endif()
        hm_target_linker_flags(${target} "-Wl,-z,relro,-z,now")
        hm_target_linker_flags(${target} "-Wl,-z,noexecstack")
        hm_target_linker_flags(${target} "-Wl,--hash-style=gnu;")

        if (HMLIB_LINKER_SCRIPT)
            hm_set_lds(${target} ${HMLIB_LINKER_SCRIPT})
        endif()

        hm_library_default_link(${target}
            OBJECTS ${HMLIB_OBJECTS}
            WHOLEARCHIVE ${HMLIB_WHOLEARCHIVE}
            LIBRARIES ${HMLIB_LIBRARIES}
            )
    endif()

    # If not built from Yocto, install the SDK first.
    if (NOT USE_STAGING)
        # add_dependencies(${target} hmsdk)
    endif()

    # Possibly override the default installation paths.
    if (NOT HMLIB_LIBS_INSTALL_PATH)
        set(HMLIB_LIBS_INSTALL_PATH ${LIBS_INSTALL_DIR})
    endif()
    if (NOT HMLIB_HEADERS_INSTALL_PATH)
        set(HMLIB_HEADERS_INSTALL_PATH ${HEADERS_INSTALL_DIR})
    endif()

    if (HMLIB_SHARED)
        add_custom_command(TARGET ${target}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}> $<TARGET_FILE_DIR:${target}>/${unstripped_library}
	    COMMAND ${CMAKE_STRIP} -D ${CONFIG_STRIP_OPTION} $<TARGET_FILE:${target}>
            )
    endif()

    # Install the output of the library (.a or .so or .o).
    if (NOT HMLIB_NO_INSTALL)
        install(TARGETS ${target}
            COMPONENT libs_install
            EXPORT export_${target}
            ARCHIVE DESTINATION ${HMLIB_LIBS_INSTALL_PATH}
            LIBRARY DESTINATION ${HMLIB_LIBS_INSTALL_PATH}
            # PUBLIC_HEADER DESTINATION ${HMLIB_HEADERS_INSTALL_PATH}
        )
        if (HMLIB_SHARED)
            install(FILES $<TARGET_FILE_DIR:${target}>/${unstripped_library}
                COMPONENT libs_install
                DESTINATION ${HMLIB_LIBS_INSTALL_PATH}
                PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                GROUP_READ GROUP_EXECUTE
                WORLD_READ WORLD_EXECUTE
                EXCLUDE_FROM_ALL
            )
        endif()

        # Possibly export the library CMake configurations to external projects.
        # Not trigged by default.
        install(EXPORT export_${target}
          NAMESPACE ${CMAKE_PROJECT_NAME}_
          DESTINATION ${CMAKE_PROJECT_NAME}/cmake/${target}
          EXCLUDE_FROM_ALL
        )

        # Export the configuration of this library to other targets in the same
        # build system. To import the project in other CMake projects, use
        # the previous install command to really export it.
        # Reference: https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#build-specification-with-generator-expressions
        export(EXPORT export_${target} NAMESPACE ${CMAKE_PROJECT_NAME}_)

        # A special component for install public headers ONLY!
        # It is excluded from the default ``make install''.
        foreach (inc ${HMLIB_PUBLIC_INCLUDES})
          get_filename_component(abs_inc ${inc} ABSOLUTE)
          if (IS_DIRECTORY "${abs_inc}")
            install(DIRECTORY ${inc}
              DESTINATION ${HMLIB_HEADERS_INSTALL_PATH}
              COMPONENT headers_install
              EXCLUDE_FROM_ALL
            )
            install(DIRECTORY ${inc}
              DESTINATION ${HMLIB_HEADERS_INSTALL_PATH}
              COMPONENT ${target}_headers_install
              EXCLUDE_FROM_ALL
            )
          else()
            install(FILES ${inc}
              DESTINATION ${HMLIB_HEADERS_INSTALL_PATH}
              COMPONENT headers_install
              EXCLUDE_FROM_ALL
              PERMISSIONS OWNER_READ OWNER_WRITE
              GROUP_READ
              WORLD_READ
            )
            install(FILES ${inc}
              DESTINATION ${HMLIB_HEADERS_INSTALL_PATH}
              COMPONENT ${target}_headers_install
              EXCLUDE_FROM_ALL
              PERMISSIONS OWNER_READ OWNER_WRITE
              GROUP_READ
              WORLD_READ
            )
          endif()
        endforeach()
    endif()
    # NOTE: xom for test
    # add at the end of the function to avoid missing link flags
    if (HMLIB_SHARED)
        if ((NOT HMLIB_DISABLE_XOM) AND (NOT HMLIB_LINKER_SCRIPT))
            if (CONFIG_XOM STREQUAL "y")
                xom_binary(${target} dynamic shared nolinker)
            endif()
        endif()
    endif()
endfunction()

function(hm_install_scripts scripts)
    install(FILES ${${scripts}}
        COMPONENT scripts_install
        DESTINATION ${SCRIPTS_INSTALL_DIR}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE
    )
endfunction()

post_check_toolchain()

# xom
# This function needs to be enabled in the new cmakeng framework.
if (CONFIG_XOM STREQUAL "y")
    set(XOM_ELF ${NATIVE_BINDIR}/xom.elf)
endif()
set(TRANS_LDS ${NATIVE_BINDIR}/trans_lds.py)
function(xom_binary target mode shared)
    if (ARCH STREQUAL "aarch64")
        hm_target_linker_flags(${target} "-Wl,-z,separate-code")
        add_custom_command(TARGET ${target}
            POST_BUILD
            COMMAND ${XOM_ELF} $<TARGET_FILE:${target}>
        )
    else()
        # get compile and link flags
        get_target_property(old_link_flags ${target} LINK_FLAGS)
        get_target_property(old_compile_flags ${target} COMPILE_OPTIONS)
        # generate target link script
        if (${shared} STREQUAL "shared")
            execute_process(COMMAND bash -c "${CMAKE_C_COMPILER} -Wl,--verbose -nostdlib  ${CMAKE_C_LINK_FLAGS} ${old_link_flags} ${CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS} > ${CMAKE_CURRENT_BINARY_DIR}/${target}_xom_tmp.lds")
        else()
            execute_process(COMMAND bash -c "${CMAKE_C_COMPILER} -Wl,--verbose -nostdlib  ${CMAKE_C_LINK_FLAGS} ${old_link_flags} > ${CMAKE_CURRENT_BINARY_DIR}/${target}_xom_tmp.lds")
        endif()
        message(STATUS "HongMengCommon [xom_binary]: ${TRANS_LDS} ${mode} ${ARGN} -xom")
        execute_process(COMMAND ${TRANS_LDS} ${mode} ${ARGN} -xom -i ${CMAKE_CURRENT_BINARY_DIR}/${target}_xom_tmp.lds -o ${CMAKE_CURRENT_BINARY_DIR}/${target}_xom.lds)
        hm_target_linker_flags(${target} "-T ${CMAKE_CURRENT_BINARY_DIR}/${target}_xom.lds")
        hm_append_compiler_flags(${target} ${SECURITY_XOM_CFLAGS})
    endif()
endfunction()
