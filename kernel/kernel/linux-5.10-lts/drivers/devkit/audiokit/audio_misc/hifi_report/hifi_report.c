/*
 * hifi_report.c
 *
 * hifi_report driver
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "../audio_misc.h"
#include <securec.h>
#include <linux/ktime.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/log/hwlog_kernel.h>
#include <linux/workqueue.h>
#include <linux/slab.h>
#include "hifi_report.h"
#include "drv_mailbox_msg.h"

#define unused_param(x)       (void)(x)
#define MOD_NAME              "hifi_report"
#define HWLOG_TAG             hifi_report
#define MSG_LEN               160
#define NAME_LEN              32

HWLOG_REGIST();

static bool g_ultra_scene = false;
static uint64_t g_ultra_start_time = 0;

struct hifi_report_rcv_msg {
	uint16_t msg_type;
	uint16_t msg_size;
	char data[MSG_LEN];
};

struct hifi_report_priv {
    struct work_struct report_work;
	char msg[MSG_LEN];
	char uevent_report_name[NAME_LEN];
};

static struct hifi_report_priv *g_hifi_priv;

enum msg_type {
	MSG_TYPE_BEGIN = 0,
	MIC_IN_SILENT_MSG,
	ULTRA_MISTOUCH_START_MSG,
	ULTRA_MISTOUCH_STOP_MSG,
	ULTRA_MISTOUCH_SWITCH_MSG,
	COMBINE_NOISE_UEVENT_MSG,
	ULTRA_PROXIMITY_START_MSG,
	ULTRA_PROXIMITY_STOP_MSG,
	MSG_TYPE_END,
};

enum uevent_type {
	ULTRA_UEVENT = 0,
	COMBINE_NOISE_UEVENT,
    UEVENT_TYPE_END,
};

static const char *g_uevent_name[] = {
	"hifi_report",
	"NAME=send_nn_state"
};

static int hifi_report_parse_msg_type(uint16_t msg_type, struct hifi_report_priv *priv)
{
	int ret = 0;
	uint64_t ultra_stop_time = 0;

	switch (msg_type) {
	case ULTRA_MISTOUCH_START_MSG:
		g_ultra_scene = true;
		g_ultra_start_time = (uint64_t)ktime_to_ms(ktime_get_real());
		ret = strncpy_s(priv->uevent_report_name, sizeof(priv->uevent_report_name),
			g_uevent_name[ULTRA_UEVENT], strlen(g_uevent_name[ULTRA_UEVENT]));
		break;
	case ULTRA_MISTOUCH_STOP_MSG:
		if (g_ultra_scene) {
			ultra_stop_time = (uint64_t)ktime_to_ms(ktime_get_real());
			HWDUBAI_LOGE("DUBAI_TAG_ULTRASONIC_POCKET", "time=%d", (int)(ultra_stop_time - g_ultra_start_time));
			g_ultra_scene = false;
		}
		g_ultra_start_time = 0;
		ret = strncpy_s(priv->uevent_report_name, sizeof(priv->uevent_report_name),
			g_uevent_name[ULTRA_UEVENT], strlen(g_uevent_name[ULTRA_UEVENT]));
		break;
	case ULTRA_PROXIMITY_START_MSG:
		ret = strncpy_s(priv->uevent_report_name, sizeof(priv->uevent_report_name),
			g_uevent_name[ULTRA_UEVENT], strlen(g_uevent_name[ULTRA_UEVENT]));
		break;
	case ULTRA_PROXIMITY_STOP_MSG:
		ret = strncpy_s(priv->uevent_report_name, sizeof(priv->uevent_report_name),
			g_uevent_name[ULTRA_UEVENT], strlen(g_uevent_name[ULTRA_UEVENT]));
		break;
	case COMBINE_NOISE_UEVENT_MSG:
		ret = strncpy_s(priv->uevent_report_name, sizeof(priv->uevent_report_name),
			g_uevent_name[COMBINE_NOISE_UEVENT], strlen(g_uevent_name[COMBINE_NOISE_UEVENT]));
		break;
	default:
		break;
	}

	if (ret != EOK) {
		hwlog_err("%s: strncpy_s fail", __func__);
		return -EFAULT;
	}
	return ret;
}

static void hifi_mailbox_recv_proc(const void *usr_para,
	struct mb_queue *mail_handle, uint32_t mail_len)
{
	unused_param(usr_para);
	struct hifi_report_rcv_msg rcv_msg = { 0, 0, "" };
	int ret;
	uint32_t mail_size = sizeof(rcv_msg);

	if (!mail_handle || (mail_len != sizeof(rcv_msg))) {
		hwlog_err("%s mailbox msg proc fail, param is NULL", __func__);
		return;
	}

	ret = mailbox_read_msg_data(mail_handle, (char*)&rcv_msg, &mail_size);
	if (ret != 0 || (mail_size != sizeof(rcv_msg))) {
		hwlog_err("%s mailbox read error, result:%d, mail size:%u",
			__func__, ret, mail_len);
		return;
	}

	hwlog_info("%s rcv_msg.msg_type=%u, report to hal process", __func__, rcv_msg.msg_type);

	if (!g_hifi_priv) {
		hwlog_info("%s g_hifi_priv is null", __func__);
		return;
	}

	ret = hifi_report_parse_msg_type(rcv_msg.msg_type, g_hifi_priv);
	if (ret != EOK)
		return;

	ret = strncpy_s(g_hifi_priv->msg, sizeof(g_hifi_priv->msg), rcv_msg.data, sizeof(rcv_msg.data));
	if (ret != EOK)
		return;

	schedule_work(&g_hifi_priv->report_work);
	return;
}

static void report_uevent_work(struct work_struct *work)
{
	struct hifi_report_priv *hifi_priv = container_of(work, struct hifi_report_priv, report_work);

	audio_misc_report_uevent(hifi_priv->uevent_report_name, hifi_priv->msg);
}

static void hifi_report_remove(void *priv)
{
	struct hifi_report_priv *hifi_priv = NULL;

	if (!priv)
		return;
	hifi_priv = (struct hifi_report_priv *)priv;
	kfree(hifi_priv);
	hifi_priv = NULL;
	g_hifi_priv = NULL;
}

static const struct audio_misc_module_ops hifi_report_ops = {
	.remove = hifi_report_remove,
};

int hifi_report_probe(struct device *dev, struct device_node *np)
{
	unused_param(dev);
	unused_param(np);
	int ret;
	struct hifi_report_priv *hifi_priv = NULL;

	hwlog_info("%s: in", __func__);
	hifi_priv = kzalloc(sizeof(*hifi_priv), GFP_KERNEL);
	if (!hifi_priv)
		return -ENOMEM;

	INIT_WORK(&hifi_priv->report_work, report_uevent_work);

	g_hifi_priv = hifi_priv;
	ret = mailbox_reg_msg_cb(MAILBOX_MAILCODE_HIFI_TO_ACPU_AUDIO_MISC,
		hifi_mailbox_recv_proc, NULL);
	if (ret) {
		hwlog_err("%s register receive func error, ret:%d, mailcode:0x%x", __func__,
			ret, MAILBOX_MAILCODE_HIFI_TO_ACPU_AUDIO_MISC);
		ret = -ENOMEM;
		goto hifi_err;
	}

	ret = audio_misc_module_constructor(MOD_NAME, hifi_priv, -1, &hifi_report_ops);
	if (ret) {
		hwlog_err("%s hifi_report construct fail", __func__);
		ret = -ENOMEM;
		goto hifi_err;
	}

	hwlog_info("%s hifi_report probe finished", __func__);
	return EOK;

hifi_err:
	kfree(hifi_priv);
	hifi_priv = NULL;
	g_hifi_priv = NULL;
	return ret;
}
