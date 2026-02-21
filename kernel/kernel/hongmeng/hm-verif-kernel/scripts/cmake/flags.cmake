include (${HM_VERIFY_KERNEL}/scripts/cmake/hmsdk.cmake)

#############################  Prepare header(start)  ###############################
set(HM_USER_INCLUDE
        -I${HM_VERIFY_KERNEL}/arch/${ARCH}/ulibs/include
        -I${HM_VERIFY_KERNEL}/ulibs/include/
        -I${HM_VERIFY_KERNEL}/klibs/include/
        -I${HM_VERIFY_KERNEL}/arch/${ARCH}/include/mapi/uapi/
        -I${HM_VERIFY_KERNEL}/kernel/include/mapi/uapi
        -I${HM_VERIFY_KERNEL}/modules/include/mapi/uapi
        -I${HMSDKINCLUDE}
)

add_compile_options(
	-include ${HM_VERIFY_KERNEL}/include/hmkernel/kconfig.h
	-I ${CMAKE_BINARY_DIR}/include
)

#############################  flags  ###############################
if (ARCH STREQUAL "aarch64")
	include (${HM_VERIFY_KERNEL}/scripts/cmake/aarch64.cmake)
else()
	include (${HM_VERIFY_KERNEL}/scripts/cmake/arm.cmake)
endif()

set(WARNING_FLAGS -Wall -Wundef -Wstrict-prototypes -fno-strict-aliasing -fno-common
	-Werror-implicit-function-declaration -Wno-format-security)

include(${HM_VERIFY_KERNEL}/scripts/cmake/extra_warning.cmake)
add_compile_options(${WARNING_FLAGS})
add_compile_options(${HM_EXTRA_WARNING_FLAGS})

execute_process(COMMAND ${CMAKE_C_COMPILER} --sysroot=${HMSDKSYSROOTPATH} --print-file-name=include
	        OUTPUT_VARIABLE GCC_INCLUDE_PATH)
# Strip linkbreak
string(STRIP ${GCC_INCLUDE_PATH} GCC_INCLUDE_PATH)

set(GCC_HEADER -nostdinc -isystem ${GCC_INCLUDE_PATH})
add_compile_options(${GCC_HEADER})

list(APPEND CFLAGS_OPT -g)

if (CONFIG_CC_OPTIMIZE_FOR_SIZE)
	check_c_compiler_flag(-Oz HAS_OZ)
	if(HAS_OZ)
		list(APPEND CFLAGS_OPT -Oz)
	else()
		list(APPEND CFLAGS_OPT -Os)
	endif()
elseif (CONFIG_CC_OPTIMIZE_O0)
	list(APPEND CFLAGS_OPT -O0)
elseif (CONFIG_CC_OPTIMIZE_O1)
	list(APPEND CFLAGS_OPT -O1)
elseif (CONFIG_CC_OPTIMIZE_O2)
	list(APPEND CFLAGS_OPT -O2)
elseif (CONFIG_CC_OPTIMIZE_O3)
	list(APPEND CFLAGS_OPT -O3)
endif()

if (CONFIG_CC_STACKPROTECTOR_AUTO)
	check_c_compiler_flag(-fstack-protector-strong HAS_FSTACK_PROTECTOR_STRONG)
	if (HAS_FSTACK_PROTECTOR_STRONG)
		list(APPEND CFLAGS_OPT -fstack-protector-strong)
	else()
		check_c_compiler_flag(-fstack-protector HAS_FSTACK_PROTECTOR)
		if (HAS_FSTACK_PROTECTOR)
			list(APPEND CFLAGS_OPT -fstack-protector)
		endif()
	endif()
elseif(CONFIG_CC_STACKPROTECTOR_REGULAR)
	list(APPEND CFLAGS_OPT -fstack-protector)
elseif(CONFIG_CC_STACKPROTECTOR_STRONG)
	list(APPEND CFLAGS_OPT -fstack-protector-strong)
elseif(CONFIG_HMBUILD_LLVM_RETURN_GUARD)
	list(APPEND CFLAGS_OPT -fstack-protector-ret-strong)
elseif(CONFIG_HMBUILD_LLVM_RETURN_GUARD_ALL)
	list(APPEND CFLAGS_OPT -fstack-protector-ret-all)
else()
	check_c_compiler_flag(-fno-stack-protector HAS_FNO_STACK_PROTECTOR)
	if (HAS_FNO_STACK_PROTECTOR)
		list(APPEND CFLAGS_OPT -fno-stack-protector)
	endif()
endif()

if (CONFIG_UBSAN)
	list(APPEND CFLAGS_OPT -fsanitize=undefined -fsanitize-undefined-trap-on-error -fno-sanitize=alignment)
endif()

set(USERLAND_C_STANDARD ${CONFIG_USERLAND_C_STANDARD})
string(STRIP ${USERLAND_C_STANDARD} USERLAND_C_STANDARD)
set(CFLAGS_USER -Wall -Wundef -Wstrict-prototypes
                -fno-strict-aliasing -fno-common
                -Werror-implicit-function-declaration
                -Wno-format-security
		-std=${USERLAND_C_STANDARD} ${CFLAGS_ARCH_USER}
		${CFLAGS_OPT} -ffreestanding)
