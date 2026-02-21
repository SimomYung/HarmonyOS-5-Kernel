# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: CMakeLists.txt for hm ulibs
# Author: Huawei OS Kernel Lab
# Create: Mon May 23 16:06:31 2022
add_link_options("-L${CMAKE_CURRENT_SOURCE_DIR}/link_script")
if (NOT BUILD_USE_HMBUILD)
cmake_minimum_required(VERSION 3.12.1 FATAL_ERROR)

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
set(HM_VERIFY_KERNEL ${CMAKE_SOURCE_DIR}/..)
include(${HM_VERIFY_KERNEL}/scripts/cmake/common.cmake)

project(hm_ulibs)
enable_language(C)
enable_language(ASM)

#if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
#	message(FATAL_ERROR "Forbid compiling in the source tree")
#endif()

include(HongMengKconfigLib)
parse_kconfig(${KCONFIG_PATH} "")

include(${HM_VERIFY_KERNEL}/scripts/cmake/flags.cmake)

if (NOT DEFINED CONFIG_COMPAT)
    add_definitions(-D__sysif_server_devhost_no_compat32_handlers__)
    add_definitions(-D__sysif_server_hmtracemgr_no_compat32_handlers__)
endif()

if (NOT DEFINED CONFIG_ILP32)
    add_definitions(-D__sysif_server_devhost_no_ilp32_handlers__)
    add_definitions(-D__sysif_server_hmtracemgr_no_ilp32_handlers__)
endif()

if (ARCH STREQUAL "arm")
    add_compile_options(-fno-omit-frame-pointer -mapcs)
endif()

