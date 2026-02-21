/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: monitor device status file
 * Author: lixiangtao
 * Create: 2021-01-28
 */

#ifndef LOADMONITOR_PLAT_H
#define LOADMONITOR_PLAT_H

#ifndef CONFIG_PM_DEL_PLATFORM_ADDR
#include <soc_pmctrl_interface.h>
#endif
#include <freqdump_plat_common.h>

#define PERI_VOTE_8_PROFILE
#define CONFIG_DUBAI_LOADMONITOR

#define MONITOR0_IP_MASK		0xFFFFFFFF
#define MAX_PERI_MONITOR0_DEVICE	bit_count(MONITOR0_IP_MASK) /* 32 */

#define MONITOR1_IP_MASK		0xFFFFFFFF
#define MAX_PERI_MONITOR1_DEVICE	bit_count(MONITOR1_IP_MASK) /* 32 no svfd */

#define AO_MONITOR0_IP_MASK		0xFFE1FF7F
#define MAX_MONITOR_IP_DEVICE_AO	bit_count(AO_MONITOR0_IP_MASK) /* 28 */

#define MAX_PERI_MONITOR_DEVICE		(MAX_PERI_MONITOR0_DEVICE + MAX_PERI_MONITOR1_DEVICE)
#define MAX_MONITOR_IP_DEVICE		(MAX_PERI_MONITOR_DEVICE + MAX_MONITOR_IP_DEVICE_AO)

#define LOADMONITOR_PERI_FREQ_CS	167120000
#define MONITOR_PERI_FREQ_CS		167120000
#define LOADMONITOR_PCLK		167

#define LOADMONITOR_AO_FREQ		64510000
#define MONITOR_AO_FREQ_CS		64510000

#define CHANNEL_SIZE	16
#define CHANNEL_SUM	CHANNEL_SIZE
#define PERI_VOTE_DATA	32U

#define DPM_PERIOD_MAX	(LOADMONITOR_PCLK * 1000 * 1000 * 25)
#define DPM_PERIOD_MIN	(LOADMONITOR_PCLK * 100 * 1000)

enum EN_FLAGS {
	ENABLE_NONE = 0x0,
	ENABLE_PERI0 = 0x2,
	ENABLE_PERI1 = 0x4,
	ENABLE_PERI = ENABLE_PERI0 | ENABLE_PERI1,
	ENABLE_AO = 0x10,
	ENABLE_DDR = 0x10000,
	ENABLE_VOTE = 0x20000,
	ENABLE_ALL = ENABLE_PERI | ENABLE_AO | ENABLE_DDR | ENABLE_VOTE,
};

/* notice: member has sequence */
struct loadmonitor_ips {
	struct ips_info peri0;
	struct ips_info peri1;
	struct ips_info ao;
};

