// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_swctrl.c
 *
 * source file for switch control
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "lcd_kit_hybrid_swctrl.h"
#include "lcd_kit_hybrid_core.h"

#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <platform_include/smart/linux/iomcu_config.h>
#include "securec.h"
#include "lcd_kit_adapt.h"
#include "ext_sensorhub_api.h"

#define SWITCH_TIMEOUT 500
#define SEND_STATE_RETRY_CNT 3
/* now contains 2 sw ctrl */
#define CTRL_LIST_CNT 2

struct hybrid_resp {
	u8 resp_cmd;
	u8 resp_data;
	bool refreshed;

	u8 callback_cmd;
	u8 callback_data;
};

/* details for hybrid switch controller, contains a switch interface */
struct hybrid_sw_ctrl {
	const char *name;
	u8 ap_ctrl_state;

	struct work_struct work;
	switch_work_handle work_handle;
	struct hybrid_sw_ops sw_ops;
	/* lock for request sync */
	struct mutex request_lock;

	enum ext_channel_id channel_id;
	u8 ctrl_sid;
	u8 ctrl_cid;
	/* aim state cmd */
	u8 aim_state_cmd;
	u8 aim_state_resp;
	/* power control cmd */
	u8 power_ctrl_cmd;
	u8 power_ctrl_resp;
	/* control switch cmd */
	u8 ctrl_sw_cmd;
	u8 ctrl_sw_resp;

	/* for msg resp */
	struct hybrid_resp resp_data;
};

static DEFINE_MUTEX(ctrl_list_lock);
static struct hybrid_sw_ctrl *ctrl_list[CTRL_LIST_CNT];

static char *str_concat(const char *prefix, const char *suffix)
{
	char *out = NULL;
	u32 len;
	int ret;

	if (!prefix || !suffix)
		return NULL;

	len = strlen(prefix) + strlen(suffix) + 1;
	out = kzalloc(len, GFP_KERNEL);
	if (!out)
		return NULL;

	ret = snprintf_s(out, len, len, "%s%s", prefix, suffix);
	if (ret < 0) {
		kfree(out);
		out = NULL;
		return NULL;
	}

	return out;
}

static int send_hybrid_cmd(struct hybrid_sw_ctrl *sw_ctrl, u8 subcmd, u8 state, u8 ack)
{
	int i;
	int ret = 0;
	int try_times = 0;
	struct command cmd = {0};
	unsigned char buffer[] = { subcmd, state };

	if (!sw_ctrl)
		return -EINVAL;

	LCD_KIT_INFO("send %s cmd:%d state: %d!\n", sw_ctrl->name, subcmd, state);
	cmd.send_buffer = buffer;
	cmd.send_buffer_len = sizeof(buffer);
	cmd.service_id = sw_ctrl->ctrl_sid;
	cmd.command_id = sw_ctrl->ctrl_cid;

	/* need set to false when commu setup */
	sw_ctrl->resp_data.refreshed = false;
	sw_ctrl->resp_data.resp_cmd = ack;
	sw_ctrl->resp_data.resp_data = state;
	for (i = 0; i < SEND_STATE_RETRY_CNT; ++i) {
		ret = send_command(sw_ctrl->channel_id, &cmd, false, NULL);
		LCD_KIT_INFO("send %s command state:%d ret:%d i:%d\n", sw_ctrl->name,
			state, ret, i);
		if (ret >= 0)
			break;

		mdelay(100);
	}
	if (i >= SEND_STATE_RETRY_CNT) {
		LCD_KIT_INFO("send %s state err\n", sw_ctrl->name);
		return ret;
	}
	/* wait and check */
	while (!sw_ctrl->resp_data.refreshed) {
		/* sleep for 1000 ~ 1001 us */
		usleep_range(1000, 1001);
		if (try_times++ > SWITCH_TIMEOUT) {
			LCD_KIT_ERR("wait for %s sw timeout!\n", sw_ctrl->name);
			return -ETIME;
		}
	}

	return ret;
}

