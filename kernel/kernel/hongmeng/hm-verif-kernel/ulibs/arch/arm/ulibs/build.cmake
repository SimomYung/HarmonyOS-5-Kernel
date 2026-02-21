add_hm_library(syscall.S)
add_hm_library(actv_rpc.S)
add_hm_library(eabi.c)
add_hm_library(sigcontext.c)
add_hm_library(actv_pure_rpc.S)
add_hm_library(actv_emrg_entry.S)
add_hm_library(memzero.c)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/generated/cap_numbers.h
	BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/gen_cap_numbers
	COMMAND ${HOSTCC} ${CMAKE_CURRENT_LIST_DIR}/gen_cap_numbers.c
		-o ${CMAKE_CURRENT_BINARY_DIR}/gen_cap_numbers
		-I${HM_VERIFY_KERNEL}/kernel/include/mapi/uapi
		-D__arm__
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gen_cap_numbers > ${CMAKE_CURRENT_BINARY_DIR}/include/generated/cap_numbers.h
	MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/gen_cap_numbers.c
	IMPLICIT_DEPENDS C ${CMAKE_CURRENT_LIST_DIR}/gen_cap_numbers.c
	COMMENT "GEN cap_numbers.h")

add_custom_target(cap_numbers.h
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/include/generated/cap_numbers.h)
set_property(TARGET cap_numbers.h PROPERTY INCLUDE_DIRECTORIES
	${HM_VERIFY_KERNEL}/kernel/include/mapi/uapi)

append_src_flag(syscall.S -I${CMAKE_CURRENT_BINARY_DIR}/include/generated/)
