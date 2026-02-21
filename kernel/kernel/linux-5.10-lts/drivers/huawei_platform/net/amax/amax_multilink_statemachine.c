/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: This module is to support wifi multilink feature.
 * Create: 2023-12-29
 */
#include <linux/netdevice.h>
#include "amax_multilink_statemachine.h"
#include "amax_multilink_tool.h"
#include "amax_multilink_stat.h"

static uint32_t g_first_link_band = ML_BAND_NULL;
static uint32_t g_second_link_band = ML_BAND_NULL;
static uint32_t g_operating_link_band = ML_BAND_NULL;
static uint8_t g_first_link_bssid[ETHER_ADDR_LEN] = {0};
static uint8_t g_second_link_bssid[ETHER_ADDR_LEN] = {0};
static amax_roam_info_stru g_roam_info;
struct hrtimer g_ml_roam_timer;
struct hrtimer g_ml_roam_send_msg_timer;
static int32_t g_dualband_send_on = 0;
static int32_t g_dualband_send_get_link_times = 0;

static amax_roam_fsm_func g_ml_roam_fsm_func[ROAM_STATE_BUTT][ML_FSM_INPUT_BUFF];

static enum hrtimer_restart amax_ml_roam_timeout_handler(struct hrtimer *ml_timer)
{
	amax_ml_roam_fail_process();
	return HRTIMER_NORESTART;
}

static enum hrtimer_restart amax_ml_roam_wait_link_rsp_timeout_handler(struct hrtimer *ml_timer)
{
	amax_send_link_info_req();
	if (g_dualband_send_get_link_times < 0
		|| g_dualband_send_get_link_times >= AMAX_ROAM_GET_LINK_INFO_RETRANS_TIMES) {
		return HRTIMER_NORESTART;
	}
	return HRTIMER_RESTART;
}

