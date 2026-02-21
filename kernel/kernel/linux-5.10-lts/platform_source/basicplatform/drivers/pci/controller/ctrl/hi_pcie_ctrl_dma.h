/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller dma logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_DMA_H
#define HI_PCIE_CTRL_DMA_H

#include <linux/types.h>
#include <linux/spinlock.h>

#include "../hi_pcie_internal_api.h"

struct hi_pcie_sq_desc {
	union {
		struct {
			uint32_t opcode : 4;
			uint32_t drop : 1;
			uint32_t nw : 1;
			uint32_t wd_barrier : 1;
			uint32_t rd_barrier : 1;
			uint32_t ldie : 1;
			uint32_t rdie : 1;
			uint32_t loop_barrier : 1;
			uint32_t spd_barrier : 1;
			uint32_t attr0 : 3;
			uint32_t resv1 : 1;
			uint32_t at : 2;
			uint32_t p3 : 1;
			uint32_t p4 : 1;
			uint32_t pf_num : 3;
			uint32_t vf_en : 1;
			uint32_t vf_num : 8;
		} bits;
		uint32_t dw;
	} dw0;
	union {
		struct {
			uint32_t pasid : 20;
			uint32_t er : 1;
			uint32_t pmr : 1;
			uint32_t prf_en : 1;
			uint32_t stabco : 1;
			uint32_t msi_vect_l : 3;
		} bits;
		uint32_t dw;
	} dw1;
	union {
		struct {
			uint32_t flowid_rmt : 8;
			uint32_t msi_vect_h : 3;
			uint32_t flowid_loc_l : 5;
			uint32_t th : 1;
			uint32_t ph : 2;
			uint32_t flowid_loc_h : 3;
			uint32_t resv1 : 2;
			uint32_t attr : 3;
			uint32_t at : 2;
			uint32_t th_dst : 1;
			uint32_t ph_dst : 2;
		} bits;
		uint32_t dw;
	} dw2;
	uint32_t lentgh;
	uint32_t src_addr_l;
	uint32_t src_addr_h;
	uint32_t dst_addr_l;
	uint32_t dst_addr_h;
};

struct hi_pcie_cq_desc {
	uint32_t resv0;
	uint32_t resv1;
	union {
		struct {
			uint32_t sqhd : 16;
			uint32_t resv : 16;
		} bits;
		uint32_t dw;
	} dw0;
	union {
		struct {
			uint32_t reserved : 16;
			uint32_t valid : 1;
			uint32_t status : 15;
		} bits;
		uint32_t dw;
	} dw1;
};

union hi_pcie_re_com {
	struct {
		uint32_t valid;
		uint32_t addr_l;
		uint32_t addr_h;
		uint32_t size;
	} message;
	uint8_t raw[4096];
};

struct hi_pcie_dma_work {
	void __iomem *src_addr;
	dma_addr_t src_addr_phy;
	void __iomem *dest_addr;
	dma_addr_t dest_addr_phy;
	u32 length;
	u32 mode;
};

struct hi_pcie_dma_item {
	void (*callback)(void *data, int ret);
	void *data;
};

struct hi_pcie_dma_queue {
	u32 channel;
	struct hi_pcie_sq_desc *sq_base;
	dma_addr_t sq_base_phy;
	u32 sq_depth;
	u32 sq_tail;
	u32 sq_head;
	u32 sq_count;
	struct hi_pcie_cq_desc *cq_base;
	dma_addr_t cq_base_phy;
	u32 cq_depth;
	u32 cq_head;
	u32 ccs;
	spinlock_t lock;
	struct hi_pcie_dma_item *items;
};

enum {
	DMA_MODE_SMALL_PACKET = 1,
	DMA_MODE_REMOTE_TO_LOCAL,
	DMA_MODE_LOCAL_TO_REMOTE,
	DMA_MODE_LOCAL_TO_LOCAL,
};

struct hi_pcie_ctrl;

void pcie_ctrl_reset_dma_queue(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue);
int pcie_ctrl_alloc_dma_queue(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue);
void pcie_ctrl_free_dma_queue(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue);
int pcie_ctrl_init_dma(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_deinit_dma(struct hi_pcie_ctrl *ctrl);
void pcie_ctrl_init_dma_queue(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue);
void pcie_ctrl_submit_dma_work(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue,
	struct hi_pcie_dma_work *work, void (*callback)(void *data, int ret), void *data);
void pcie_ctrl_read_from_remote(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue,
	struct hi_pcie_dma_para *para);
void pcie_ctrl_write_to_remote(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue,
	struct hi_pcie_dma_para *para);
void pcie_ctrl_dma_finish(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue);
void pcie_ctrl_dump_queue_state(struct hi_pcie_ctrl *ctrl, struct hi_pcie_dma_queue *queue);
#endif
