/* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved. */
#ifndef __DDR_RELEASE_MESSAGE_H__
#define __DDR_RELEASE_MESSAGE_H__

#include <global_ddr_map.h>
#include <ddr/ddr_capacity_adaption.h>
#include <ddr/ddr_stat_struct.h>

#define DDRINFO_SHARE_MEM_ADDR			HISI_SUB_RESERVED_UEFI_DDRINFO_SHARE_BASE

#define MAX_DDR_FREQ_NUM	16
#define MAX_L1BUS_FREQ_NUM	8
#define DDR_FREQ_RATIO 1000
#define MAX_FREP_NUM 12
#define DMD_PASERSOR_NUM 4
#define DMD_TRAIN_NUM	10
typedef struct {
	unsigned int train[DMD_TRAIN_NUM];
	unsigned int pasersor1[DMD_PASERSOR_NUM];
	unsigned int pasersor2[DMD_PASERSOR_NUM];
	unsigned int ddr_temp;
	unsigned int avs_voltage[MAX_FREP_NUM];
}ddr_dmd_info_param;

typedef struct {
	unsigned int manufactory_id;
	unsigned int ddr_total_size_mbyte;
	ddr_phy_addr_info_str ddr_trans_phy_addr;
	unsigned int ddr_type;
	unsigned int revision_id1;
	unsigned int revision_id2;
	unsigned int density;
	unsigned int intlv_area_addr0;
	unsigned int intlv_area_addr1;
	unsigned int ddr_sub_type;
	unsigned int reserved[13];
	unsigned int ddr_freq[MAX_DDR_FREQ_NUM];
	unsigned int ddr_freq_num;
	unsigned int l1bus_freq[MAX_L1BUS_FREQ_NUM];
	unsigned int l1bus_freq_num;
	ddr_lp_info lp_info;
	ddr_bandwidth_info bandwidth_info;
	ddr_dmd_info_param dmd_pama;
} ddr_release_message_str;


#endif
