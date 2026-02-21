/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#ifndef __DDR_DEFINE_KERNEL_H__
#define __DDR_DEFINE_KERNEL_H__

#define RESERVED_MNTN_PHY_MEM_BASE	0x2F880000
#define RESERVED_MNTN_PHY_MEM_SIZE	(0x620000)

#define RDR_DFA_MEM_SIZE		0x11000
#define RDR_FFTS_MEM_SIZE		0x10000
#define RDR_DDR_MEM_SIZE		0x40000
#define RDR_DDR_SEC_MEM_SIZE		0x1000
#define RDR_DDR_MEM_BASE		(RESERVED_MNTN_PHY_MEM_BASE + RESERVED_MNTN_PHY_MEM_SIZE -\
	RDR_FFTS_MEM_SIZE - RDR_DDR_MEM_SIZE - RDR_DFA_MEM_SIZE - RDR_DDR_SEC_MEM_SIZE)

#define DDR_MNTN_INFO_OFFSET		0x3C000
#define DDR_MNTN_INFO_BASE		(RDR_DDR_MEM_BASE + DDR_MNTN_INFO_OFFSET)
#define DDR_MNTN_INFO_SIZE		0x400

#define GET_BITS_VALUE(data, start, end) (((data) >> (start)) & ((1 << ((end) - (start) + 1)) - 1))

#define DDRMNTN_RECORD_DDR_FREQ_START			(2)
#define DDRMNTN_RECORD_DDR_FREQ_END				(5)
#define DDRMNTN_RECORD_LAST_FREQ_START			(6)
#define DDRMNTN_RECORD_LAST_FREQ_END			(9)
#define DDRMNTN_RECORD_CORE_VOLT_START			(10)
#define DDRMNTN_RECORD_CORE_VOLT_END			(19)
#define DDRMNTN_RECORD_LAST_CORE_VOLT_START		(20)
#define DDRMNTN_RECORD_LAST_CORE_VOLT_END		(29)

#define QICE_FREQ_INFO_ADDR		0xFDB23428UL

#define RANK_MAX_NUM 2
#define CHANNEL_MAX_NUM 2
#define BYTELANE_MAX_NUM 2
struct ddr_storage_aging_info {
	unsigned int eye_area_count_rdq[RANK_MAX_NUM][CHANNEL_MAX_NUM][BYTELANE_MAX_NUM];
	unsigned int rdqscyc[RANK_MAX_NUM][CHANNEL_MAX_NUM][BYTELANE_MAX_NUM];
};
struct ddr_mntn_info_stru {
	unsigned int qice_int_magic;
	unsigned int mntn_record;
	unsigned int qice_freq_info;
};

#define QICE_INT_MAGICNUM		0xCDE01199

#include <pm_def.h>
enum ddr_sr_dfs_tickmark_point
{
	TICK_FLOOR_VOTE_AP      = 0x0,
	TICK_FLOOR_VOTE_HIFI_ID = 0x1,
	TICK_FLOOR_VOTE_LPMCU   = 0x2,
	TICK_FLOOR_VOTE_IOMCU   = 0x3,
	TICK_FLOOR_VOTE_LIT_LAT = 0x4,
	TICK_FLOOR_VOTE_BIG_LAT = 0x5,
	TICK_FLOOR_VOTE_MID_LAT = 0x6,
	TICK_FLOOR_VOTE_LATSTAT = 0x7,
	TICK_FLOOR_VOTE_MODEM   = 0x8,
	TICK_FLOOR_VOTE_CPU_XPU = 0x9,
	TICK_FLOOR_VOTE_GPU_XPU = 0xa,
	TICK_FLOOR_VOTE_RESERVED11 = 0xb,
	TICK_FLOOR_VOTE_RESERVED12 = 0xc,
	TICK_FLOOR_VOTE_RESERVED13 = 0xd,
	TICK_FLOOR_VOTE_RESERVED14 = 0xe,
	TICK_FLOOR_VOTE_DSS        = 0xf,
	TICK_SR_DFS_SLEEP_FREQ_TIMES,
	TICK_SR_DFS_NO_SLEEP_FREQ_TIMES,
	TICK_SR_DFS_CAN_SLEEP_TIMES,
	TICK_SR_DFS_CAN_NOT_SLEEP_TIMES,
	TICK_VOTE_USED_MAX,
	/* must be last */
	TICK_SR_DFS_MRAK_MAX = LPMCU_TELE_MNTN_DATA_DDR_MARK_SIZE / 4,
};
struct ddr_sr_dfs_tick_mntn_stru {
	unsigned int tick[TICK_VOTE_USED_MAX];
};
#endif /* __DDR_DEFINE_KERNEL_H__ */
