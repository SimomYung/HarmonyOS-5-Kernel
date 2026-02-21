/**
 * @file ukmd_cmdlist.h
 * @brief The ioctl the interface file for cmdlist node.
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __UKMD_CMDLIST_H__
#define __UKMD_CMDLIST_H__

#include <linux/types.h>
#include <linux/bits.h>

#define CMDLIST_IOCTL_MAGIC 'C'

#define CMDLIST_CREATE_CLIENT _IOWR(CMDLIST_IOCTL_MAGIC, 0x1, struct cmdlist_node_client)
#define CMDLIST_SIGNAL_CLIENT _IOWR(CMDLIST_IOCTL_MAGIC, 0x2, struct cmdlist_node_client)
#define CMDLIST_RELEASE_CLIENT _IOWR(CMDLIST_IOCTL_MAGIC, 0x3, struct cmdlist_node_client)
#define CMDLIST_APPEND_NEXT_CLIENT _IOWR(CMDLIST_IOCTL_MAGIC, 0x4, struct cmdlist_node_client)
#define CMDLIST_DUMP_USER_CLIENT _IOWR(CMDLIST_IOCTL_MAGIC, 0x5, struct cmdlist_node_client)
#define CMDLIST_DUMP_SCENE_CLIENT _IOWR(CMDLIST_IOCTL_MAGIC, 0x6, struct cmdlist_node_client)
#define CMDLIST_ALLOC_MEM _IOWR(CMDLIST_IOCTL_MAGIC, 0x7, struct cmdlist_mem_request)
#define CMDLIST_FREE_MEM _IOWR(CMDLIST_IOCTL_MAGIC, 0x8, struct cmdlist_mem_request)
#define CMDLIST_INFO_GET _IOWR(CMDLIST_IOCTL_MAGIC, 0x9, unsigned)

enum {
	REGISTER_CONFIG_TYPE = BIT(0),
	DATA_TRANSPORT_TYPE = BIT(1),
	DM_TRANSPORT_TYPE = BIT(2),
	SCENE_NOP_TYPE = BIT(3),
	OPR_REUSE_TYPE = BIT(4),
};

struct cmdlist_node_client {
	uint32_t dev_id;
	uint32_t id;

	uint32_t type;
	uint32_t scene_id;
	uint32_t node_size;
	uint32_t dst_addr;

	uint32_t append_next_id;
	uint64_t viraddr;
	uint64_t phyaddr;
	uint32_t valid_payload_size;
};

enum {
	MEMORY_TYPE_DMA,
	MEMORY_TYPE_ION,
};

enum {
	ADDR_MODE_PA,
	ADDR_MODE_IOVA,
	ADDR_MODE_VA,
};

struct cmdlist_mem_request {
	uint32_t size;
	uint64_t va_offset;
};

struct cmdlist_info {
	uint8_t memory_type;
	uint8_t use_kernel_mode;
	uint8_t addr_mode; // [0:1:2] <=> [pa:iova:va] mode
	uint8_t rsv[1];
	uint32_t pool_size;
	uint64_t viraddr_base;
	uint64_t dma_addr_base;
};

#endif
