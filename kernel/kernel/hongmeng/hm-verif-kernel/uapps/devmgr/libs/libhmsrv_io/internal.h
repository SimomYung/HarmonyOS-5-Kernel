/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service library internal header
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#ifndef ULIBS_LIBHMSRV_IO_INTERNAL_H
#define ULIBS_LIBHMSRV_IO_INTERNAL_H

#include <stdint.h>
#include <libhmucap/ucap.h>

#define max(a, b) ({		\
	typeof(a) __a = (a);	\
	typeof(b) __b = (b);	\
	__a > __b ? __a : __b;	\
})
#define min(a, b) ({		\
	typeof(a) __a = (a);	\
	typeof(b) __b = (b);	\
	__a < __b ? __a : __b;	\
})

extern rref_t g_devmgr_ap_rref;

rref_t hmsrv_io_require(void);

#endif	/* ULIBS_LIBHMSRV_IO_INTERNAL_H */