static void amax_ml_roam_fsm_init(void)
{
	uint32_t state_idx;
	uint32_t event_idx;
	
	for (state_idx = 0; state_idx < ROAM_STATE_BUTT; state_idx++) {
		for (event_idx = 0; event_idx < ML_FSM_INPUT_BUFF; event_idx++) {
			g_ml_roam_fsm_func[state_idx][event_idx] = amax_ml_fsm_null_fn;
		}
	}
	g_ml_roam_fsm_func[INIT_STATUS][ROAM_START_REQ] = amax_ml_first_link_roaming_start_func;
	
	g_ml_roam_fsm_func[FIRST_LINK_ROAM_ONGOING][ROAM_SUCC_REQ] = amax_ml_first_link_done_func;
	g_ml_roam_fsm_func[FIRST_LINK_ROAM_ONGOING][ROAM_FAIL_REQ] = amax_ml_first_link_roam_fail_func;

	g_ml_roam_fsm_func[FIRST_LINK_ROAM_DONE][ROAM_START_REQ] = amax_ml_second_link_roaming_start_func;
	g_ml_roam_fsm_func[FIRST_LINK_ROAM_DONE][ROAM_FAIL_REQ] = amax_ml_first_link_done_error_func;

	g_ml_roam_fsm_func[SECOND_LINK_ROAM_ONGOING][ROAM_SUCC_REQ] = amax_ml_second_link_roam_done_func;
	g_ml_roam_fsm_func[SECOND_LINK_ROAM_ONGOING][ROAM_FAIL_REQ] = amax_ml_second_link_roam_fail_func;

	g_ml_roam_fsm_func[SECOND_LINK_ROAM_FAIL][RESTART_REQ] = amax_ml_second_link_roam_fail_resume_func;

	hrtimer_init(&g_ml_roam_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	g_ml_roam_timer.function = amax_ml_roam_timeout_handler;

	hrtimer_init(&g_ml_roam_send_msg_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	g_ml_roam_send_msg_timer.function = amax_ml_roam_wait_link_rsp_timeout_handler;
}

static void amax_roam_timer_restart(uint64_t timethreshold)
{
	ktime_t amax_kt;
	amax_kt = ktime_set(0, timethreshold);
	if (hrtimer_active(&g_ml_roam_timer)) {
		hrtimer_cancel(&g_ml_roam_timer);
		hrtimer_start(&g_ml_roam_timer, amax_kt, HRTIMER_MODE_REL);
	} else {
		hrtimer_start(&g_ml_roam_timer, amax_kt, HRTIMER_MODE_REL);
	}
}

static void amax_roam_timer_exit(void)
{
	if (hrtimer_active(&g_ml_roam_timer)) {
		hrtimer_cancel(&g_ml_roam_timer);
	}
}

static void amax_roam_send_link_info_timer_restart(uint64_t timethreshold)
{
	ktime_t amax_kt;
	amax_kt = ktime_set(0, timethreshold);
	if (hrtimer_active(&g_ml_roam_send_msg_timer)) {
		hrtimer_cancel(&g_ml_roam_send_msg_timer);
		hrtimer_start(&g_ml_roam_send_msg_timer, amax_kt, HRTIMER_MODE_REL);
	} else {
		hrtimer_start(&g_ml_roam_send_msg_timer, amax_kt, HRTIMER_MODE_REL);
	}
}

static void amax_roam_send_link_info_timer_exit(void)
{
	if (hrtimer_active(&g_ml_roam_send_msg_timer)) {
		hrtimer_cancel(&g_ml_roam_send_msg_timer);
	}
}

static void amax_ml_roam_clear(void)
{
	(void)memset_s(&g_roam_info, sizeof(amax_roam_info_stru), 0, sizeof(amax_roam_info_stru));
	(void)memset_s(&g_first_link_bssid, sizeof(g_first_link_bssid), 0, sizeof(g_first_link_bssid));
	(void)memset_s(&g_second_link_bssid, sizeof(g_second_link_bssid), 0, sizeof(g_second_link_bssid));
	g_first_link_band = ML_BAND_NULL;
	g_second_link_band = ML_BAND_NULL;
	g_dualband_send_on = 0;
	g_dualband_send_get_link_times = 0;
}

uint32_t amax_ml_roam_init(void)
{
	amax_print(PRINT_INFO, "amax_ml_roam_init.\n");
	amax_ml_roam_clear();
	amax_ml_roam_fsm_init();
	(void)memset_s(&g_amax_roam_start_info, sizeof(dps_action_info), 0, sizeof(dps_action_info));
	(void)memset_s(&g_amax_roam_end_info, sizeof(dps_action_info), 0, sizeof(dps_action_info));
	/* 初始化双频漫游统计 */
	if (amax_roam_sta_init() != SUCC) {
		amax_print(PRINT_ERROR, "amax_roam_sta_init initialize failed!\n");
		amax_roam_sta_exit();
		return FAIL;
	}
	if (amax_roam_err_init() != SUCC) {
		amax_print(PRINT_ERROR, "amax_roam_err_init initialize failed!\n");
		amax_roam_err_exit();
		return FAIL;
	}
	return SUCC;
}

void amax_ml_roam_exit(void)
{
	amax_print(PRINT_INFO, "amax_ml_roam_exit.\n");
	amax_ml_roam_clear();
	(void)memset_s(&g_amax_roam_start_info, sizeof(dps_action_info), 0, sizeof(dps_action_info));
	(void)memset_s(&g_amax_roam_end_info, sizeof(dps_action_info), 0, sizeof(dps_action_info));
	amax_roam_sta_exit();
	amax_roam_err_exit();
}

/*
 * 功能描述  : 调用双频漫游状态机函数表
 * 1.日    期  : 2024年1月2日
 *   作    者  : wifi
 *   修改内容  : 新生成函数
 */
uint32_t amax_ml_roam_main_fsm_action(amax_roam_info_stru *roam_info,
	ml_roam_fsm_input_type_enum event, amax_chan_info_struct *chan_info)
{
	if (roam_info == NULL) {
		amax_print(PRINT_ERROR, "{amax_ml_roam_main_fsm_action} roam info no exist.\n");
		return FAIL;
	}

	if (roam_info->ml_roam_main_state >= ROAM_STATE_BUTT) {
		amax_print(PRINT_ERROR, "{amax_ml_roam_main_fsm_action} roam state Error.\n");
		return FAIL;
	}

	if (event >= ML_FSM_INPUT_BUFF) {
		amax_print(PRINT_ERROR, "{amax_ml_roam_main_fsm_action} roam state event Error.\n");
		return FAIL;
	}

	return g_ml_roam_fsm_func[roam_info->ml_roam_main_state][event](roam_info, chan_info);
}
	
/*
 * 功能描述  : 改变双频漫游状态机状态
 * 1.日  期  : 2024年1月24日
 *   作  者  : wifi
 *   修改内容  : 新生成函数
 */
void amax_ml_roam_main_change_state(amax_roam_info_stru *roam_info, ml_roam_state_enum state)
{
	if (roam_info == NULL) {
		amax_print(PRINT_ERROR, "{amax_ml_roam_main_change_state} roam info no exist.\n");
		return;
	}
	amax_print(PRINT_INFO, "{amax_ml_roam_main_change_state::[%d]->[%d]}", roam_info->ml_roam_main_state, state);
	roam_info->ml_roam_main_state = state;
}

static uint32_t amax_ml_fsm_null_fn(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	amax_print(PRINT_INFO, "amax_ml_fsm_null_fn.\n");
	return AMAX_ROAM_SUCC;
}

bool ml_check_roam_para(amax_roam_info_stru *roam_info)
{
	if ((roam_info->ml_roam_main_state == FIRST_LINK_ROAM_ONGOING && roam_info->ml_roam_band != g_first_link_band)
		|| (roam_info->ml_roam_main_state == SECOND_LINK_ROAM_ONGOING && roam_info->ml_roam_band != g_second_link_band))
		return AMAX_ROAM_FAIL;
	return AMAX_ROAM_SUCC;
}

static void amax_fill_link_info_tlv(dps_action_tlv *dps_tlv, uint8_t fn_dev)
{
	uint32_t ret;
	if (dps_tlv == NULL) {
		amax_print(PRINT_ERROR, "[DPS] amax_fill_link_info_tlv dps_tlv is NULL\n");
		return;
	}

	dps_swap_uint16(dps_tlv->tlv_length, DPS_MSG_GET_INFO_LENGTH);
	dps_tlv->apMLD_id = 0;
	dps_tlv->type = TLV_TYPE_MULTILINK_BASIC_V2;
	dps_tlv->link_num = MAX_LINK_NUM;
	dps_tlv->length = DPS_MSG_GET_INFO_TLV_LENGTH;

	for (int i = 0; i < dps_tlv->link_num; i++) {
		uint8_t *sta_mac = dps_get_sta_mac_addr(fn_dev);
		if (sta_mac == NULL) {
			return;
		}
		dps_tlv->link_info[i].link_name = i == 0 ? g_first_link_band : g_second_link_band;
		dps_tlv->link_info[i].link_capa = 0;
		dps_tlv->link_info[i].link_id = i;
		dps_tlv->link_info[i].link_type = fn_dev;
		dps_tlv->link_info[i].link_channel_num = 0;
		dps_tlv->link_info[i].link_auth_type = 0;

		ret = memcpy_s(dps_tlv->link_info[i].link_bssid, ETHER_ADDR_LEN, sta_mac, ETHER_ADDR_LEN);
		if (ret != AMAX_ROAM_SUCC) {
			return;
		}
	}
}

static bool amax_second_bssid_is_null(void)
{
	for (int i = 0; i < ETHER_ADDR_LEN; i++) {
		if (g_second_link_bssid[i] != 0)
			return false;
	}
	return true;
}

static void amax_send_link_info_req()
{
	if (!amax_second_bssid_is_null()) {
		amax_print(PRINT_INFO, "[amax] amax_send_link_info_rsq already get\n");
		g_dualband_send_get_link_times = AMAX_ROAM_GET_LINK_INFO_RETRANS_TIMES;
		return;
	}
	uint32_t ret;
	dps_action_info amax_data_info;
	uint8_t fn_main = get_main_link_band();
	uint8_t fn_dev = g_first_link_band == fn_main ? DPS_DEV_MAIN : DPS_DEV_ASSIST;
	(void)memset_s(&amax_data_info, sizeof(dps_action_info), 0, sizeof(dps_action_info));
 
    dps_mgmt_msg *dps_data_info = (dps_mgmt_msg *)amax_data_info.param_buf;
	if (dps_data_info == NULL) {
        amax_print(PRINT_ERROR, "[DPS] dps_data_info is NULL\n");
        return;
    }
    dps_action_header *dps_action_info = (dps_action_header *)dps_data_info->data;
    dps_action_tlv *dps_tlv = (dps_action_tlv *)(dps_data_info->data + sizeof(dps_action_header));
    if (dps_action_info == NULL || dps_tlv == NULL) {
        amax_print(PRINT_ERROR, "[DPS] dps_action_info or dps_tlv is NULL\n");
        return;
    }

    ret = memcpy_s(dps_data_info->bssid, ETHER_ADDR_LEN, g_first_link_bssid, ETHER_ADDR_LEN);
	if (ret != AMAX_ROAM_SUCC) {
		return;
	}
	dps_data_info->send_dev = fn_dev;
	dps_action_info->version = DPS_VERSION_TYPE;
	dps_swap_uint16(dps_action_info->transactionID, TRANSITION_DEFAULT_ID);
	dps_swap_uint16(dps_action_info->operation, OPS_GET_LINK_INFO);
	amax_fill_link_info_tlv(dps_tlv, fn_dev);
	
	amax_data_info.param_size = DPS_MSG_GET_INFO_HEADER_LENGTH + DPS_MSG_GET_INFO_LENGTH;
	amax_print(PRINT_INFO, "[amax] amax_send_link_info_req to router[%d]\n", g_dualband_send_get_link_times);
	dps_alloc_and_send_frame(dps_data_info, amax_data_info.param_size);
	g_dualband_send_get_link_times++;
}

/*
 * 功能描述  : 双频漫游初始化处理函数
 * 1.日  期  : 2024年1月25日
 *   作  者  : wifi
 *   修改内容  : 新生成函数
 */
void amax_ml_roam_init_process(void)
{
	amax_ml_roam_clear();
	amax_roam_timer_exit();
	amax_roam_send_link_info_timer_exit();
	set_send_mode_param(FN_INVALID);
}

/*
 * 功能描述  : 双频漫游失败处理函数
 * 1.日  期  : 2024年1月25日
 *   作  者  : wifi
 *   修改内容  : 新生成函数
 */
static void amax_ml_roam_fail_process(void)
{
	amax_print(PRINT_ERROR, "{amax_ml_roam_fail_process} err[%d]\n", g_roam_info.ml_roam_result);
	/*通知上层处理*/
	if (g_roam_info.ml_roam_main_state <= FIRST_LINK_ROAM_ONGOING) {
		dps_report_amax_msg_to_userspace(NULL, 0, TRANSITION_AMAX_ROAM_DISABLE);
		amax_chr_update_dualband_roam_fail(g_roam_info.ml_roam_result, 0);
	} else {
		dps_report_amax_msg_to_userspace(NULL, 0, TRANSITION_AMAX_ROAM_FAIL);
		amax_chr_update_dualband_roam_fail(g_roam_info.ml_roam_result, 1);
	}
	/*通知路由处理*/
	if (g_amax_roam_end_info.param_size != 0 
		&& amax_choose_valid_link((dps_mgmt_msg *)g_amax_roam_end_info.param_buf) == AMAX_ROAM_SUCC) {
		dps_alloc_and_send_frame((dps_mgmt_msg *)g_amax_roam_end_info.param_buf, g_amax_roam_end_info.param_size);
	} else {
		amax_print(PRINT_ERROR, "{g_amax_roam_end_info} is null\n");
	}
	/*关闭双发冗余模式*/
	set_send_mode_param(FN_INVALID);
	
	/*信息清空*/
	amax_ml_roam_clear();
	amax_roam_chr_stat *roam_chr_info = (amax_roam_chr_stat *)get_amax_roam_stat();
	if(roam_chr_info != NULL) {
		amax_print(PRINT_INFO, "amax dual_band roam loss pkt rx[%d], tx_2[%d], tx_5[%d]\n", 
			roam_chr_info->rx_pkt_loss, roam_chr_info->tx_pkt_loss[0], roam_chr_info->tx_pkt_loss[1]);
	}
	return;
}

static uint32_t amax_ml_first_link_roaming_start_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	/*信道校验*/
	if (roam_info == NULL || chan_info == NULL) {
		return AMAX_ROAM_FAIL;
	}

	g_first_link_band = roam_info->ml_roam_band;
	if (chan_info->fn_main == roam_info->ml_roam_band) {
		g_second_link_band = chan_info->fn_assist;
	} else if (chan_info->fn_assist == roam_info->ml_roam_band) {
		g_second_link_band = chan_info->fn_main;
	} else {
		return AMAX_ROAM_FBAND_DISMATCH;
	}
	/*第一条链开始漫游*/
	if (g_amax_roam_start_info.param_size == 0 
		|| amax_choose_valid_link((dps_mgmt_msg *)g_amax_roam_start_info.param_buf) != AMAX_ROAM_SUCC) {
		amax_print(PRINT_ERROR, "{amax_ml_first_link_roaming_start_func} g_amax_roam_start_info is null\n");
		return AMAX_ROAM_START_MSG_NULL;
	}
	amax_print(PRINT_INFO, "{amax_ml_first_link_roaming_start_func} g_first_link_band = %d, "
	"g_second_link_band = %d\n", g_first_link_band, g_second_link_band);
	dps_alloc_and_send_frame((dps_mgmt_msg *)g_amax_roam_start_info.param_buf, g_amax_roam_start_info.param_size);
	dps_report_amax_msg_to_userspace(NULL, 0, TRANSITION_AMAX_ROAM_ENABLE);
	roam_info->ml_roam_ops_band = g_first_link_band;
	roam_info->ml_roam_result = AMAX_ROAM_FAIL;
	amax_ml_roam_main_change_state(roam_info, FIRST_LINK_ROAM_ONGOING);
	amax_roam_timer_restart(AMAX_ROAM_FIRSTLINK_TIMEOUT_THRESHOLD);
	set_send_mode_param(FN_DOUBLE);
	amax_chr_update_start();
	
	return AMAX_ROAM_SUCC;
}

uint32_t amax_choose_valid_link(dps_mgmt_msg *msg_info)
{
	if (msg_info == NULL) {
		amax_print(PRINT_ERROR, "{amax_choose_valid_link} msg_info is null\n");
		return AMAX_ROAM_FAIL;
	}
	uint8_t fn_mode = get_main_link_band();
	uint8_t *bssid = NULL;
	if(g_first_link_band == fn_mode) {
		msg_info->send_dev = DPS_DEV_ASSIST;
		bssid = amax_get_assist_bssid();
		if (bssid == NULL) {
			amax_print(PRINT_ERROR, "{amax_choose_valid_link} bssid is null\n");
			return AMAX_ROAM_FAIL;
		}
		if (memcpy_s(msg_info->bssid, ETHER_ADDR_LEN, bssid, ETHER_ADDR_LEN) != EOK) {
			amax_print(PRINT_ERROR, "{amax_choose_valid_link} memcpy fail\n");
			return AMAX_ROAM_FAIL;
		}
	} else if (g_second_link_band == fn_mode) {
		return AMAX_ROAM_SUCC;
	} else {
		amax_print(PRINT_ERROR, "{amax_choose_valid_link} roam band err\n");
		return AMAX_ROAM_FAIL;
	}
	return AMAX_ROAM_SUCC;
}

static uint32_t amax_ml_first_link_done_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	if (roam_info == NULL) {
		return AMAX_ROAM_FAIL;
	}
	int32_t msg_type;
	if (ml_check_roam_para(roam_info)) {
	    amax_print(PRINT_ERROR, "{amax_ml_first_link_done_func} roam_info check fail, "
		"roam success band = %d\n", roam_info->ml_roam_band);
	    amax_ml_roam_main_fsm_action(roam_info, ROAM_FAIL_REQ, chan_info);
		roam_info->ml_roam_result =  AMAX_ROAM_FBAND_DISMATCH;
        return AMAX_ROAM_FBAND_DISMATCH;
    }
    amax_ml_roam_main_change_state(roam_info, FIRST_LINK_ROAM_DONE);
    roam_info->ml_roam_ops_band = g_second_link_band;
	/* 双频漫游消息交互 */
    msg_type = g_first_link_band == chan_info->fn_main ? 
        TRANSITION_AMAX_MAIN_LINK_INFO_REQ : TRANSITION_AMAX_ASSIST_LINK_INFO_REQ;
	dps_report_amax_msg_to_userspace(roam_info->auc_bssid, ETHER_ADDR_LEN, msg_type);
	/* 如果处于双频漫游流程中，原路由双发结束 */
	if (g_amax_roam_end_info.param_size != 0 
		&& amax_choose_valid_link((dps_mgmt_msg *)g_amax_roam_end_info.param_buf) == AMAX_ROAM_SUCC) {
		dps_alloc_and_send_frame((dps_mgmt_msg *)g_amax_roam_end_info.param_buf, g_amax_roam_end_info.param_size);
	} else {
		amax_print(PRINT_ERROR, "{amax_ml_first_link_done_func} g_amax_roam_end_info is null\n");
	}

	if (memcpy_s(g_first_link_bssid, ETHER_ADDR_LEN, roam_info->auc_bssid, ETHER_ADDR_LEN) != AMAX_ROAM_SUCC) {
		return AMAX_ROAM_FAIL;
	}
	amax_roam_timer_restart(AMAX_ROAM_SECONDLINK_STRAT_TIMEOUT_THRESHOLD);
	set_send_mode_param(g_first_link_band);
	amax_chr_update_first_link_done();
	/* 新路由Link Info */
	amax_send_link_info_req();
	amax_roam_send_link_info_timer_restart(AMAX_ROAM_GET_LINK_INFO_RETRANS);
	
	return AMAX_ROAM_SUCC;
}

