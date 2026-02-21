/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: This module is to support wifi multilink feature.
 * Create: 2023-12-29
 */

#ifndef AMAX_MULTILINK_STATEMACHINE_H_
#define AMAX_MULTILINK_STATEMACHINE_H_

#include "amax_multilink_main.h"
#include "dps_data_txrx.h"
#include <linux/kallsyms.h>
#include <huawei_platform/net/amax/amax_multilink_interface.h>

#define AMAX_ROAM_FIRSTLINK_TIMEOUT_THRESHOLD 3000000000 //3s:3000000000
#define AMAX_ROAM_SECONDLINK_STRAT_TIMEOUT_THRESHOLD 5000000000 //5s:5000000000
#define AMAX_ROAM_SECONDLINK_TIMEOUT_THRESHOLD 3000000000 //3s:3000000000
#define AMAX_ROAM_GET_LINK_INFO_RETRANS 500000000 //500ms:500000000
#define AMAX_ROAM_GET_LINK_INFO_RETRANS_TIMES 3

/*双频漫游成功/失败原因*/
#define AMAX_ROAM_SUCC 0
#define AMAX_ROAM_FAIL 1
#define AMAX_ROAM_FDOING 2 //第一条链双频漫游未结束
#define AMAX_ROAM_SDOING 3 //第二条链双频漫游未结束
#define AMAX_ROAM_FUNDOING 4 //第一条链双频漫游未开始，收到漫游结果
#define AMAX_ROAM_SUNDOING 5 //第二条链双频漫游未开始，收到漫游结果
#define AMAX_ROAM_FBAND_DISMATCH 6 //第一条链漫游频段不匹配
#define AMAX_ROAM_SBAND_DISMATCH 7 //第二条链漫游频段不匹配
#define AMAX_ROAM_FTIMEOUT 8 //第一条链漫游结果上报超时
#define AMAX_ROAM_STIMEOUT 9 //第二条链漫游结果上报超时
#define AMAX_ROAM_INFO_TYPE_DISMATCH 10 //漫游结果上报类型不匹配(非漫游开始/漫游完成上报信息)
#define AMAX_ROAM_START_MSG_NULL 12 //无下发双频漫游开始信息
#define AMAX_ROAM_END_MSG_NULL 13 //无下发双频漫游结束信息
#define AMAX_ROAM_ML_END 14 //Multilink退出
#define AMAX_ROAM_FROAMFAIL 15 //第一条链漫游结果上报失败
#define AMAX_ROAM_SROAMFAIL 16 //第二条链漫游结果上报失败
#define AMAX_ROAM_DIFF_AP 17 //未漫游至同一AP

/* 漫游主状态机状态 */
typedef enum {
	INIT_STATUS = 0,
	FIRST_LINK_ROAM_ONGOING = 1,
	FIRST_LINK_ROAM_DONE = 2,
	SECOND_LINK_ROAM_ONGOING = 3,
	SECOND_LINK_ROAM_FAIL = 4,
	ROAM_STATE_BUTT,
} ml_roam_state_enum;

 /* 状态机切换的原语 */
typedef enum {
	ROAM_START_REQ = 0,
	ROAM_SUCC_REQ = 1,
	ROAM_FAIL_REQ = 2,
	RESTART_REQ = 3,
	ML_FSM_INPUT_BUFF,
} ml_roam_fsm_input_type_enum;
	
/* 信道频点 */
typedef enum {
	ML_BAND_2GHZ,
	ML_BAND_5GHZ,
	ML_BAND_60GHZ,
	ML_BAND_6GHZ,
	ML_BAND_S1GHZ,
	ML_BAND_LC,
	ML_BAND_NULL,
	NUM_ML_ROAM_BANDS,
} ml_roam_info_band_enum;
typedef uint8_t ml_roam_info_band_enum_uint8;

typedef struct {
	ml_roam_info_band_enum ml_roam_band;
	ml_roam_info_band_enum ml_roam_ops_band;
	ml_roam_state_enum ml_roam_main_state;
	uint8_t auc_bssid[ETHER_ADDR_LEN];
	uint32_t ml_roam_result;
} amax_roam_info_stru;

typedef struct{
	uint8_t force_send_mode;
	uint8_t fn_main;
	uint8_t fn_assist;
} amax_chan_info_struct;

typedef uint32_t (*amax_roam_fsm_func)(amax_roam_info_stru *pst_roam_info, amax_chan_info_struct *p_param);

uint32_t amax_ml_roam_init(void);
void amax_ml_roam_exit(void);

static uint32_t amax_ml_fsm_null_fn(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_first_link_roaming_start_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_first_link_done_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_first_link_roam_fail_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_second_link_roaming_start_func(amax_roam_info_stru *roam_info,
	amax_chan_info_struct *chan_info);
static uint32_t amax_ml_first_link_done_error_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_second_link_roam_done_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_second_link_roam_fail_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info);
static uint32_t amax_ml_second_link_roam_fail_resume_func(amax_roam_info_stru *roam_info,
	amax_chan_info_struct *chan_info);

static void amax_send_link_info_req(void);
static void amax_ml_roam_fail_process(void);
void amax_ml_roam_init_process(void);
int32_t amax_ml_roam_info_report(amax_dualband_roam_info_stru *dualband_roam_info, amax_chan_info_struct *chan_info);
ml_roam_state_enum amax_ml_get_roam_state(void);
int32_t get_dualband_send_on(void);
void set_dualband_send_on(int32_t sendmode);
int32_t get_first_link_band(void);
uint32_t amax_choose_valid_link(dps_mgmt_msg *msg_info);
void set_link_bssid(uint8_t *bssid, uint8_t linkid);

#endif /* _AMAX_MULTILINK_STATEMACHINE_H_ */
