#ifndef __DFX_SH_H__
#define __DFX_SH_H__

#include "../common/dfx_sh_common.h"

enum sh_type {
    sh_type_start,
	sh_type_cluster_0 = sh_type_start,
	sh_type_cluster_1,
	sh_type_cluster_2,
    sh_type_cluster_max = sh_type_cluster_2,
	sh_type_l3,
	sh_type_peri,
	sh_type_isp,
	sh_type_max, // sh_type_max cannot exceed SH_CFG_INFO_TYPE_MAX to avoid array out-of-bounds
};

#define SH_SW_CNT_THRESHOLD_CL0       3
#define SH_SW_CNT_THRESHOLD_CL1       3
#define SH_SW_CNT_THRESHOLD_CL2       3
#define SH_SW_CNT_THRESHOLD_L3        3
#define SH_SW_CNT_THRESHOLD_PERI      3
#define SH_SW_CNT_THRESHOLD_ISP       0

static const struct sh_sw_cnt g_sh_sw_cnt[sh_type_max] = 
{
	/* [count] [threshold] [reserved] [enable] */
	{ SH_VALUES_NOT_NEEDED, SH_SW_CNT_THRESHOLD_CL0,
	 SH_VALUES_NOT_NEEDED, SH_CFG_ENABLE},
	{ SH_VALUES_NOT_NEEDED, SH_SW_CNT_THRESHOLD_CL1,
	 SH_VALUES_NOT_NEEDED, SH_CFG_ENABLE},
	{ SH_VALUES_NOT_NEEDED, SH_SW_CNT_THRESHOLD_CL2,
	 SH_VALUES_NOT_NEEDED, SH_CFG_ENABLE},
	{ SH_VALUES_NOT_NEEDED, SH_SW_CNT_THRESHOLD_L3,
	 SH_VALUES_NOT_NEEDED, SH_CFG_ENABLE},
	{ SH_VALUES_NOT_NEEDED, SH_SW_CNT_THRESHOLD_PERI,
	 SH_VALUES_NOT_NEEDED, SH_CFG_ENABLE},
	{ SH_VALUES_NOT_NEEDED, SH_SW_CNT_THRESHOLD_ISP,
	 SH_VALUES_NOT_NEEDED, SH_CFG_DISABLE},
};

#define SH_CLEAR_INTERVAL_ENABLE_CFG  0b11111

/* 1 month */
#define SH_CLEAR_INTERVAL_CL0    (30 * 24 * 3600)
#define SH_CLEAR_INTERVAL_CL1    (30 * 24 * 3600)
#define SH_CLEAR_INTERVAL_CL2    (30 * 24 * 3600)
#define SH_CLEAR_INTERVAL_L3     (30 * 24 * 3600)
#define SH_CLEAR_INTERVAL_PERI   (30 * 24 * 3600)
#define SH_CLEAR_INTERVAL_ISP    0

static const unsigned int g_sh_clear_interval[sh_type_max] = 
{
	SH_CLEAR_INTERVAL_CL0,
    SH_CLEAR_INTERVAL_CL1,
    SH_CLEAR_INTERVAL_CL2,
    SH_CLEAR_INTERVAL_L3,
    SH_CLEAR_INTERVAL_PERI,
	SH_CLEAR_INTERVAL_ISP,
};

#define SH_DFV_ENABLE      SH_CFG_DISABLE
#define SH_DFV_THRESHOLD   0

#define SH_CLEAR_DATA_ENABLE_CFG  0b00

#define SH_VERSION_RTC_TIME       0
#define SH_VERSION                2

#define SH_DDR_CHANNEL    0
#define SH_DDR_CNT        0
#define SH_DDR_CFG        SH_CFG_DISABLE

#endif /* __DFX_SH_H__ */
