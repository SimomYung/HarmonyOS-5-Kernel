/*
 * hoisp.h
 *
 * Copyright (c) 2023-2024 HISI Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _HISI_OISP_H
#define _HISI_OISP_H

#include <linux/scatterlist.h>

#define OISP_MSG_MEM_SIZE 0x20000

int hoisp_shared_mem_map(struct scatterlist *sgl, unsigned long isp_addr, unsigned long isp_size);
int hoisp_shared_mem_unmap(void);
int hoisp_msg_mem_map(struct sg_table *sg_table);
int hoisp_msg_mem_unmap(void);

extern int hoisp_start_prepare(void);
extern int hoisp_stop_cleanup(void);
#endif /* _HISI_OISP_H */

