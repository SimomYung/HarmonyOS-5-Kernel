/* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved. */
#ifndef __DDR_CAPACITY_ADAPTION_H__
#define __DDR_CAPACITY_ADAPTION_H__

struct ddr_ch24_hole_info {
	unsigned int size_gbyte : 8;
	unsigned int addr_gbyte : 8;
};

struct ddr_ch24_intlv_info {
	unsigned int intlv_area_sel : 8;
	unsigned int intlv_area_addr0 : 8;
	unsigned int intlv_area_addr1 : 8;
	struct ddr_ch24_hole_info hole_info_ac;
	struct ddr_ch24_hole_info hole_info_bd;
};

struct cpu_fastwakeup_cfg {
	unsigned int pd_fastwakeup0_hc;
	unsigned int pd_fastwakeup0_lit;
	unsigned int pd_fastwakeup1;
	unsigned int fcm_cpu_ctrl8;
	unsigned int fcm_cpu_ctrl10;
	unsigned int hwp_fastwakeup;
};

/* edit two struct as same time */

#define MAX_ADDR_SEG_NUM 8
typedef struct {
	unsigned long long start_addr; /* unit: byte */
	unsigned long long length; /* unit: mbyte */
} ddr_phy_addr_seg_str;

typedef struct {
	long long segment_num;
	ddr_phy_addr_seg_str addr_seg[MAX_ADDR_SEG_NUM];
} ddr_phy_addr_info_str;

struct ddr_l1bus_remap_info {
	unsigned int intlv_area_addr0 : 8;
	unsigned int intlv_area_addr1 : 8;
	unsigned int intlv_area_sel : 8;
	unsigned int intlv_gran : 8;
	unsigned int scramble_en : 8;
	unsigned int addr_shift_mode : 8;
};

#endif