static uint32_t amax_ml_first_link_roam_fail_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	amax_print(PRINT_ERROR, "{amax_ml_first_link_roam_fail_func} first link roam fail\n");
	amax_ml_roam_main_change_state(roam_info, INIT_STATUS);

	/*双频漫游失败处理*/
	amax_ml_roam_fail_process();
	amax_roam_timer_exit();
	amax_roam_send_link_info_timer_exit();
	return AMAX_ROAM_FBAND_DISMATCH;
}

static uint32_t amax_ml_second_link_roaming_start_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	if (roam_info == NULL) {
		return AMAX_ROAM_FAIL;
	}
	if(g_second_link_band != roam_info->ml_roam_band){
		amax_print(PRINT_ERROR, "{amax_ml_second_link_roaming_start_func} g_second_link_band check fail\n");
		roam_info->ml_roam_result = AMAX_ROAM_SBAND_DISMATCH;
		return AMAX_ROAM_SBAND_DISMATCH;
	}
	set_send_mode_param(FN_DOUBLE);
	amax_ml_roam_main_change_state(roam_info, SECOND_LINK_ROAM_ONGOING);
	amax_roam_timer_restart(AMAX_ROAM_SECONDLINK_TIMEOUT_THRESHOLD);
	amax_roam_send_link_info_timer_exit();
	amax_chr_update_second_link_start();
	return AMAX_ROAM_SUCC;
}