static int send_hybrid_state(struct hybrid_sw_ops *ops, u8 state)
{
	int ret;
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	/* get switch controller details by switch interface */
	if (!ops)
		return -EINVAL;
	sw_ctrl = container_of(ops, struct hybrid_sw_ctrl, sw_ops);
	if (!sw_ctrl)
		return -EINVAL;

	hybrid_set_display_msg(state);
	mutex_lock(&sw_ctrl->request_lock);
	ret = send_hybrid_cmd(sw_ctrl, sw_ctrl->aim_state_cmd, state, sw_ctrl->aim_state_resp);
	mutex_unlock(&sw_ctrl->request_lock);
	return ret;
}

static int hybrid_power_ctrl(struct hybrid_sw_ops *ops, u8 state)
{
	int ret;
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	/* get switch controller details by switch interface */
	if (!ops)
		return -EINVAL;
	sw_ctrl = container_of(ops, struct hybrid_sw_ctrl, sw_ops);
	if (!sw_ctrl)
		return -EINVAL;

	mutex_lock(&sw_ctrl->request_lock);
	ret = send_hybrid_cmd(sw_ctrl, sw_ctrl->power_ctrl_cmd, state, sw_ctrl->power_ctrl_resp);
	mutex_unlock(&sw_ctrl->request_lock);
	return ret;
}

static int hybrid_request_control(struct hybrid_sw_ops *ops, u8 state)
{
	int ret;
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	/* get switch controller details by switch interface */
	if (!ops)
		return -EINVAL;
	sw_ctrl = container_of(ops, struct hybrid_sw_ctrl, sw_ops);
	if (!sw_ctrl)
		return -EINVAL;

	mutex_lock(&sw_ctrl->request_lock);
	ret = send_hybrid_cmd(sw_ctrl, sw_ctrl->ctrl_sw_cmd, state, sw_ctrl->ctrl_sw_resp);
	if (ret >= 0)
		sw_ctrl->ap_ctrl_state = !!state;
	mutex_unlock(&sw_ctrl->request_lock);
	return ret;
}

/* AP control state */
static bool sw_status_check(struct hybrid_sw_ops *ops)
{
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	/* get switch controller details by switch interface */
	if (!ops)
		return false;

	sw_ctrl = container_of(ops, struct hybrid_sw_ctrl, sw_ops);
	if (!sw_ctrl)
		return false;
	/* wait for other cmd finished */
	mutex_lock(&sw_ctrl->request_lock);
	mutex_unlock(&sw_ctrl->request_lock);
	return sw_ctrl->ap_ctrl_state;
}

/* MCU control state, read from share memory */
static bool sw_status_check_mcu(struct hybrid_sw_ops *ops)
{
	struct hybrid_sw_ctrl *sw_ctrl = NULL;
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));
	bool mcu_ctrl = !sw_status_check(ops);

	/* get switch controller details by switch interface */
	if (!ops || !quick_on_config)
		return mcu_ctrl;

	sw_ctrl = container_of(ops, struct hybrid_sw_ctrl, sw_ops);
	if (!sw_ctrl)
		return mcu_ctrl;

	/* wait for other cmd finished */
	mutex_lock(&sw_ctrl->request_lock);
	mutex_unlock(&sw_ctrl->request_lock);

	return quick_on_config->lcd_ctrl_state;
}

static int config_init_aimstate(struct hybrid_sw_ctrl *sw_ctrl, struct device_node *np)
{
	if (!sw_ctrl || !np)
		return -EINVAL;
	/* get aim state sub cmdid */
	if (of_property_read_u8(np, "aim_state_cmd", &sw_ctrl->aim_state_cmd)) {
		LCD_KIT_ERR("of_property_read: aim_state_cmd not find\n");
		return -EFAULT;
	}

	/* get aim state resp id */
	if (of_property_read_u8(np, "aim_state_resp", &sw_ctrl->aim_state_resp)) {
		LCD_KIT_ERR("of_property_read: aim_state_resp not find\n");
		return -EFAULT;
	}

	return 0;
}

