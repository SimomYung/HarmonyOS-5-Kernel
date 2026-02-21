if (CONFIG_BIG_ENDIAN)
	list(APPEND CFLAGS_ARCH_USER -mbig-endian)
    set(LDFLAGS_ARCH -Wl,-EB)
else()
	list(APPEND CFLAGS_ARCH_USER -mlittle-endian)
    set(LDFLAGS_ARCH -Wl,-EL)
endif()

# Modify the CFLAGS to disable sysmgr using floating point, and the
# condition is added because x86_64_Clang for LLT does not support this
# compilation option.
if (NOT CONFIG_COMPILE_X86_64)
	list(APPEND CFLAGS_ARCH_USER -mgeneral-regs-only)
endif()
