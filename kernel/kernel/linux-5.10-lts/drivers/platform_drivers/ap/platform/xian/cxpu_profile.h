/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: xpu profile exposed
 * Create: 2021-09-10
 */
#ifndef __CXPU_PROFILE_H__
#define __CXPU_PROFILE_H__

#define CXPU_MAX_TEMPZONE_NUM		9
#define CXPU_LOW_TEMPZONE_IDX		0
#define CXPU_LOW_TEMPZONE_BASE		1
#define CXPU_NORMAL_TEMPZONE_NUM	(CXPU_MAX_TEMPZONE_NUM - CXPU_LOW_TEMPZONE_BASE)

enum {
	CPU_CHIP_PHONE_STANDARD = 0, /* Bin1 */
	CPU_CHIP_PHONE_LITE, /* Bin7 */
	CPU_CHIP_PHONE_LITE2, /* Bin8 */
	CPU_CHIP_WIFI_STANDARD, /* Bin9 */
	CPU_CHIP_WIFI_LITE, /* Bin11 */
	CPU_CHIP_PHONE_TURBO, /* turbo */
	MAX_CPU_CHIP_LEVEL,
};

struct profile_feature_switch {
	unsigned int dfv : 1;
	unsigned int hpm_avs : 1;
	unsigned int ffs : 1;
	unsigned int idle_div : 1;
	unsigned int active_defense : 1;
};

union apll_ctrl0_un {
	unsigned int value;
	struct {
		unsigned int en : 1;
		unsigned int bypass : 1;
		unsigned int refdiv : 6;
		unsigned int fbdiv : 12;
		unsigned int postdiv1 : 3;
		unsigned int postdiv2 : 3;
		unsigned int reserved : 6;
	} reg;
};

union apll_ctrl1_un {
	unsigned int value;
	struct {
		unsigned int fracdiv : 24;
		unsigned int int_mod : 1;
		unsigned int cfg_vld : 1;
		unsigned int reserved : 6;
	} reg;
};

#define build_check_valid(condition)	((void)sizeof(char[1 - 2 * (int)(!!(condition))]))
#define is_struct_aligned_int(_struct)	build_check_valid(sizeof(_struct) % sizeof(int) != 0)

#if defined(CONFIG_LPMCU_DIRECT_ACCESS_CXPU) || defined(CONFIG_ACPU_DIRECT_ACCESS_CXPU)
struct xpu_cpu_profile { /* use struct NOT from cxpu itself */
#else
struct cpu_profile {
#endif
	unsigned short freq;
	unsigned char ppll0_div;
	unsigned char fout2x;
	unsigned short pim_delta;
	unsigned short logic_avs_mv[CXPU_MAX_TEMPZONE_NUM];
	union apll_ctrl0_un apll_ctrl0;
	union apll_ctrl1_un apll_ctrl1;
	unsigned int peri_mem_volt_link_mv;
	unsigned short logic_volt_mv;
	unsigned short mem_volt_mv;
	unsigned short ddr_minlimit_0;
	unsigned short ddr_minlimit_1;
	unsigned char idle_clkdiv;
	unsigned char vdm_div;
	unsigned char valid_flag;
	unsigned char lowtemp_flag;
	unsigned short mem_avs_mv;
	unsigned short reserved;
	struct profile_feature_switch func_sw;
};

struct feature_switch {
	unsigned int dvfs_en : 1;
	unsigned int pasensor_en : 1;
	unsigned int fdcs_monitor_en : 1;
	unsigned int wfi_div_en : 1;
	unsigned int l2_idlediv_en : 1;
	unsigned int pcr_maxpower_en : 1;
	unsigned int pcr_didt_en : 1;
	unsigned int ddr_link_en : 1;
	unsigned int fine_freq : 1;
	unsigned int fdcs_ctrl : 1;
	unsigned int hw_onsys_avs_en : 1;
	unsigned int ppll0_switch : 1;
	unsigned int fout2x_en : 1;
	unsigned int fdcs_en : 1;
	unsigned int lpcs_en : 1;
	unsigned int doze_ctrl : 1;
	unsigned int fdcs_dcc : 1;
	unsigned int pim_en : 1;
};

struct cpu_status {
	unsigned int dvfs_running : 1;
	unsigned int dvs_running : 1;
	unsigned int op_enable : 1;
};

#define MAX_RT_NAME	8
#if defined(CONFIG_LPMCU_DIRECT_ACCESS_CXPU) || defined(CONFIG_ACPU_DIRECT_ACCESS_CXPU)
struct xpu_cpu_dvfs_runtime { /* use struct NOT from cxpu itself */
#else
struct cpu_dvfs_runtime {
#endif
	unsigned char name[MAX_RT_NAME];

	unsigned char cur_prof; /* offset:0x8 */
	unsigned char tar_prof;
	unsigned char last_prof;
	unsigned char tzone;

	unsigned char low_tctrl_prof;
	unsigned char min_prof;
	unsigned char max_prof;
	unsigned char vdm_step_prof; /* the lowest profile we switch vdm div */

	unsigned char ppll0_step_prof; /* the lowest profile we use apll */
	unsigned char real_target;
	unsigned char cpu_peri_limit;
	unsigned char cur_memidx;

	unsigned char tar_memidx;
	unsigned char cur_logic_cfg; /* logic volt cfg */
	unsigned char cur_mem_cfg; /* mem volt cfg */
	unsigned char last_logic_cfg;

	unsigned char ignore_tzone;
	unsigned char reserved0_0;
	unsigned char reserved0_1;
	unsigned char pim_volt_cfg;

	unsigned short dfv_prof;
	unsigned short reserved1;
	unsigned int reserved2;
	unsigned int reserved3;

	struct feature_switch sw;
	struct cpu_status stat; /* size:0x30 */
};

#endif /* __CXPU_PROFILE_H__ */
