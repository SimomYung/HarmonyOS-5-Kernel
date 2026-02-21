/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: libhmcrypt header file for PAC
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 21 09:32:20 2024
 */
#ifndef ULIBS_INCLUDE_LIBCRYPT_PAC_H
#define ULIBS_INCLUDE_LIBCRYPT_PAC_H

#define PAC_FECFI_XPAC_SIGN_ZERO(func)	\
	asm volatile("xpaci %0\n"	\
		     "mov x0, xzr\n"	\
		     "pacia %0, x0\n"	\
		     : "+r" (func)	\
		     : 			\
		     : "x0")

#ifdef PAC_FWCFI_SYSSERVICE
#define __PAC_INIT_SECTION __attribute__((noinline,section(".pac_init_section")))
#else
#define __PAC_INIT_SECTION
#endif

#endif
