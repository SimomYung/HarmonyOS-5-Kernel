/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#ifndef __DDR_DEFINE_KERNEL_H__
#define __DDR_DEFINE_KERNEL_H__

#define RESERVED_MNTN_PHY_MEM_BASE	0x2F880000
#define RESERVED_MNTN_PHY_MEM_SIZE	(0x620000)

#define RDR_DFA_MEM_SIZE		0x11000
#define RDR_FFTS_MEM_SIZE		0x10000
#define RDR_DDR_MEM_SIZE		0x40000
#define RDR_VDEC_MEM_SIZE		0x20000
#define RDR_DDR_SEC_MEM_SIZE		0x1000
#define RDR_DDR_MEM_BASE		(RESERVED_MNTN_PHY_MEM_BASE + RESERVED_MNTN_PHY_MEM_SIZE -\
	RDR_FFTS_MEM_SIZE - RDR_DDR_MEM_SIZE - RDR_DFA_MEM_SIZE - RDR_VDEC_MEM_SIZE - RDR_DDR_SEC_MEM_SIZE)

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

#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE	0x10941000
#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_SIZE	(0x10000)

#define RANK_MAX_NUM 2
#define CHANNEL_MAX_NUM 4
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

#endif /* __DDR_DEFINE_KERNEL_H__ */
