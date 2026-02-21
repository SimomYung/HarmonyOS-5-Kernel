/*
 * Audio Algorithm DFT Module.
 *
 * Copyright (c) 2013 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include "securec.h"
#include <asm/io.h>
#ifdef CONFIG_HISYSEVENT
#include <dfx/hiview_hisysevent.h>
#endif
#include "dsp_om.h"
#include "dsp_misc.h"
#include "algorithm_dft.h"


#if defined(CONFIG_LOG_EXCEPTION) || defined(CONFIG_HISYSEVENT)
static void convert_data_to_hex_string(char *dst_str, const uint32_t str_len,
	const uint16_t *data, const uint32_t data_len)
{
	uint32_t i;
	uint32_t msg_len = str_len / BITS_TO_PRINT_EACH_NUMBER;

	SMARTPA_DFT_ASSERT(dst_str != NULL);
	SMARTPA_DFT_ASSERT(data != NULL);

	SMARTPA_DFT_ASSERT(data_len <= msg_len);

	for (i = 0; i < msg_len; ++i)
		snprintf(dst_str + BITS_TO_PRINT_EACH_NUMBER * i, BITS_TO_PRINT_EACH_NUMBER + 1, "%04x", data[i]);
	dst_str[str_len - 1] = '\0';
}
#endif


#ifdef CONFIG_HISYSEVENT
static void smartpa_dft_hisysevent_report_process_error(const struct smartpa_dft_process_error *msg)
{
	SMARTPA_DFT_ASSERT(msg != NULL);
	enum smartpa_dft_id id= msg->dft_id;
	char str[MSG_SIZE_65_BYTES];
	int ret = -1;
	struct hiview_hisysevent *event = NULL;
	switch (id) {
	case DFT_ID_PROCESS_ERROR:
		event = hisysevent_create("AUDIO", "SMARTPA_ALGORITHM_PROC_ERROR", FAULT);
		if (event == NULL) {
			loge("smartpa hisysevent report process create failed: SMARTPA_ALGORITHM_PROC_ERROR");
			return;
		}
		break;
	default:
		return;
	}
	SMARTPA_DFT_ASSERT(event != NULL);
	hisysevent_put_integer(event, "EVENT_LEVEL", msg->err_code);
	memset_s(str, sizeof(str), 0, sizeof(str));
	convert_data_to_hex_string(str, sizeof(str),
		msg->event_module, sizeof(msg->event_module) / sizeof(msg->event_module[0]));
	hisysevent_put_string(event, "EVENT_MODULE", str);
	ret = hisysevent_write(event);
	if (ret < 0)
		loge("smartpa hisysevent report process write failed");
	hisysevent_destroy(&event);
	logi("smartpa hisysevent report success");
}

static void smartpa_dft_hisysevent_report_abnormal_gain(const struct smartpa_dft_gain_protection *msg)
{
	SMARTPA_DFT_ASSERT(msg != NULL);
	enum smartpa_dft_id id = msg->dft_id;
	char str[MSG_SIZE_65_BYTES];
	int ret = -1;
	struct hiview_hisysevent *event = NULL;
	switch (id) {
	case DFT_ID_FRES_ERROR:
		event = hisysevent_create("AUDIO", "SMARTPA_PARAMETER_SET_ERROE", FAULT);
		if (event == NULL) {
			loge("smartpa hisysevent report abnormal create failed: SMARTPA_PARAMETER_SET_ERROE");
			return;
		}
		break;
	case DFT_ID_RDC_ERROR:
		event = hisysevent_create("AUDIO", "SMARTPA_MALLOC_ERROR", FAULT);
		if (event == NULL) {
			loge("smartpa hisysevent report abnormal create failed: SMARTPA_MALLOC_ERROR");
			return;
		}
		break;
	case DFT_ID_TEMP_ERROR:
		event = hisysevent_create("AUDIO", "SMARTPA_STATE_ERROR", FAULT);
		if (event == NULL) {
			loge("smartpa hisysevent report abnormal create failed: SMARTPA_STATE_ERROR");
			return;
		}
		break;
	default:
		return;
	}
	SMARTPA_DFT_ASSERT(event != NULL);
	hisysevent_put_integer(event, "EVENT_LEVEL", msg->err_code);

	memset_s(str, sizeof(str), 0, sizeof(str));
	convert_data_to_hex_string(str, sizeof(str),
		msg->event_module, sizeof(msg->event_module) / sizeof(msg->event_module[0]));
	hisysevent_put_string(event, "EVENT_MODULE", str);

	memset_s(str, sizeof(str), 0, sizeof(str));
	convert_data_to_hex_string(str, sizeof(str),
		msg->err_pos, sizeof(msg->err_pos) / sizeof(msg->err_pos[0]));
	hisysevent_put_string(event, "ERR_POS_TAG", str);
	ret = hisysevent_write(event);
	if (ret < 0)
		loge("smartpa hisysevent report process create  hisysevent write failed");
	hisysevent_destroy(&event);
	logi("smartpa hisysevent report success");
}
#endif

void hifi_om_work_smartpa_dft_report(const enum smartpa_dft_id *data)
{
	if (data == NULL)
		return;

	switch (*data) {
	case DFT_ID_PROCESS_ERROR:
#ifdef CONFIG_HISYSEVENT
		smartpa_dft_hisysevent_report_process_error((const struct smartpa_dft_process_error*)data);
#endif
		break;
	case DFT_ID_FRES_ERROR:
	case DFT_ID_RDC_ERROR:
	case DFT_ID_TEMP_ERROR:
#ifdef CONFIG_HISYSEVENT
		smartpa_dft_hisysevent_report_abnormal_gain((const struct smartpa_dft_gain_protection*)data);
#endif
		break;
	default:
		break;
	}
}

void hifi_om_work_audio_db(const enum smartpa_dft_id *data)
{
	if (data == NULL)
		return;

	switch (*data) {
	case DFT_ID_AUDIO_DB:
		break;
	default:
		break;
	}
}


