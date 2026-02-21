check_c_compiler_flag("-mno-unaligned-access" HAS_MNO_UNALIGNED_ACCESS)
if(HAS_MNO_UNALIGNED_ACCESS)
	list(APPEND CFLAGS_ARCH_USER -mno-unaligned-access)
endif()

list(APPEND CFLAGS_ARCH_USER -D__HM_ARM_ARCH__=7 -march=armv7-a -Wa,-march=armv7-a)

if (CONFIG_BIG_ENDIAN)
	list(APPEND CFLAGS_ARCH_USER -mbig-endian)
    set(LDFLAGS_ARCH -Wl,-EB,--be8)
else()
	list(APPEND CFLAGS_ARCH_USER -mlittle-endian)
    set(LDFLAGS_ARCH -Wl,-EL)
endif()

check_c_compiler_flag("-marm" HAS_MARM)
if(HAS_MARM)
	list(APPEND CFLAGS_ARCH_USER -marm)
endif()

list(APPEND CFLAGS_ARCH_USER -mcpu=cortex-a9)