static int config_init_powerctrl(struct hybrid_sw_ctrl *sw_ctrl, struct device_node *np)
{
	if (!sw_ctrl || !np)
		return -EINVAL;
	/* get power ctrl sub cmdid */
	if (of_property_read_u8(np, "power_ctrl_cmd", &sw_ctrl->power_ctrl_cmd)) {
		LCD_KIT_ERR("of_property_read: power_ctrl_cmd not find\n");
		return -EFAULT;
	}

	/* get power ctrl resp id */
	if (of_property_read_u8(np, "power_ctrl_resp", &sw_ctrl->power_ctrl_resp)) {
		LCD_KIT_ERR("of_property_read: power_ctrl_resp not find\n");
		return -EFAULT;
	}

	return 0;
}

static int config_init_ctrlswitch(struct hybrid_sw_ctrl *sw_ctrl, struct device_node *np)
{
	if (!sw_ctrl || !np)
		return -EINVAL;
	/* get control switch sub cmdid */
	if (of_property_read_u8(np, "ctrl_sw_cmd", &sw_ctrl->ctrl_sw_cmd)) {
		LCD_KIT_ERR("of_property_read: ctrl_sw_cmd not find\n");
		return -EFAULT;
	}

	/* get control switch resp id */
	if (of_property_read_u8(np, "ctrl_sw_resp", &sw_ctrl->ctrl_sw_resp)) {
		LCD_KIT_ERR("of_property_read: ctrl_sw_resp not find\n");
		return -EFAULT;
	}

	return 0;
}

static int hybrid_config_init(struct hybrid_sw_ctrl *sw_ctrl)
{
	int len = 0;
	const char *status = NULL;
	struct device_node *np = NULL;
	char *compat = NULL;
	int ret = -ENODEV;

	if (!sw_ctrl)
		return -EINVAL;
	/* get compat node from dts tree */
	compat = str_concat(sw_ctrl->name, ",control");
	if (!compat)
		return -EFAULT;
	np = of_find_compatible_node(NULL, NULL, compat);
	if (!np) {
		LCD_KIT_ERR("node not found\n");
		goto out;
	}

	status = of_get_property(np, "status", &len);
	if (!status) {
		LCD_KIT_ERR("can't find property status\n");
		return ret;
	}
	if (strstr(status, "ok")) {
		LCD_KIT_INFO("control node enabled!\n");
	} else {
		LCD_KIT_INFO("control node disabled!\n");
		return ret;
	}

	/* get state commu service id */
	if (of_property_read_u8(np, "ctrl_sid", &sw_ctrl->ctrl_sid)) {
		LCD_KIT_ERR("of_property_read: ctrl_sid not find\n");
		goto out;
	}

	/* get state commu command id */
	if (of_property_read_u8(np, "ctrl_cid", &sw_ctrl->ctrl_cid)) {
		LCD_KIT_ERR("of_property_read: ctrl_cid not find\n");
		goto out;
	}

	if (config_init_aimstate(sw_ctrl, np))
		goto out;

	if (config_init_powerctrl(sw_ctrl, np))
		goto out;

	if (config_init_ctrlswitch(sw_ctrl, np))
		goto out;

	ret = 0;
	LCD_KIT_INFO("ctrl_sid:%02x, ctrl_cid:%02x\n", sw_ctrl->ctrl_sid, sw_ctrl->ctrl_cid);
out:
	kfree(compat);
	compat = NULL;
	return ret;
}

static void switch_work(struct work_struct *work)
{
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	if (!work)
		return;

	sw_ctrl = container_of(work, struct hybrid_sw_ctrl, work);
	if (!sw_ctrl)
		return;

	LCD_KIT_INFO("%s receive callback! cmd:%d, data:%d\n", sw_ctrl->name,
		sw_ctrl->resp_data.callback_cmd, sw_ctrl->resp_data.callback_data);

	/* call the callback handle */
	if (sw_ctrl->work_handle)
		sw_ctrl->work_handle(sw_ctrl->resp_data.callback_cmd, sw_ctrl->resp_data.callback_data);
}

