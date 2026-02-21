/* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved. */
#ifndef __DDR_STAT_STRUCT_H__
#define __DDR_STAT_STRUCT_H__
#include <ddr/ddr_vote_statistic_struct.h>

#define DDR_STAT_FREQ_NUM_MAX 		10
#define DDR_BAND_MNTN_MAX_VALID_NUM 8
#define FREQ_TRACK_TIMES 			50
#define BANKWIDTH_MNTN_TIMES 		10
#define XPU_DDR_FLOOR_VOTE_NUMS (FLOOR_VOTE_HW_MAX)
#define MS_TO_S 1000
#define DATA_END 0xFF
#define UNINT_MAX 0xFFFFFFFF
#define ddr_slice_to_ms(n)   (((n) * 1000) / 32764)
#define ddr_us_to_ms(time_us) ((time_us) / 1000)
#define slice_to_ms(n)   ((n) * 32 / 1000)
#define flux_to_mbyte(n)    ((n) >> 16)

typedef enum {
	DDR_MNTN_BAND_ENABLE = 1,
	DDR_MNTN_BAND_INTERVAL,
	DDR_MNTN_BAND_DURATION,
	DDR_MNTN_BAND_THRESHOLD,
	DDR_MNTN_BAND_TRIGGER_ONCE,
	DDR_MNTN_BAND_MASTER_ID,
	DDR_MNTN_LP_ENABLE,
	DDR_MNTN_LP_INTERVAL,
	DDR_MNTN_LP_STAT_ENABLE,
} DDR_MNTN_CTRL_SET_ID;

typedef enum {
	DDR_TRIGGER_LOAD_CMD_UP = 0,
	DDR_TRIGGER_LOAD_DATA_UP,
	DDR_TRIGGER_LOAD_DATA_DOWN,
	DDR_TRIGGER_VOTE_FLOOR,
	DDR_TRIGGER_VOTE_TOP,
	DDR_TRIGGER_DFS_MAX
} DDR_TRIGGER_DFS_ID;

typedef enum {
	DDR_MNTN_MID_CPU = 0,
	DDR_MNTN_MID_GPU,
	DDR_MNTN_MID_DSS,
	DDR_MNTN_MID_ISP1,
	DDR_MNTN_MID_ISP2,
	DDR_MNTN_MID_SC = 5,
	DDR_MNTN_MID_VDEC,
	DDR_MNTN_MID_NPU,
	DDR_MNTN_MID_HIFI,
	DDR_MNTN_MID_MODEM = 9,
	DDR_BAND_MNTN_MID_MAX
} DDR_BAND_MNTN_MID;

typedef struct {
	unsigned int readable;
	unsigned int freq_num;
	unsigned int stat_ms[DDR_STAT_FREQ_NUM_MAX];
} ddr_freq_stat;

typedef struct {
	unsigned char freq_id[FREQ_TRACK_TIMES];
	unsigned int timestamp[FREQ_TRACK_TIMES];
} ddr_freq_track;

typedef struct {
	unsigned char freq_id[FREQ_TRACK_TIMES];
	unsigned char trigger_source_id[FREQ_TRACK_TIMES];
	unsigned int timestamp[FREQ_TRACK_TIMES];
} ddr_load_track;

typedef struct {
	unsigned int enable;
	ddr_freq_stat freq_stat;
	ddr_freq_track freq_track;
	ddr_load_track load_track;
} ddr_stat_info;

typedef struct {
	unsigned int enable;
	unsigned int interval;
	unsigned int duration;
	unsigned int threshold;
	unsigned int trigger_once;
	unsigned int master_id_enable[DDR_BAND_MNTN_MID_MAX];
} ddr_bandwidth_ctrl_info;

typedef struct {
	unsigned int freq_id[BANKWIDTH_MNTN_TIMES];
	unsigned int timestamp[BANKWIDTH_MNTN_TIMES];
	unsigned int flux[BANKWIDTH_MNTN_TIMES];
} ddr_bandwidth_master_info;

typedef struct {
	unsigned char logic_id;
	unsigned int stat_ms[DDR_STAT_FREQ_NUM_MAX];
} xpu_ddr_vote_stat;

typedef struct {
	xpu_ddr_vote_stat vote_stat[XPU_DDR_FLOOR_VOTE_NUMS];
} ddr_freq_vote_info;

typedef struct  {
	unsigned int id;
	unsigned int stat[DDR_STAT_FREQ_NUM_MAX];
} xpu_ddr_lp_statistic_msg;

typedef struct {
	xpu_ddr_lp_statistic_msg lp_statistic_msg[DDR_LP_STATISTIC_MAX];
} ddr_lp_statistic_info;

typedef struct {
	unsigned int enable;
	unsigned int interval;
} ddr_lp_ctrl_info;

typedef struct {
	ddr_bandwidth_ctrl_info bandwidth_ctrl_info;
	ddr_bandwidth_master_info bandwidth_master_info[DDR_BAND_MNTN_MID_MAX];
} ddr_bandwidth_info;

typedef struct {
	ddr_lp_ctrl_info lp_ctrl_info;
	ddr_freq_vote_info freq_vote_info;
	ddr_lp_statistic_info lp_statistic_info;
	ddr_stat_info stat_info;
} ddr_lp_info;
#endif
