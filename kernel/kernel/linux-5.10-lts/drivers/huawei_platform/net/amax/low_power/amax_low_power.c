/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: This module is to support amax low power management.
 * Create: 2024-12
 */

#include "amax_low_power.h"

static amax_sleep_control_info g_sleep_ctl_info = { 0 };

static bool is_assist_link_has_pkt(void)
{
	if (g_sleep_ctl_info.tx_assist_pkt_flag || g_sleep_ctl_info.rx_assist_pkt_flag)
		return true;

	return false;
}

void set_tx_assit_pkt_flag(uint8_t set_val)
{
	g_sleep_ctl_info.tx_assist_pkt_flag = set_val;
}

void set_rx_assit_pkt_flag(uint8_t set_val)
{
	g_sleep_ctl_info.rx_assist_pkt_flag = set_val;
}

static uint8_t get_amax_sleep_state(void)
{
	return g_sleep_ctl_info.sleep_state;
}

static void set_amax_sleep_state(uint8_t set_val)
{
	g_sleep_ctl_info.sleep_state = set_val;
}

static uint8_t get_router_wake_up_state(void)
{
	return g_sleep_ctl_info.router_wake_up_state;
}

static void set_router_wake_up_state(uint8_t set_val)
{
	g_sleep_ctl_info.router_wake_up_state = set_val;
}

bool is_router_support_low_power(void)
{
	return g_sleep_ctl_info.is_router_support_low_power;
}

void set_router_support_low_power(bool set_val)
{
	g_sleep_ctl_info.is_router_support_low_power = set_val;
}

void amax_low_power_process(void)
{
	do {
		/* 若当前已经处于休眠状态，或者路由器指示不休眠，则本次不休眠 */
		if (get_amax_sleep_state() == 1 || get_router_wake_up_state() == 1)
			break;
		/* 辅链有收发包，不休眠 */
		if (is_assist_link_has_pkt())
			break;
		amax_send_low_power_frame(AMAX_LOW_POWER_SLEEP, DPS_OPERATION_AMAX_LOW_POWER_REQ);
	} while (0);

	/* 定时器中清空该周期内的辅链统计以及路由器指示 */
	set_router_wake_up_state(0);
	set_tx_assit_pkt_flag(0);
	set_rx_assit_pkt_flag(0);
}

static void build_low_power_tlv(uint8_t *buf, uint8_t is_sleep)
{
	uint16_t *total_len = NULL;
	dps_tlv *first_tlv = NULL;

	if (unlikely(buf == NULL))
		return;

	total_len = (uint16_t *)buf;
	*total_len = htons(SINGLE_TLV_BYTE);

	first_tlv = (dps_tlv *)(total_len + 1);
	first_tlv->type = LOW_POWER_TLV_TYPE;
	first_tlv->len = 1;
	first_tlv->value[0] = is_sleep;
}

void amax_send_low_power_frame(uint8_t is_sleep, dps_ops_type type)
{
	int32_t ret;
	uint32_t len;
	uint8_t *buf = NULL;
	uint32_t offset = 0;

	if (type != DPS_OPERATION_AMAX_LOW_POWER_REQ && type != DPS_OPERATION_AMAX_LOW_POWER_RSP)
		return;

	len = COMMON_PART_LEN + sizeof(dps_head) + TOTAL_LEN_BYTE + SINGLE_TLV_BYTE;
	buf = (uint8_t *)kmalloc(len, GFP_KERNEL);
	if (buf == NULL) {
		printk("[DPS] low power frame malloc failed\n");
		return;
	}
	(void)memset_s(buf, len, 0, len);

	ret = dps_build_common_part(buf, len);
	if (ret != 0) {
		printk("[DPS] common part build failed\n");
		kfree(buf);
		buf = NULL;
		return;
	}
	offset += sizeof(dps_common_part);

	dps_build_data_header(buf + offset, type, VER_11);
	offset += sizeof(dps_head);

	build_low_power_tlv(buf + offset, is_sleep);

	dps_alloc_and_send_frame((dps_mgmt_msg *)buf, len);

	printk("[DPS] low power frame send successfully. sleep:%hhu, type:%u\n", is_sleep, type);

	kfree(buf);
	buf = NULL;
}

void report_low_power_to_fwk(uint8_t is_sleep)
{
	uint32_t len;
	uint8_t *buf = NULL;
	uint32_t offset = 0;

	len = sizeof(dps_head) + TOTAL_LEN_BYTE + SINGLE_TLV_BYTE;
	buf = (uint8_t *)kmalloc(len, GFP_KERNEL);
	if (buf == NULL) {
		printk("[DPS] low power malloc failed\n");
		return;
	}
	(void)memset_s(buf, len, 0, len);

	dps_build_data_header(buf, DPS_OPERATION_AMAX_LOW_POWER_REQ, VER_11);
	offset += sizeof(dps_head);

	build_low_power_tlv(buf + offset, is_sleep);

	devicepipe_rx_data_report(buf, len);

	printk("[DPS] indicate fwk to set lp_long_dtim cmd to drvier. is_sleep:%hhu\n", is_sleep);

	kfree(buf);
	buf = NULL;
}

