/*
 * asp_codec_gt_proxy_common.h -- proxy codec driver common
 *
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
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

#ifndef ASP_CODEC_GT_PROXY_COMMON_H
#define ASP_CODEC_GT_PROXY_COMMON_H

enum notify_mcu_cmd_type {
	CMD_OPEN_ROUTE = 1,
	CMD_OPEN_ROUTE_RESP,
	CMD_CLOSE_ROUTE,
	CMD_CLOSE_ROUTE_RESP,
	CMD_DSP_RESET,
	CMD_DSP_RESET_RESP,
    CMD_CNT,
};

struct send_to_mcu_mesg {
	int route;
	int sn;
};

#ifdef CONFIG_SND_GT_PROXY_CODEC
int send_to_mcu(enum notify_mcu_cmd_type cmd, const void *buf, int len);
#endif

#ifdef CONFIG_HIFI_RESET_NOTIFY_MCU
void dsp_reset_notify_mcu(void);
#else
static inline void dsp_reset_notify_mcu(void) { return; };
#endif

#endif /* __ASP_CODEC_GT_PROXY_COMMON_H__ */
