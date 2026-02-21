/*
 * om_total_log_debug.h
 *
 * debug for socdsp
 *
 * Copyright (c) 2013-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __OM_TOTAL_LOG_DEBUG_H__
#define __OM_TOTAL_LOG_DEBUG_H__

#include <linux/kernel.h>
#include "dsp_om.h"
#ifdef ENABLE_HIFI_DEBUG
void socdsp_save_total_log(enum socdsp_om_work_id work_id, const unsigned char *addr, unsigned int len);
int open_the_total_log_saving(void);
int close_the_total_log_saving(void);
void resource_release_for_save_totallog(void);
int send_dsp_set_totallog_saving_state(unsigned short is_open_total_log_saving);
#else
static inline void socdsp_save_total_log(enum socdsp_om_work_id work_id, 
	const unsigned char *addr, unsigned int len) { return; }
static inline int open_the_total_log_saving(void) { return 0; }
static inline int close_the_total_log_saving(void) { return 0; }
static inline void resource_release_for_save_totallog(void) { return; }
static inline int send_dsp_set_totallog_saving_state(unsigned short is_open_total_log_saving) { return 0; }
#endif
#endif /* __OM_TOTAL_LOG_DEBUG_H__ */
