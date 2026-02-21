set(HOSTCC gcc)

if (NOT ARCH)
	message(FATAL_ERROR "Not setting variable 'ARCH'")
endif()

if (NOT CROSS_COMPILE)
	message(FATAL_ERROR "Not setting variable 'CROSS_COMPILE'")
endif()

set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
