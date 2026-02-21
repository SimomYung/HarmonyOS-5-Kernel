# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: CMakeLists.txt for kernel
# Author: Huawei OS Kernel Lab
# Create: Tue Feb 08 16:04:58 2022

cmake_minimum_required(VERSION 3.20)
project(HmKernel LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)
hmbuild_config_target(kernel.base.elf PROPERTY HMBUILD_NO_DEFAULT_LINK  TRUE)
hmbuild_config_target(kernel.base.elf PROPERTY HMBUILD_LINK_WHOLE_BUILTIN  TRUE)
include(LinkKernel.cmake)

function(get_plat_include outvar)
if(DEFINED CONFIG_ARM)
  set(${outvar} "${CMAKE_SOURCE_DIR}/arch/arm/plat/${CONFIG_ARM_PLAT_DIR}/include" PARENT_SCOPE)
elseif(DEFINED CONFIG_AARCH64)
  set(${outvar} "${CMAKE_SOURCE_DIR}/arch/aarch64/plat/${CONFIG_AARCH64_PLAT_DIR}/include" PARENT_SCOPE)
endif()
endfunction(get_plat_include)

function(gcc_print_filename outvar name)
    execute_process(
        COMMAND sh -c "${CMAKE_C_COMPILER} --print-file-name=${name}"
        OUTPUT_VARIABLE stdout_output
        ERROR_VARIABLE  stderr_output
        RESULT_VARIABLE result
    )

    if (NOT (${result} EQUAL 0))
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
        return()
    endif()

    if (NOT (EXISTS ${stdout_output}))
        set(${outvar} "NOTFOUND" PARENT_SCOPE)
    endif()

    string(STRIP "${stdout_output}" ${outvar})
    set(${outvar} "${${outvar}}" PARENT_SCOPE)
endfunction(gcc_print_filename)

function(generate_kernel_release_h)
    # NOTE: If Kernel seperate from repo,need change this COMMAND
    execute_process(
        COMMAND sh -c "${CMAKE_SOURCE_DIR}/scripts/setlocalversion"
        OUTPUT_VARIABLE stdout_output
    )
    string(STRIP "${stdout_output}" KERNEL_VERSION)
    configure_file(${CMAKE_SOURCE_DIR}/kernel.release.h.in ${CMAKE_BINARY_DIR}/include/generated/kernel.release.h)
endfunction(generate_kernel_release_h)


# Kernel Global include directories
set(__KERNEL_BINARY_INCLUDE ${CMAKE_BINARY_DIR}/include)
get_plat_include(__KERNEL_PLAT_INCLUDE_DIR)

set(__KERNEL_ARCH_INCLUDE_DIR
      ${CMAKE_SOURCE_DIR}/arch/${CONFIG_ARCH}/include
      ${CMAKE_SOURCE_DIR}/arch/${CONFIG_ARCH}/include/mapi
      ${CMAKE_SOURCE_DIR}/arch/${CONFIG_ARCH}/include/mapi/uapi
)
set(KERNEL_GLOBAL_INCLUDE
      ${__KERNEL_BINARY_INCLUDE}
      ${__KERNEL_PLAT_INCLUDE_DIR}
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_SOURCE_DIR}/include/mapi
      ${CMAKE_SOURCE_DIR}/include/mapi/uapi
      ${CMAKE_SOURCE_DIR}/../klibs/include
      # Place headers that are included in both kernel/services/uapps in `nostdinc` folder
      ${CMAKE_SYSROOT}/usr/include/nostdinc
      ${__KERNEL_ARCH_INCLUDE_DIR}
)
include_directories(${KERNEL_GLOBAL_INCLUDE})

# Preconfig
string(APPEND CMAKE_ASM_FLAGS " -D__ASSEMBLY__ ")
if(CONFIG_KERNEL_MERGE_BUILD)
    string(APPEND CMAKE_ASM_FLAGS " --hmjam-nojam ")
endif()
if(CONFIG_SMP)
  set(_KERNEL_KLOCKS_DEF_H  ${CMAKE_CURRENT_SOURCE_DIR}/include/mapi/hmkernel/klocks/klocks_def.h)
  set(_KERNEL_KLOCKS_CHECK_TOOL   ${CMAKE_CURRENT_SOURCE_DIR}/../tools/klock_checker/klock_checker.py)
  execute_process(COMMAND python3 ${_KERNEL_KLOCKS_CHECK_TOOL} ${_KERNEL_KLOCKS_DEF_H}
                  COMMAND_ERROR_IS_FATAL LAST)
endif()

gcc_print_filename(__KERNEL_INCLUDE_SYSTEM "include")
string(APPEND CMAKE_C_FLAGS " -isystem ${__KERNEL_INCLUDE_SYSTEM} ")
generate_kernel_release_h()

# Uglycode: kernel not need cmakeng set HMBase::Sysroot
# hack cmakeng behavior by setting HMCMAKENG_HMBUILD_GLOBAL_INITED
set(HMCMAKENG_HMBUILD_GLOBAL_INITED "1")
include(HongmengCMakeNG/HMbuild)

if(HMBUILD_MAKE_BINARIES)
#Link
kernel_link()