static uint32_t amax_ml_first_link_done_error_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	amax_print(PRINT_ERROR, "{amax_ml_first_link_done_error_func} error\n");
	/*超时未收到第二条链开始漫游的通知*/
	/*接收到的开始漫游的通知信息不对*/
	amax_ml_roam_main_change_state(roam_info, INIT_STATUS);
	/*双频漫游失败处理*/
	amax_ml_roam_fail_process();
	amax_roam_timer_exit();
	amax_roam_send_link_info_timer_exit();
	return AMAX_ROAM_SUCC;
}

static uint32_t amax_ml_second_link_roam_done_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	uint32_t ret = AMAX_ROAM_SUCC;
	if (roam_info == NULL) {
		return AMAX_ROAM_FAIL;
	}
	uint8_t fn_mode = get_main_link_band();
	if (ml_check_roam_para(roam_info)) {
		amax_ml_roam_main_fsm_action(roam_info, ROAM_FAIL_REQ, chan_info);
		roam_info->ml_roam_result = AMAX_ROAM_SBAND_DISMATCH;
		return AMAX_ROAM_SBAND_DISMATCH;
	}
	set_send_mode_param(FN_INVALID);
	amax_roam_timer_exit();
	amax_roam_send_link_info_timer_exit();
	if (memcmp(g_second_link_bssid, roam_info->auc_bssid, ETHER_ADDR_LEN) != 0) {
		amax_print(PRINT_ERROR, "{amax_ml_second_link_roam_done_func} g_second_link_bssid check fail\n");
		roam_info->ml_roam_result = AMAX_ROAM_DIFF_AP;
		amax_ml_roam_fail_process();
		return AMAX_ROAM_DIFF_AP;
	}
	amax_ml_roam_main_change_state(roam_info, INIT_STATUS);
	dps_report_amax_msg_to_userspace(NULL, 0, TRANSITION_AMAX_ROAM_SUCC);
	amax_ml_roam_clear();
	amax_print(PRINT_INFO, "{amax_ml_second_link_roam_done_func} dual_band roam success!\n");
	amax_roam_chr_stat *roam_chr_info = (amax_roam_chr_stat *)get_amax_roam_stat();
	if (roam_chr_info != NULL) {
		amax_print(PRINT_INFO, "amax dual_band roam loss pkt rx[%d], tx_2[%d], tx_5[%d]\n",
			roam_chr_info->rx_pkt_loss, roam_chr_info->tx_pkt_loss[0], roam_chr_info->tx_pkt_loss[1]);
	}
	amax_chr_update_dualband_roam_done();
	amax_dualband_roam_upload(ktime_get());
	struct net_device* main_dev = get_main_link_dev();
	if (main_dev != NULL) {
		amax_print(PRINT_INFO, "netdev_notify_peers send to router\n");
		call_netdevice_notifiers(NETDEV_NOTIFY_PEERS, main_dev);
	}
	return AMAX_ROAM_SUCC;
}

