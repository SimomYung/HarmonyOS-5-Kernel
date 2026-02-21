/*
 * dbg.h
 *
 * This is for log intf.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef VFMW_DEBUG
#define VFMW_DEBUG
#include "vcodec_types.h"

typedef enum {
	PRN_FATAL = 0,
	PRN_ERROR,
	PRN_WARN,
	PRN_ALWS,
	PRN_DBG,
	PRN_CTRL,
	PRN_EVENT,
} print_type;


#define DEFAULT_PRINT_ENABLE (1 << PRN_FATAL | 1 << PRN_ERROR | 1 << PRN_WARN | 1 << PRN_ALWS)
#define default_print_check(type) ((DEFAULT_PRINT_ENABLE & (1 << type)) != 0)

#define dprint(type, fmt, arg...) \
	do { \
		if (default_print_check(type)) { \
			printk(KERN_ALERT "VDEC: <%s: %d>"fmt, __func__, __LINE__, ##arg); \
		} \
	} while (0)

#define vfmw_assert(cond) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, "assert error: condition %s not match.\n", #cond); \
			return; \
		} \
	} while (0)

#define vfmw_assert_prnt(cond, fmt, arg...) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, fmt, ##arg); \
			return; \
		} \
	} while (0)

#define vfmw_assert_goto(cond, tag) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, "assert error: condition %s not match.\n", #cond); \
			goto tag; \
		} \
	} while (0)

#define vfmw_assert_goto_prnt(cond, tag, fmt, arg...) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, fmt, ##arg); \
			goto tag; \
		} \
	} while (0)

#define vfmw_assert_ret(cond, ret) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, "assert error: condition %s not match.\n", #cond); \
			return ret; \
		} \
	} while (0)

#define vfmw_assert_ret_prnt(cond, ret, fmt, arg...) \
	do { \
		if (!(cond)) { \
			dprint(PRN_ERROR, fmt, ##arg); \
			return ret; \
		} \
	} while (0)

#endif
