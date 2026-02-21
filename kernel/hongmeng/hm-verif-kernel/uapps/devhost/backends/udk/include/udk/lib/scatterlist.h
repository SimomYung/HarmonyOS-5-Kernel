/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: scatter list for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 12 14:56:34 2023
 */
#ifndef __UDK_SCATTERLIST_H__
#define __UDK_SCATTERLIST_H__

#include <udk/types.h>

#define for_each_sg(sglist, sg, nr, __i)				\
	for ((__i) = 0, (sg) = (sglist); (__i) < (nr); (__i)++, (sg)++)

struct udk_scatterlist {
	unsigned long dma_address;
	unsigned int dma_length;
};

struct udk_sg_table {
	struct udk_scatterlist *sgl; /* the list */
	unsigned int nents; /* number of mapped entries */
};

#endif /* __UDK_SCATTERLIST_H__ */
