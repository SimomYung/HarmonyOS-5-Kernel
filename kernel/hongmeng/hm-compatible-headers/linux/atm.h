/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/atm.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 05 17:21:24 2021
 */

#ifndef LINUX_ATM_H
#define LINUX_ATM_H

#define ATM_CELL_SIZE			53
#define ATM_CELL_PAYLOAD		48

#if defined(__sparc__) || defined(__ia64__)
#define __ATM_API_ALIGN	__attribute__((aligned(8)))
#else
#define __ATM_API_ALIGN
#endif

struct sockaddr_atmpvc {
	unsigned short	sap_family;
	struct {
		short	itf;
		short	vpi;
		int	vci;
	} sap_addr __ATM_API_ALIGN;
};

#endif