static int hybrid_recv_mcu_cb(unsigned char service_id,
	unsigned char command_id, unsigned char *data, int data_len)
{
	int i;
	/* match sw ctrl */
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	mutex_lock(&ctrl_list_lock);
	for (i = 0; i < CTRL_LIST_CNT; ++i) {
		if (!ctrl_list[i])
			continue;
		if (ctrl_list[i]->ctrl_sid == service_id && ctrl_list[i]->ctrl_cid == command_id) {
			sw_ctrl = ctrl_list[i];
			break;
		}
	}
	mutex_unlock(&ctrl_list_lock);
	if (!sw_ctrl)
		return 0;

	/* need more than 2 data */
	if (!data || data_len < 2)
		return 0;

	if (sw_ctrl->resp_data.resp_cmd == data[0] && sw_ctrl->resp_data.resp_data == data[1]) {
		sw_ctrl->resp_data.refreshed = true;
		return 0;
	}
	sw_ctrl->resp_data.callback_cmd = data[0];
	sw_ctrl->resp_data.callback_data = data[1];
	schedule_work(&sw_ctrl->work);

	return 0;
}

static void register_cmd_callback(struct hybrid_sw_ctrl *sw_ctrl)
{
	if (!sw_ctrl)
		return;
	struct sid_cid cmds[] = {
		{ sw_ctrl->ctrl_sid, sw_ctrl->ctrl_cid },
	};

	struct subscribe_cmds subcmds = {
		cmds, 1,
	};

	register_data_callback(sw_ctrl->channel_id, &subcmds, hybrid_recv_mcu_cb);
}

struct hybrid_sw_ops *hybrid_swctrl_init(switch_work_handle handle,
	const char *name, int channel_id)
{
	int i;
	int ret;
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	if (!handle || !name)
		return NULL;

	sw_ctrl = kzalloc(sizeof(*sw_ctrl), GFP_KERNEL);
	if (!sw_ctrl)
		return NULL;
	/* initialize the switch controller details */
	sw_ctrl->name = name;
	sw_ctrl->work_handle = handle;
	sw_ctrl->channel_id = channel_id;
	/* now default control at AP */
	sw_ctrl->ap_ctrl_state = 1;

	sw_ctrl->sw_ops.request_control = hybrid_request_control;
	sw_ctrl->sw_ops.power_control = hybrid_power_ctrl;
	sw_ctrl->sw_ops.sw_status_check = sw_status_check;
	sw_ctrl->sw_ops.sw_status_check_mcu = sw_status_check_mcu;
	sw_ctrl->sw_ops.send_hybrid_state = send_hybrid_state;
	memset_s(&sw_ctrl->resp_data, sizeof(sw_ctrl->resp_data), 0, sizeof(sw_ctrl->resp_data));

	mutex_init(&sw_ctrl->request_lock);
	ret = hybrid_config_init(sw_ctrl);
	if (ret < 0) {
		kfree(sw_ctrl);
		sw_ctrl = NULL;
		return NULL;
	}

	mutex_lock(&ctrl_list_lock);
	for (i = 0; i < CTRL_LIST_CNT; ++i) {
		if (!ctrl_list[i]) {
			ctrl_list[i] = sw_ctrl;
			break;
		}
	}
	mutex_unlock(&ctrl_list_lock);

	INIT_WORK(&sw_ctrl->work, switch_work);
	/* register data callback */
	register_cmd_callback(sw_ctrl);

	/* return the interface object for caller */
	return &sw_ctrl->sw_ops;
}

void hybrid_swctrl_release(struct hybrid_sw_ops *ops)
{
	struct hybrid_sw_ctrl *sw_ctrl = NULL;

	/* get switch controller details by switch interface */
	if (!ops)
		return;

	sw_ctrl = container_of(ops, struct hybrid_sw_ctrl, sw_ops);
	/* release the switch controller details */
	kfree(sw_ctrl);
	sw_ctrl = NULL;
}