execute_process(
    COMMAND sh -c "readlink -f $(${CMAKE_C_COMPILER} --sysroot=${HMSDKSYSROOTPATH} -print-file-name=libclang_rt.builtins.a)"
    OUTPUT_VARIABLE LLVM_RT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
execute_process(
    COMMAND sh -c "if test -f ${LLVM_RT} && file ${LLVM_RT} | grep -q archive; then echo y; else echo n; fi"
    OUTPUT_VARIABLE USE_LLVM_RT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

if(USE_LLVM_RT STREQUAL "n")
    set(HM_COMPILER_RT "libgcc.a")
else()
    set(HM_COMPILER_RT "libclang_rt.builtins.a")
endif()

string(FIND "${CMAKE_C_FLAGS}" "-mabi=ilp32" ilp32)

# find ilp32 option in CFLAGS
if(NOT ilp32 EQUAL -1)
    set(COMPILER_RT_SUB_PATH "ilp32/")
    set(ABI_CFLAGS "-mabi=ilp32")
endif()

# add include directory to sdk generated sysif apis for ulibs
set(SDK_SYSIF_INCLUDE ${HMSDKSYSIFCLIENT}/pwrmgr
  ${HMSDKSYSIFCLIENT}/freqmgr
  ${HMSDKSYSIFCLIENT}/hguard
  ${HMSDKSYSIFCLIENT}/auditmgr
  ${HMSDKSYSIFCLIENT}/srvmgr
  ${HMSDKSYSIFCLIENT}/devmgr
  ${HMSDKSYSIFCLIENT}/crypt
  ${HMSDKSYSIFCLIENT}/uvmm
  ${HMSDKSYSIFCLIENT}/fs
  ${HMSDKSYSIFCLIENT}/sysmgr
  ${HMSDKSYSIFCLIENT}/keymgr
  ${HMSDKSYSIFCLIENT}/devhost
  ${HMSDKSYSIFSERVER}/devhost
  ${HMSDKSYSIFCLIENT}/hmtracemgr
  ${HMSDKSYSIFSERVER}/hmtracemgr
  ${HMSDKSYSIFBASE}
)

include_directories(BEFORE ${SDK_SYSIF_INCLUDE})

set(LIBRARY_SRCS "" CACHE INTERNAL "")
include(libalgo/build.cmake)
include(libbunch/build.cmake)
include(libcrt/build.cmake)
include(libpanic/build.cmake)
include(libhmlog/build.cmake)
include(libhmsync/build.cmake)
include(libhmirq/build.cmake)
include(libmem/build.cmake)
include(libperf/build.cmake)
include(libprocfs/build.cmake)
include(libsysif/build.cmake)
include(libhmucap/build.cmake)
include(libvsyscall/build.cmake)
include(klibs/build.cmake)
include(libhmsrv_sys/build.cmake)
include(libhmshmalloc/build.cmake)
include(libhmpm/build.cmake)
include(librb/build.cmake)
include(libself_rel/build.cmake)
include(libhmactv/build.cmake)
include(libhmtrace/build.cmake)
include(libhmtrace_ng/build.cmake)
if (CONFIG_HMVSYSCALL_TRACE)
include(libstrace/build.cmake)
endif ()
include(libhmelf/build.cmake)
include(libkbox/build.cmake)
include(libhmuev/build.cmake)
include(libkev/build.cmake)
include(libhguard/build.cmake)
include(libhmsrv_crypt/build.cmake)
include(libhmebr/build.cmake)
include(libpreempt/build.cmake)
include(libvpipe/build.cmake)
include(libhmiov/build.cmake)
include(libhmnotifier/build.cmake)
include(libhmuname/build.cmake)
include(libfdtable/build.cmake)
include(libhmpsf/build.cmake)
include(libhmprobe/build.cmake)
include(libsysevent/build.cmake)
include(libtrack_info/build.cmake)

if (CONFIG_LIBFAULTEVT)
include(libhmfaultevt/build.cmake)
endif ()

if (CONFIG_HYPERVISOR)
include(libvhost_user/build.cmake)
endif ()

include(${HM_VERIFY_KERNEL}/arch/${ARCH}/ulibs/build.cmake)
if (CONFIG_KEYMGR)
include(libhmsrv_key/build.cmake)
endif ()
if (CONFIG_FS_SAFETY_READ)
include(libhmcert/build.cmake)
set(LIBHITLS_INCLUDES
    ${HMSDKINCLUDE}/libhmcrypt/
    ${HMSDKINCLUDE}/crypto/
    ${HMSDKINCLUDE}/bsl/
   )
set(LIBHITLS
    libhmcrypt.a
    libhitls_crypto.a
    libhitls_bsl.a
   )
endif ()
if (CONFIG_UCFI)
include(libucfi/build.cmake)
endif ()
#set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
#add_custom_target(headers_install
#	COMMAND cmake -E copy_directory ${PROJECT_SOURCE_DIR}/include ${HM_SDK_PREFIX}/ulibs)
add_library(hmulibs-static
    STATIC
    ${LIBRARY_SRCS}
    )
set_target_properties(hmulibs-static PROPERTIES OUTPUT_NAME hmulibs)
add_library(hmulibs
    SHARED
    ${LIBRARY_SRCS}
    )
# XXX Remove this when other apps explicitly handle shared and static libraries
set_target_properties(hmulibs PROPERTIES OUTPUT_NAME hmulibs VERSION 0.1 SOVERSION 0)
target_compile_options(hmulibs-static PRIVATE ${CFLAGS_USER})
target_compile_options(hmulibs-static PRIVATE -D_XOPEN_SOURCE=700)
target_compile_options(hmulibs-static PRIVATE ${HM_USER_INCLUDE})

target_include_directories(hmulibs PRIVATE ${HM_VERIFY_KERNEL}/sysmgr/include/aapi
					   ${HM_VERIFY_KERNEL}/kernel/include/mapi/uapi
					   ${HM_VERIFY_KERNEL}/arch/${ARCH}/include/mapi/uapi
					   ${LIBHITLS_INCLUDES}
					   )
target_include_directories(hmulibs-static PRIVATE ${HM_VERIFY_KERNEL}/sysmgr/include/aapi
						  ${HM_VERIFY_KERNEL}/kernel/include/mapi/uapi
						  ${HM_VERIFY_KERNEL}/arch/${ARCH}/include/mapi/uapi
						  ${LIBHITLS_INCLUDES}
						  )

target_compile_options(hmulibs PRIVATE ${CFLAGS_USER})
target_compile_options(hmulibs PRIVATE -D_XOPEN_SOURCE=700)
target_compile_options(hmulibs PRIVATE ${HM_USER_INCLUDE})
# add -funwind-tables for AARCH64
if (CONFIG_AARCH64)
target_compile_options(hmulibs PRIVATE -funwind-tables)
endif()

# add KASAN isolation macro
if (CONFIG_ENABLE_KASAN)
    target_compile_options(hmulibs PRIVATE -DCONFIG_ENABLE_KASAN)
    target_compile_options(hmulibs-static PRIVATE -DCONFIG_ENABLE_KASAN)
endif()

# add DLIST_DEBUG_ENABLE macro if CONFIG_DLIST_DEBUG=y
if (CONFIG_DLIST_DEBUG)
    target_compile_options(hmulibs PRIVATE -DDLIST_DEBUG_ENABLE)
endif()

if (NOT LIBGCC_PATH)
    execute_process(
        COMMAND ${CMAKE_C_COMPILER} --sysroot=${HMSDKSYSROOTPATH}
                --print-file-name ${COMPILER_RT_SUB_PATH}${HM_COMPILER_RT}
        RESULT_VARIABLE ret
        OUTPUT_VARIABLE path
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (ret)
        message(FATAL_ERROR "${CMAKE_C_COMPILER} cannot tell where is libgcc.a")
    endif()
    if (NOT path)
        message(FATAL_ERROR "The libgcc.a path is invalid")
    endif()
    message(STATUS "Detected LIBGCC_PATH: ${path}")
    set(LIBGCC_PATH ${path})
endif()
add_library(libgcc STATIC IMPORTED)
set_target_properties(libgcc PROPERTIES IMPORTED_LOCATION ${LIBGCC_PATH})
target_link_libraries(hmulibs PRIVATE libgcc ${LIBHITLS})

append_link_flags(hmulibs "-nostdlib;${LDFLAGS_ARCH}")
append_link_flags(hmulibs "-Wl,-z,relro,-z,now;${LDFLAGS_ARCH}")
append_link_flags(hmulibs "-Wl,-z,noexecstack;${LDFLAGS_ARCH}")
# The only archive used by libhmulibs is libgcc/libclang
append_link_flags(hmulibs "-Wl,--exclude-libs=ALL")

add_dependencies(hmulibs cap_numbers.h)
add_dependencies(hmulibs-static cap_numbers.h)

execute_process(
    COMMAND ${CMAKE_C_COMPILER} --print-prog-name=strip
    RESULT_VARIABLE ret
    OUTPUT_VARIABLE STRIP_EXECUTABLE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

add_custom_command(TARGET hmulibs
    POST_BUILD
    COMMAND cp -a $<TARGET_FILE:hmulibs> $<TARGET_FILE_DIR:hmulibs>/libhmulibs.unstripped.so
    COMMAND ${STRIP_EXECUTABLE} -D ${CONFIG_STRIP_OPTION} $<TARGET_FILE:hmulibs>
    )

if (NOT HMULIBS_INSTALL_LIB_DIR)
    set(HMULIBS_INSTALL_LIB_DIR usr/lib)
endif()
if (NOT HMULIBS_INSTALL_HEADERS_DIR)
    set(HMULIBS_INSTALL_HEADERS_DIR ${HMULIBS_INSTALL_LIB_DIR}/../include)
endif()
install(TARGETS hmulibs-static
    COMPONENT hmulibs_static_install
    ARCHIVE DESTINATION ${HMULIBS_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${HMULIBS_INSTALL_LIB_DIR}
    )
install(TARGETS hmulibs
    COMPONENT hmulibs_install
    ARCHIVE DESTINATION ${HMULIBS_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${HMULIBS_INSTALL_LIB_DIR}
    )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/hmulibs-static.dir/libcrt/hook.c.o
    COMPONENT hmulibs_static_install
    DESTINATION ${HMULIBS_INSTALL_LIB_DIR}
    RENAME hook.o
    )
install(FILES $<TARGET_FILE_DIR:hmulibs>/libhmulibs.unstripped.so
    COMPONENT hmulibs_install
    DESTINATION ${HMULIBS_INSTALL_LIB_DIR}
    EXCLUDE_FROM_ALL
    )
else()
cmake_minimum_required(VERSION 3.16)
project(hm-ulibs LANGUAGES NONE)
include(HongmengCMakeNG/Kconfig)

set(FIND_INCLUDE_PATH "${CMAKE_SYSROOT}/usr/${CONFIG_HMBUILD_FINDPATH_INCLUDE}")

if (DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
    set(CAP_INCLUDE_PATH "${FIND_INCLUDE_PATH}/generated")
    include_directories(BEFORE ${CAP_INCLUDE_PATH})
endif()

include(HongmengCMakeNG/HMbuild)

set(FIND_LIB_PATH "${CMAKE_SYSROOT}/usr/${CONFIG_HMBUILD_FINDPATH_LIB}")

if (DEFINED CONFIG_HMBUILD_MAKE_BINARIES)
    hmbuild_get_interface_target_property(static_hmulibs hmulibs_static HMBUILD_CMAKETARGET_FINAL)

    add_custom_command(TARGET ${static_hmulibs}
        POST_BUILD
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/temp
        COMMAND cp ${FIND_LIB_PATH}/libmetalibs${CONFIG_HMBUILD_OUTPUT_SUFFIX}.a ${CMAKE_BINARY_DIR}/temp
    )

    add_custom_command(TARGET ${static_hmulibs}
        POST_BUILD
        COMMAND ${CMAKE_AR} x libmetalibs${CONFIG_HMBUILD_OUTPUT_SUFFIX}.a
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/temp"
    )

    add_custom_command(TARGET ${static_hmulibs}
        POST_BUILD
        COMMAND ${CMAKE_AR} rcs ${CMAKE_BINARY_DIR}/libhmulibs${CONFIG_HMBUILD_OUTPUT_SUFFIX}.a ${CMAKE_BINARY_DIR}/temp/*.o
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/temp
    )
endif()

endif()