#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION
 struct description   : PERI_VOTE_VOL_RANK_SFT_0 Register structure definition
                        Address Offset:0x000 Initial:0x0000 Width:32
 register description : PERI 投票源电压档位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_vote_vol_rank_0 : 3;  /* bit[0-2]  : PERI 投票源0电压档位
                                                                b'000 对应profile0档位；
                                                                b'001 对应profile1档位；
                                                                b'010 对应profile2档位；
                                                                b'011 对应profile3档位；
                                                                b'100 对应profile4档位；
                                                                b'101 对应profile5档位； 
                                                                b'110 对应profile6档位；
                                                                b'111 对应profile7档位 */
        unsigned int  peri_vote_vol_rank_1 : 3;  /* bit[3-5]  :  */
        unsigned int  peri_vote_vol_rank_2 : 3;  /* bit[6-8]  :  */
        unsigned int  peri_vote_vol_rank_3 : 3;  /* bit[9-11] :  */
        unsigned int  peri_vote_vol_rank_4 : 3;  /* bit[12-14]:  */
        unsigned int  reserved_0           : 1;  /* bit[15]   :  */
        unsigned int  reserved_1           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_0_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_0_END    (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_1_START  (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_1_END    (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_2_START  (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_2_END    (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_3_START  (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_3_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_4_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_peri_vote_vol_rank_4_END    (14)


/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION
 struct description   : PERI_VOTE_VOL_RANK_SFT_1 Register structure definition
                        Address Offset:0x004 Initial:0x0000 Width:32
 register description : PERI 投票源电压档位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_vote_vol_rank_5 : 3;  /* bit[0-2]  :  */
        unsigned int  peri_vote_vol_rank_6 : 3;  /* bit[3-5]  :  */
        unsigned int  peri_vote_vol_rank_7 : 3;  /* bit[6-8]  :  */
        unsigned int  peri_vote_vol_rank_8 : 3;  /* bit[9-11] :  */
        unsigned int  peri_vote_vol_rank_9 : 3;  /* bit[12-14]:  */
        unsigned int  reserved_0           : 1;  /* bit[15]   :  */
        unsigned int  reserved_1           : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_5_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_5_END    (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_6_START  (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_6_END    (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_7_START  (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_7_END    (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_8_START  (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_8_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_9_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_peri_vote_vol_rank_9_END    (14)


/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_UNION
 struct description   : PERI_VOTE_VOL_RANK_SFT_2 Register structure definition
                        Address Offset:0x02C Initial:0x0000 Width:32
 register description : PERI 投票源电压档位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_vote_vol_rank_10 : 3;  /* bit[0-2]  :  */
        unsigned int  peri_vote_vol_rank_11 : 3;  /* bit[3-5]  :  */
        unsigned int  peri_vote_vol_rank_12 : 3;  /* bit[6-8]  :  */
        unsigned int  peri_vote_vol_rank_13 : 3;  /* bit[9-11] :  */
        unsigned int  peri_vote_vol_rank_14 : 3;  /* bit[12-14]:  */
        unsigned int  reserved_0            : 1;  /* bit[15]   :  */
        unsigned int  reserved_1            : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_10_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_10_END    (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_11_START  (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_11_END    (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_12_START  (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_12_END    (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_13_START  (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_13_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_14_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_peri_vote_vol_rank_14_END    (14)


/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_3_UNION
 struct description   : PERI_VOTE_VOL_RANK_SFT_3 Register structure definition
                        Address Offset:0x03C Initial:0x0000 Width:32
 register description : PERI 投票源电压档位
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_vote_vol_rank_15 : 3;  /* bit[0-2]  :  */
        unsigned int  reserved_0            : 3;  /* bit[3-5]  :  */
        unsigned int  reserved_1            : 3;  /* bit[6-8]  :  */
        unsigned int  reserved_2            : 3;  /* bit[9-11] :  */
        unsigned int  reserved_3            : 3;  /* bit[12-14]:  */
        unsigned int  reserved_4            : 1;  /* bit[15]   :  */
        unsigned int  reserved_5            : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_3_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_3_peri_vote_vol_rank_15_START  (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_3_peri_vote_vol_rank_15_END    (2)


/*****************************************************************************
 struct               : SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION
 struct description   : PERI_VOTE_VOL_VALID Register structure definition
                        Address Offset:0x008 Initial:0x0000 Width:32
 register description : PERI 投票源有效状态
*****************************************************************************/
#ifndef __SOC_H_FOR_ASM__
typedef union {
    unsigned int      value;
    struct {
        unsigned int  peri_vote_vol_valid_0  : 1;  /* bit[0]    : PERI 投票源0有效状态
                                                                  b‘1 参与投票有效
                                                                  b’0 不参与投票无效 */
        unsigned int  peri_vote_vol_valid_1  : 1;  /* bit[1]    :  */
        unsigned int  peri_vote_vol_valid_2  : 1;  /* bit[2]    :  */
        unsigned int  peri_vote_vol_valid_3  : 1;  /* bit[3]    :  */
        unsigned int  peri_vote_vol_valid_4  : 1;  /* bit[4]    :  */
        unsigned int  peri_vote_vol_valid_5  : 1;  /* bit[5]    :  */
        unsigned int  peri_vote_vol_valid_6  : 1;  /* bit[6]    :  */
        unsigned int  peri_vote_vol_valid_7  : 1;  /* bit[7]    :  */
        unsigned int  peri_vote_vol_valid_8  : 1;  /* bit[8]    :  */
        unsigned int  peri_vote_vol_valid_9  : 1;  /* bit[9]    :  */
        unsigned int  peri_vote_vol_valid_10 : 1;  /* bit[10]   :  */
        unsigned int  peri_vote_vol_valid_11 : 1;  /* bit[11]   :  */
        unsigned int  peri_vote_vol_valid_12 : 1;  /* bit[12]   :  */
        unsigned int  peri_vote_vol_valid_13 : 1;  /* bit[13]   :  */
        unsigned int  peri_vote_vol_valid_14 : 1;  /* bit[14]   :  */
        unsigned int  peri_vote_vol_valid_15 : 1;  /* bit[15]   :  */
        unsigned int  reserved               : 16; /* bit[16-31]: 写mask bit，分别mask bit[15:0],只有当mask bit[x+16]为1时，bit[x]的写才有效 */
    } reg;
} SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION;
#endif
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_0_START   (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_0_END     (0)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_1_START   (1)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_1_END     (1)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_2_START   (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_2_END     (2)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_3_START   (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_3_END     (3)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_4_START   (4)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_4_END     (4)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_5_START   (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_5_END     (5)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_6_START   (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_6_END     (6)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_7_START   (7)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_7_END     (7)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_8_START   (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_8_END     (8)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_9_START   (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_9_END     (9)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_10_START  (10)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_10_END    (10)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_11_START  (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_11_END    (11)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_12_START  (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_12_END    (12)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_13_START  (13)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_13_END    (13)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_14_START  (14)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_14_END    (14)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_15_START  (15)
#define SOC_PMCTRL_PERI_VOTE_VOL_VALID_peri_vote_vol_valid_15_END    (15)
#endif

struct dubai_peri {
	SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_0_UNION vote0;
	SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_1_UNION vote1;
	SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_2_UNION vote2;
        SOC_PMCTRL_PERI_VOTE_VOL_RANK_SFT_3_UNION vote3;
	SOC_PMCTRL_PERI_VOTE_VOL_VALID_UNION enable;
};

struct loadmonitor_data {
	struct loadmonitor_ips ips;
	struct loadmonitor_ddr ddr;
	struct dubai_peri peri;
};

#define MAX_NUM_LOGIC_IP_DEVICE	(sizeof(struct loadmonitor_ips) / \
				 sizeof(struct ips_info) * MAX_SENSOR_NUM)

struct delay_time_with_freq {
	unsigned int monitor_enable_flags;
	unsigned int peri0;
	unsigned int peri1;
	unsigned int ao;
};
#endif