static uint32_t amax_ml_second_link_roam_fail_func(amax_roam_info_stru *roam_info, amax_chan_info_struct *chan_info)
{
	amax_print(PRINT_ERROR, "{amax_ml_second_link_roam_fail_func} second_link para check fail\n");
	set_send_mode_param(g_first_link_band);
	amax_ml_roam_main_change_state(roam_info, SECOND_LINK_ROAM_FAIL);
	amax_ml_roam_main_fsm_action(roam_info, ROAM_FAIL_REQ, chan_info);
	return AMAX_ROAM_SBAND_DISMATCH;
}

static uint32_t amax_ml_second_link_roam_fail_resume_func(amax_roam_info_stru *roam_info,
	amax_chan_info_struct *chan_info)
{
	amax_ml_roam_main_change_state(roam_info, INIT_STATUS);
	/*重新触发multilink流程*/
	amax_ml_roam_fail_process();
	amax_roam_timer_exit();
	amax_roam_send_link_info_timer_exit();
	return AMAX_ROAM_SBAND_DISMATCH;
}

int32_t amax_ml_roam_info_report(amax_dualband_roam_info_stru *dualband_roam_info, 
                                 amax_chan_info_struct *chan_info)
{
	uint32_t ret = AMAX_ROAM_SUCC;

	if (dualband_roam_info == NULL || chan_info == NULL) {
		return AMAX_ROAM_FAIL;
	}
	amax_print(PRINT_INFO, "{amax_ml_roam_info_report} ml_roam_msg_type[%u]\n", dualband_roam_info->ml_roam_msg_type);
	if (dualband_roam_info->ml_roam_msg_type == ML_ROAM_START) {
		/*上报信令类型为双频漫游开始指示*/
		if (g_roam_info.ml_roam_main_state != INIT_STATUS && g_roam_info.ml_roam_main_state != FIRST_LINK_ROAM_DONE) {
			/*双频漫游处于中间态,不做处理 */
			ret = g_roam_info.ml_roam_main_state < FIRST_LINK_ROAM_DONE ? AMAX_ROAM_FDOING : AMAX_ROAM_SDOING;
			return ret;
		}
		ret = memcpy_s(&g_roam_info.auc_bssid, ETHER_ADDR_LEN, dualband_roam_info->ml_auc_bssid, ETHER_ADDR_LEN);
		if (ret != AMAX_ROAM_SUCC) {
			amax_print(PRINT_ERROR, "{amax_ml_roam_info_report} memcpy_s err.\n");
			return AMAX_ROAM_FAIL;
		}
		g_roam_info.ml_roam_band = dualband_roam_info->ml_roam_band;
		ret = amax_ml_roam_main_fsm_action(&g_roam_info, ROAM_START_REQ, chan_info);
	} else if (dualband_roam_info->ml_roam_msg_type == ML_ROAM_INFO_REPORT) {
		/* 上报信令类型为双频漫游成功指示 */
		if (g_roam_info.ml_roam_main_state != FIRST_LINK_ROAM_ONGOING &&
			g_roam_info.ml_roam_main_state != SECOND_LINK_ROAM_ONGOING) {
			/*双频漫游未开始,不处理漫游成功指示 */
			ret = g_roam_info.ml_roam_main_state < SECOND_LINK_ROAM_ONGOING ? AMAX_ROAM_FUNDOING : AMAX_ROAM_SUNDOING;
			return ret;
		}
		ret = memcpy_s(&g_roam_info.auc_bssid, ETHER_ADDR_LEN, dualband_roam_info->ml_auc_bssid, ETHER_ADDR_LEN);
		if (ret != AMAX_ROAM_SUCC) {
			amax_print(PRINT_ERROR, "{amax_ml_roam_info_report} memcpy_s err.\n");
			return AMAX_ROAM_FAIL;
		}
		g_roam_info.ml_roam_band = dualband_roam_info->ml_roam_band;
		ret = amax_ml_roam_main_fsm_action(&g_roam_info, ROAM_SUCC_REQ, chan_info);
	}  else if (dualband_roam_info->ml_roam_msg_type == ML_ROAM_FAIL) {
		/* 上报信令类型为双频漫游失败指示 */
		amax_print(PRINT_ERROR, "{amax_ml_roam_info_report} roam fail\n");
		if (g_roam_info.ml_roam_main_state == FIRST_LINK_ROAM_ONGOING) {
			g_roam_info.ml_roam_result = AMAX_ROAM_FROAMFAIL;
		} else if (g_roam_info.ml_roam_main_state == SECOND_LINK_ROAM_ONGOING) {
			g_roam_info.ml_roam_result = AMAX_ROAM_SROAMFAIL;
		} else {
			return ret;
		}
		amax_ml_roam_fail_process();
		amax_roam_timer_exit();
		amax_roam_send_link_info_timer_exit();
	} else {
		amax_print(PRINT_ERROR, "{amax_ml_roam_info_report} info type err.\n");
		return AMAX_ROAM_INFO_TYPE_DISMATCH;
	}
	return ret;
}

ml_roam_state_enum amax_ml_get_roam_state(void)
{
	return g_roam_info.ml_roam_main_state;
}

int32_t get_dualband_send_on(void)
{
	return g_dualband_send_on;
}

void set_dualband_send_on(int32_t sendmode)
{
	g_dualband_send_on = sendmode;
}

int32_t get_first_link_band(void)
{
	return g_first_link_band;
}

void set_link_bssid(uint8_t *bssid, uint8_t linkid)
{
	uint32_t ret = 0;
	if (bssid == NULL) {
		amax_print(PRINT_ERROR, "{set_link_bssid} bssid null.\n");
		return;
	}
	if (linkid == 0) {
		ret = memcpy_s(g_first_link_bssid, ETHER_ADDR_LEN, bssid, ETHER_ADDR_LEN);
	} else if (linkid == 1) {
		ret = memcpy_s(g_second_link_bssid, ETHER_ADDR_LEN, bssid, ETHER_ADDR_LEN);
	} else {
		amax_print(PRINT_ERROR, "{set_link_bssid} linkid err[%d].\n", linkid);
	}
	if (ret) {
		amax_print(PRINT_ERROR, "{set_link_bssid} memcpy_s err.\n");
	}
}

