/*
 *
 * dfx diaginfo baseinfo module.
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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
#ifndef __DFX_DIAGINFO_BASEINFO_H__
#define __DFX_DIAGINFO_BASEINFO_H__

#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>

#define BASE_INFO_SINGLE_MAX_LEN		256

struct base_info_item {
	const char *keyword;
	int (*base_info_get_fn)(const void *rdr_memory, size_t rdr_size, char *buf, size_t len);
};

#ifdef CONFIG_DFX_SEC_QIC_V300
struct cfgbus_timeout_info {
	u32 lpm_sliding_window_addr;
	u32 access_addr_from_lpm;
	u32 cfgbus_timeout_reason;
};
#endif

#ifdef CONFIG_DFX_DIAGINFO_BASEINFO
int diaginfo_backup_rdr_memory(void *rdr_memory, size_t size);
int dfx_diaginfo_collect_base_info(void);
int dfx_diaginfo_get_base_info(char *buf, size_t size);
#ifdef CONFIG_DFX_SEC_QIC_V300
int get_qic_cfgbus_timeout_info(char *out_buf, size_t buf_size);
#endif
#else
static inline int diaginfo_backup_rdr_memory(void *rdr_memory, size_t size) {return -1;}
static inline int dfx_diaginfo_collect_base_info(void) {return -1;}
static inline int dfx_diaginfo_get_base_info(char *buf, size_t size) {return -1;}
#ifdef CONFIG_DFX_SEC_QIC_V300
static inline int get_qic_cfgbus_timeout_info(char *out_buf, size_t buf_size) {return -1;}
#endif
#endif
#endif