install(PROGRAMS "${CMAKE_BINARY_DIR}/kernel.unstripped.elf"
        DESTINATION "/debug/${CONFIG_HMBUILD_INSTALL_DIR_BOOTDIR}"
        COMPONENT kernel_install
)
install(PROGRAMS "${CMAKE_BINARY_DIR}/kernel.elf"
        DESTINATION "${CONFIG_HMBUILD_INSTALL_DIR_BOOTDIR}"
        COMPONENT kernel_install
)

if (DEFINED CONFIG_AARCH64)
	set(ARCH "aarch64")
elseif (DEFINED CONFIG_ARM)
	set(ARCH "arm")
endif()

set(HOSTCC gcc)
set(FIND_INCLUDE_PATH "${CMAKE_SYSROOT}/usr/${CONFIG_HMBUILD_FINDPATH_INCLUDE}")

execute_process(
	COMMAND ${HOSTCC} ${CMAKE_CURRENT_LIST_DIR}/arch/${ARCH}/kernel/gen_pgtblattr_models.c
		-o ${CMAKE_CURRENT_BINARY_DIR}/gen_pgtblattr_models
		-I${CMAKE_SOURCE_DIR}/../arch/${ARCH}/include
		-I${CMAKE_SOURCE_DIR}/../arch/${ARCH}/include/mapi
		-I${CMAKE_SOURCE_DIR}/../arch/${ARCH}/include/mapi/uapi
		-I${CMAKE_SOURCE_DIR}/../kernel/include
		-I${CMAKE_SOURCE_DIR}/../kernel/include/mapi
		-I${CMAKE_SOURCE_DIR}/../kernel/include/mapi/uapi
		-I${CMAKE_SOURCE_DIR}/../include
		-I${CMAKE_SOURCE_DIR}/../klibs/include
		-I${FIND_INCLUDE_PATH}/libvsync
		-include ${CMAKE_BINARY_DIR}/include/generated/autoconf.h
		-D__${ARCH}__ -Wno-builtin-declaration-mismatch
)
execute_process(
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gen_pgtblattr_models
	OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/include/generated/pgtblattr_models.h
)

endif()

if (HMBUILD_MAKE_HEADERS)
	# Remove trampoline layer at hmasm/pgtbl, specifie pgtbl-2levels or pgtbl-lpae
	# Remove trampoline layer at hmasm/pgstr, specifie pgstr-32bits or pgstr-64bits
	# and create symlink
	if (DEFINED CONFIG_ARM)
		install(CODE [=[MESSAGE("Generate pgtbl link for arm page table")]=])
		install(CODE [=[MESSAGE("Generate pgstr link for arm page struct")]=])
		install(CODE [=[MESSAGE("Generate exception link for exception.h")]=])
		if (NOT CONFIG_ARM_LPAE)
			install(CODE [=[MESSAGE("Link hmasm/pgtbl to hmasm/pgtbl-2levels")]=])
			install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgtbl)")
			install(CODE "FILE(CREATE_LINK pgtbl-2levels \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgtbl SYMBOLIC)")

			install(CODE [=[MESSAGE("Link hmasm/pgstr to hmasm/pgstr-32bits")]=])
			install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgstr)")
			install(CODE "FILE(CREATE_LINK pgstr-32bits \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgstr SYMBOLIC)")

			install(CODE [=[MESSAGE("Link hmasm/exception.h to hmasm/exception-2levels.h")]=])
			install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/exception.h)")
			install(CODE "FILE(CREATE_LINK exception-2levels.h \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/exception.h SYMBOLIC)")
		else()
			install(CODE [=[MESSAGE("Link hmasm/pgtbl to hmasm/pgtbl-lpae")]=])
			install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgtbl)")
			install(CODE "FILE(CREATE_LINK pgtbl-lpae \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgtbl SYMBOLIC)")

			install(CODE [=[MESSAGE("Link hmasm/pgstr to hmasm/pgstr-64bits")]=])
			install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgstr)")
			install(CODE "FILE(CREATE_LINK pgstr-64bits \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/pgstr SYMBOLIC)")

			install(CODE [=[MESSAGE("Link hmasm/exception.h to hmasm/exception-lpae.h")]=])
			install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/exception.h)")
			install(CODE "FILE(CREATE_LINK exception-lpae.h \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/exception.h SYMBOLIC)")
		endif()

	endif()
	if (DEFINED CONFIG_AARCH64)
		install(CODE [=[MESSAGE("Create hmasm/current_compat.h from arm/.../hmasm/current_compat.h (Use file(rename) instead of file(copy_file)) ")]=])
		install(CODE "FILE(REMOVE_RECURSE \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current_compat.h)")
		install(CODE "FILE(RENAME \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current.h \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current.h.a64)")
		install(CODE "FILE(COPY ${CMAKE_SOURCE_DIR}/../arch/arm/include/mapi/uapi/hmasm/current.h DESTINATION \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm)")
		install(CODE "FILE(RENAME \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current.h \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current_compat.h)")
		install(CODE "FILE(RENAME \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current.h.a64 \$ENV{DESTDIR}${CONFIG_HMBUILD_INSTALL_DIR_INCLUDEDIR}/hmasm/current.h)")
	endif()

endif()

# vim:ts=4:sw=4:expandtab
