/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef DISP_CONFIG_H
#define DISP_CONFIG_H

#include <linux/platform_device.h>
#include <linux/types.h>
#include <linux/clk.h>
#include <dpu/soc_dpu_define.h>
#include <dpu/dpu_lbuf.h>
#include <platform_include/display/dkmd/dkmd_res_mgr.h>
#include <platform_include/display/dkmd/dkmd_dpu.h>
#include "dksm_debug.h"

#ifndef max_of_three
#define max_of_three(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))
#endif

#ifndef array_len
#define array_len(array) (sizeof(array) / sizeof((array)[0]))
#endif

#ifndef ceil_div
#define ceil_div(val, al) (((val) + ((al) - 1)) / (al))
#endif

#define DTS_OFFLINE_SCENE_ID_MAX 3

#define PERCENT 100
#define HZ_TO_US 1000000
#define DPU_DYN_CONFIG_MAX 10
#define DPU_DEFAULT_FPS 60

struct lbuf_static_reserved_config {
	int32_t part_id;
	int32_t scene_id;
	uint32_t reserved_lb;
};

struct lbuf_static_reserved_configs {
	uint32_t config_num;
	struct lbuf_static_reserved_config *lb_static_reserved_cfg;
};

struct lbuf_part_res_config_info {
	int32_t part_id;
	uint32_t max_lb;
};

struct lbuf_part_node_config_info {
	uint32_t part_num;
	struct lbuf_part_res_config_info *lb_cfg_info;
};

struct dpu_config {
	char __iomem *ip_base_addrs[DISP_IP_MAX];
	uint32_t lbuf_size;
	union dpu_version version;
	uint32_t xres;
	uint32_t yres;
	uint32_t product_type;
	uint32_t offline_scene_id_count;
	uint32_t offline_scene_ids[DTS_OFFLINE_SCENE_ID_MAX];

	uint32_t pmctrl_dvfs_enable;
	struct clk *clk_gate_edc;
};

struct lbuf_node_config_info {
	int32_t hw_node_id;
	uint32_t max_lb;
};

struct lbuf_pool_config_info {
	uint32_t node_size;
	struct lbuf_node_config_info *node_config_info;
};

struct lbuf_pool_node_config_info {
	uint32_t pool_size;
	struct lbuf_pool_config_info *lb_cfg_info;
};

struct lbuf_conn_pool_config_info {
	int32_t hw_pool_id;
	uint32_t node_size;
	int32_t *hw_node_ids;
};

enum lbuf_conn_pool_config_info_index {
	CONN_POOL_INDEX_0,
	CONN_POOL_INDEX_1,
	CONN_POOL_INDEX_2,
	CONN_POOL_INDEX_3,
	CONN_POOL_INDEX_4,
	CONN_POOL_INDEX_5,
	CONN_POOL_INDEX_6,
	CONN_POOL_INDEX_7,
	CONN_POOL_INDEX_MAX,
};

struct lbuf_conn_pool_node_config_info {
	uint32_t conn_pool_index;
	struct lbuf_conn_pool_config_info *lb_conn_pool_cfg_info;
};

struct lbuf_conn_config_info {
	int32_t cur_opr;
	int32_t nxt_opr;
	uint32_t conn_pool_size;
	struct lbuf_conn_pool_node_config_info *lb_conn_pool_node_cfg_info;
};

struct dpu_dyn_config_info {
	uint32_t addr_offset;
	uint32_t value;
};

struct info_src_dbuf_depth {
	uint32_t depth;
	uint32_t level0;
	uint32_t level1;
	uint32_t level2;
};

struct dpu_dyn_configs {
	uint32_t config_cnt;
	struct dpu_dyn_config_info dyn_configs[DPU_DYN_CONFIG_MAX];
};

struct dbuf_config_info {
	uint32_t addr_offset[DPU_DBUF_REG_NUM];
	uint32_t value[DPU_DBUF_REG_NUM];
};

struct dbuf_calc_thd_input {
	uint32_t xres;
	uint32_t yres;
	uint32_t dsc_en;
	uint32_t dsc_out_width;
	uint32_t fps;
	uint32_t scene_id;
};

extern struct dpu_config g_dpu_config_data;

int32_t dpu_init_config(struct platform_device *device);

static inline bool dpu_config_enable_pmctrl_dvfs(void)
{
	return (g_dpu_config_data.pmctrl_dvfs_enable == 1);
}

static inline char __iomem *dpu_config_get_ip_base(uint32_t ip)
{
	if (ip >= DISP_IP_MAX)
		return NULL;

	return g_dpu_config_data.ip_base_addrs[ip];
}

static inline uint32_t dpu_config_get_lbuf_size(void)
{
	return g_dpu_config_data.lbuf_size;
}

static inline uint64_t dpu_config_get_version_value(void)
{
	return g_dpu_config_data.version.value;
}

static inline void dpu_config_set_screen_info(uint32_t xres, uint32_t yres)
{
	if (g_dpu_config_data.xres == 0 && g_dpu_config_data.yres == 0) {
		g_dpu_config_data.xres = xres;
		g_dpu_config_data.yres = yres;
	}
}

static inline void dpu_config_get_screen_info(uint32_t *xres, uint32_t *yres)
{
	*xres = g_dpu_config_data.xres;
	*yres = g_dpu_config_data.yres;
}

static inline uint32_t* dpu_config_get_offline_scene_ids(uint32_t *count)
{
	*count = g_dpu_config_data.offline_scene_id_count;
	return g_dpu_config_data.offline_scene_ids;
}

static inline void dpu_config_set_product_type(uint32_t product_type)
{
	g_dpu_config_data.product_type = product_type;
}

static inline uint32_t dpu_config_get_product_type(void)
{
	return g_dpu_config_data.product_type;
}

uint32_t *dpu_config_get_scf_lut_addr_tlb(uint32_t *length);
uint32_t *dpu_config_get_arsr_lut_addr_tlb(uint32_t *length);
void dpu_config_dvfs_vote_exec(uint64_t clk_rate);
uint32_t dpu_config_get_version(void);

struct lbuf_pool_node_config_info *dpu_get_lb_pool_node_config_info(void);
struct lbuf_conn_pool_node_config_info *dpu_get_lb_conn_pool_node_config_info(int32_t cur_opr,
	int32_t nxt_opr, uint32_t *pool_size);
uint32_t dpu_get_pool_a_lb_max_size(void);
uint32_t dpu_get_pool_b_srot_lb_num(void);
uint32_t dpu_get_pool_b_lb_constraint(uint32_t lb_num);
uint32_t dpu_is_support_ebit(void);
void dpu_print_dvfs_vote_status(void);
void dpu_skip_intra_frame_idle_dvfs(void);
struct dbuf_config_info *dbuf_get_config_info(const struct dbuf_calc_thd_input *dbuf_calc_input);

struct lbuf_part_node_config_info *dpu_get_lb_part_node_config_info(void);
struct lbuf_static_reserved_configs *dpu_get_lb_static_reserved_config(void);

void dpu_get_dyn_config_info(struct dpu_dyn_configs *dyn_config, uint32_t xres, uint32_t yres,
	uint32_t dsc_en, uint32_t dsc_out_width, uint32_t fps);
uint32_t get_line_num_perlb(uint32_t width);

#endif /* DISP_CONFIG_H */
