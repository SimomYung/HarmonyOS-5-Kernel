/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_HTS_API_H
#define HVGR_HTS_API_H

#include "hvgr_defs.h"
#include "hvgr_ioctl_softjob.h"

#define HTS_EVENT_MAX_NUM                        1024
#define HVGR_HTS_SSID_START                      64
#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
/*
 * load hts event refcnt from DDR
 * Constraints: this api must be called after hvgr_fcp_load_by_poll, Otherwise the mmu is
 * not enable and load failed.
 */
void hvgr_hts_load_event_table(struct hvgr_device * const gdev);

/*
 * store event refcnt to DDR
 * Constraints: this api must be called after hvgr_fcp_load_by_poll, Otherwise the mmu is
 * not enable and store failed.
 */
void hvgr_hts_store_event_table(struct hvgr_device * const gdev);

/* alloc memory for events refconuts */
int hvgr_hts_alloc_events_memory(struct hvgr_device * const gdev, struct hvgr_ctx *ctx);

/* free events memory */
void hvgr_hts_free_events_memory(struct hvgr_device * const gdev);

/* lock and cache hts events */
void hvgr_hts_event_lock(struct hvgr_device *gdev);

/* unlock hts events */
void hvgr_hts_event_unlock(void);

int hvgr_hts_init(struct hvgr_device * const gdev);

void hvgr_hts_term(struct hvgr_device *gdev);

void hvgr_hts_no_consumer_irq(struct hvgr_device * const gdev);

bool hvgr_hts_is_waitqueue_empty(struct hvgr_device * const gdev);

uint32_t hvgr_hts_get_ref_cnt(struct hvgr_device *gdev, uint32_t event_id);

void hvgr_hts_get_ssid(struct hvgr_ctx *ctx);

void hvgr_hts_set_ssid(struct hvgr_ctx *ctx, uint32_t channel_id);

void hvgr_hts_no_consumer_irq_enable(struct hvgr_device * const gdev);

void hvgr_hts_no_consumer_irq_disable(struct hvgr_device * const gdev);

bool hvgr_hts_wait_entry_enqueue(struct hvgr_cq_ctx * const cq_ctx);

void hvgr_hts_wait_pendinglist_clear(struct hvgr_device *gdev, struct hvgr_cq_ctx * const cq_ctx);

void hvgr_hts_wait_list_in_schedule(struct hvgr_device * const gdev, enum sch_type type);

void hvgr_hts_store_event_through_register(struct hvgr_device * const gdev);

int hvgr_hts_ctx_init(struct hvgr_cq_ctx *cq_ctx);

long hvgr_ioctl_hts_clear_event(struct hvgr_ctx * const ctx,
	union hvgr_ioctl_para_hts_clear_event * const para);

void hvgr_hts_block_proc(struct hvgr_ctx * const ctx);

void hvgr_hts_alloc_static_asid_and_ssid(struct hvgr_ctx * const ctx);

void hvgr_ctx_free_hts_assid(struct hvgr_ctx *ctx);
#else
/*
 * load hts event refcnt from DDR
 * Constraints: this api must be called after hvgr_fcp_load_by_poll, Otherwise the mmu is
 * not enable and load failed.
 */
static inline void hvgr_hts_load_event_table(struct hvgr_device * const gdev) {}

/*
 * store event refcnt to DDR
 * Constraints: this api must be called after hvgr_fcp_load_by_poll, Otherwise the mmu is
 * not enable and store failed.
 */
static inline void hvgr_hts_store_event_table(struct hvgr_device * const gdev) {}

/* alloc memory for events refconuts */
static inline int hvgr_hts_alloc_events_memory(struct hvgr_device * const gdev, struct hvgr_ctx *ctx)
{
	return 0;
}

/* free events memory */
static inline void hvgr_hts_free_events_memory(struct hvgr_device * const gdev) {}

/* lock and cache hts events */
static inline void hvgr_hts_event_lock(struct hvgr_device *gdev) {}

/* unlock hts events */
static inline void hvgr_hts_event_unlock(void) {}

static inline int hvgr_hts_init(struct hvgr_device * const gdev)
{
	return 0;
}

static inline void hvgr_hts_term(struct hvgr_device *gdev) {}

static inline void hvgr_hts_no_consumer_irq(struct hvgr_device * const gdev) {}

static inline bool hvgr_hts_is_waitqueue_empty(struct hvgr_device * const gdev)
{
	return true;
}

static inline uint32_t hvgr_hts_get_ref_cnt(struct hvgr_device *gdev, uint32_t event_id)
{
	return 0;
}

static inline void hvgr_hts_set_ssid(struct hvgr_ctx *ctx, uint32_t channel_id) {}

static inline void hvgr_hts_no_consumer_irq_enable(struct hvgr_device * const gdev) {}

static inline void hvgr_hts_no_consumer_irq_disable(struct hvgr_device * const gdev) {}

static inline bool hvgr_hts_wait_entry_enqueue(struct hvgr_cq_ctx * const cq_ctx)
{
	return false;
}

static inline void hvgr_hts_wait_pendinglist_clear(struct hvgr_device *gdev,
	struct hvgr_cq_ctx * const cq_ctx) {}

static inline void hvgr_hts_wait_list_in_schedule(struct hvgr_device * const gdev,
	enum sch_type type) {}

static inline void hvgr_hts_store_event_through_register(struct hvgr_device * const gdev) {}

static inline int hvgr_hts_ctx_init(struct hvgr_cq_ctx *cq_ctx)
{
	return 0;
}

static inline void hvgr_hts_block_proc(struct hvgr_ctx * const ctx) {}
#endif

#endif