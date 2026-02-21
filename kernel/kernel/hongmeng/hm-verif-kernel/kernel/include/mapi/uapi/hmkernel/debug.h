/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Headers of arm_debug module
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 13:58:42 2019
 */
#ifndef UAPI_HMKERNEL_DEBUG_H
#define UAPI_HMKERNEL_DEBUG_H

#include <hmkernel/types.h>

#define WCR_ENABLE	0x1
#define WCR_LSC_SHIFT	0x3
#define WCR_BAS_SHIFT	0x5

enum __debug_config_cmd {
	DEBUG_CONFIG_GET_DEBUG_INFO,	/* get debug info */
	DEBUG_CONFIG_GET_HWBKPT_INFO, /* arg len: 1, only use addr */
	DEBUG_CONFIG_GET_WP_REG,	/* arg len: 1, only use index */
	DEBUG_CONFIG_SET_WP_REG,	/* arg len: 3 */
	DEBUG_CONFIG_SET_WP_REG_ADDR, /* arg len: 3 */
	DEBUG_CONFIG_SET_WP_REG_CTRL, /* arg len: 3 */
	DEBUG_CONFIG_WP_ENABLE,		/* arg len: 1, only use index */
	DEBUG_CONFIG_WP_DISABLE,	/* arg len: 1, only use index */
	DEBUG_CONFIG_GET_BP_REG,	/* arg len: 1, only use index */
	DEBUG_CONFIG_SET_BP_REG,	/* arg len: 3 */
	DEBUG_CONFIG_SET_BP_REG_ADDR, /* arg len: 3 */
	DEBUG_CONFIG_SET_BP_REG_CTRL, /* arg len: 3 */
	DEBUG_CONFIG_BP_ENABLE,		/* arg len: 1, only use index */
	DEBUG_CONFIG_BP_DISABLE,	/* arg len: 1, only use index */
	DEBUG_CONFIG_SS_ENABLE,		/* enable single step, only for aarch64 */
	DEBUG_CONFIG_SS_DISABLE,	/* disable single step, only for aarch64 */
	DEBUG_CONFIG_INVAL,
};

enum __debug_ctx_use_state {
	DEBUG_NOT_USED = 0,
	DEBUG_USED_PDEBUG = 1, /* default debug ctx state is DEBUG_USED_PDEBUG */
	DEBUG_USED_PERF = 2,
};

enum __hwbkpt_type {
	HWBKPT_BP,
	HWBKPT_WP,
};

struct __debug_config_arg {
	__u32 use_state;
	__u32 reg_idx;	/* config[0] */
	union {		/* config[1,2] */
		struct {
			__u64 addr;
			__u64 ctrl;
		} watch_point;
		struct {
			__u64 addr;
			__u64 ctrl;
		} break_point;
		__u64 hwbkpt_addr;
	};
};

struct __debug_config_result {
	union {
		struct {
			__u32 version;
			__u32 brp_nr;
			__u32 wrp_nr;
			__u32 max_wp_len;
		} debug_info;
		struct {
			__u64 addr;
			__u32 ctrl;
		} watch_point;
		struct {
			__u64 addr;
			__u32 ctrl;
		} break_point;
		struct {
			enum __hwbkpt_type type;
			__u32 idx;
		} hwbkpt_info;
	};
};

#endif