void process_low_power_req(uint8_t *buf, int32_t len)
{
	dps_head *head = NULL;
	uint16_t *len_pos = NULL;
	uint16_t total_len = 0;
	dps_tlv *first_tlv = NULL;

	if (unlikely(buf == NULL || len != sizeof(dps_head) + TOTAL_LEN_BYTE + SINGLE_TLV_BYTE)) {
		printk("[DPS] buf is null or len %d is invalid\n", len);
		return;
	}

	head = (dps_head *)buf;
	len_pos = (uint16_t *)(head + 1);
	total_len = ntohs(*len_pos);
	if (total_len != SINGLE_TLV_BYTE) {
		printk("[DPS] total_len %hu is invalid\n", total_len);
		return;
	}

	first_tlv = (dps_tlv *)(len_pos + 1);
	if (first_tlv->type != LOW_POWER_TLV_TYPE) {
		printk("[DPS] tlv type %hhu is invalid\n", first_tlv->type);
		return;
	}

	printk("[DPS] receive low power req. type[%hhu] value[%hhu]\n", first_tlv->type, first_tlv->value[0]);

	if (first_tlv->value[0] == AMAX_LOW_POWER_WAKE_UP) {
		report_low_power_to_fwk(AMAX_LOW_POWER_WAKE_UP);
		set_router_wake_up_state(1);
		set_amax_sleep_state(0);
		printk("[DPS] receive router weak up req. set beacon interval 100ms\n");
		/* 回复对端，指示req已收到并成功解析 */
		amax_send_low_power_frame(AMAX_LOW_POWER_WAKE_UP, DPS_OPERATION_AMAX_LOW_POWER_RSP);
	}
}

void process_low_power_rsp(uint8_t *buf, int32_t len)
{
	dps_head *head = NULL;
	uint16_t *len_pos = NULL;
	uint16_t total_len = 0;
	dps_tlv *first_tlv = NULL;

	if (unlikely(buf == NULL || len != sizeof(dps_head) + TOTAL_LEN_BYTE + SINGLE_TLV_BYTE)) {
		printk("[DPS] buf is null or len %d is invalid\n", len);
		return;
	}

	head = (dps_head *)buf;
	len_pos = (uint16_t *)(head + 1);
	total_len = ntohs(*len_pos);
	if (total_len != SINGLE_TLV_BYTE) {
		printk("[DPS] total_len %hu is invalid\n", total_len);
		return;
	}

	first_tlv = (dps_tlv *)(len_pos + 1);
	if (first_tlv->type != LOW_POWER_TLV_TYPE) {
		printk("[DPS] tlv type %hhu is invalid\n", first_tlv->type);
		return;
	}

	printk("[DPS] receive low power rsp. type[%hhu] value[%hhu]\n", first_tlv->type, first_tlv->value[0]);

	if (first_tlv->value[0] == AMAX_LOW_POWER_WAKE_UP) {
		printk("[DPS] router reject switch to low power state\n");
		set_amax_sleep_state(0);
		set_router_wake_up_state(1);
	} else if (first_tlv->value[0] == AMAX_LOW_POWER_SLEEP) {
		if (!is_assist_link_has_pkt()) {
			report_low_power_to_fwk(AMAX_LOW_POWER_SLEEP);
			set_amax_sleep_state(1);
		}
	} else {
		printk("[DPS] value[%hhu] is invalid\n", first_tlv->value[0]);
	}
	return;
}

void amax_low_power_assist_link_pkt_process(void)
{
	if (!is_router_support_low_power() || get_amax_sleep_state() == 0)
		return;

	report_low_power_to_fwk(AMAX_LOW_POWER_WAKE_UP);
	set_amax_sleep_state(0);
	amax_send_low_power_frame(AMAX_LOW_POWER_WAKE_UP, DPS_OPERATION_AMAX_LOW_POWER_REQ);
}

void reset_low_power_control_info(void)
{
	(void)memset_s(&g_sleep_ctl_info, sizeof(g_sleep_ctl_info), 0, sizeof(g_sleep_ctl_info));
}

void low_power_set_proc(void)
{
	(void)memset_s(&g_sleep_ctl_info, sizeof(g_sleep_ctl_info), 0, sizeof(g_sleep_ctl_info));
	g_sleep_ctl_info.is_router_support_low_power = 1;
	printk("[DPS] router support amax low power set to 1\n");
}
