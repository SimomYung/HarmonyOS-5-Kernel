/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Hyperhold latency accounting feature
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 14:36:05 2024
 */
#ifndef SYSMGR_INTAPI_MEM_HPACCT_H
#define SYSMGR_INTAPI_MEM_HPACCT_H
#include <intapi/inspector/xacct/xacct.h>

#define HP_SLOT_MAX 7

enum hpacct_scene {
	HP_BATCH_OUT,
	HP_RECLAIM_IN,
	HP_FAULT_OUT,
	HP_SCENE_MAX,
};

enum hpacct_action {
	HP_ISOLATE_EXTS,
	HP_IO_GET,
	HP_READ_ASYNC,
	HP_MOVE_EXTENT,
	HP_ALLOC_EXTENT,
	HP_COLLECT_OBJS,
	HP_WRITE_ASYNC,
	HP_EXT_INSERT,
	HP_IO_PUT,
	HP_WAIT_SYNC,
	HP_MOVE_OBJ,
	HP_ACTION_MAX,
};

enum hpacct_metric {
	HP_LAT = XACCT_LAT,
	HP_CNT = XACCT_CNT,
	HP_METRIC_MAX,
};

#ifdef CONFIG_HPACCT
struct hpacct {
	struct xacct xacct;
	uint64_t lat_slot[HP_SLOT_MAX];
	uint64_t cnt_slot[HP_SLOT_MAX];
};
struct xacct *hpacct_current_xacct(void);
int mem_hpacct_read(char *data, size_t data_len);
int hpacct_module_init(void);
#else
struct hpacct {};
static inline struct xacct *hpacct_current_xacct(void) { return NULL; }
static inline int hpacct_module_init(void) { return E_HM_OK; }
#endif

#if (defined CONFIG_HPACCT) || (defined CONFIG_HPACCT_TRACE_ONLY)
void hpacct_start(struct hpacct *hpacct, enum hpacct_scene scene);
void hpacct_action_start(struct xacct *xacct, enum hpacct_action action);
void hpacct_action_end(struct xacct *xacct, enum hpacct_action action);
void hpacct_end(struct hpacct *hpacct);
#else
static inline void hpacct_start(struct hpacct *hpacct, enum hpacct_scene scene) { }
static inline void hpacct_action_start(struct xacct *xacct, enum hpacct_action action) { }
static inline void hpacct_action_end(struct xacct *xacct, enum hpacct_action action) { }
static inline void hpacct_end(struct hpacct *hpacct) { }
#endif

#endif
