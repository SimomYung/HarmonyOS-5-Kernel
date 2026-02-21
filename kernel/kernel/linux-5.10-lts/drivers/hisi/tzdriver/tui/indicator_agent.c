/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: indicator agent for indicator light display and interact
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
#include "indicator_agent.h"
#include <securec.h>
#include "teek_client_constants.h"
#include "teek_client_type.h"
#include "teek_client_api.h"
#include "teek_ns_client.h"
#include "agent.h"
#include "tc_ns_log.h"
#include "mailbox_mempool.h"
#include "smc_smp.h"
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/wait.h>
#include <linux/jiffies.h>

#define INDICATOR_TIMEOUT 200
#define INDICATOR_HIGH_VALUES 32
#define SCREEN_PIXEL_X_MAX 10000
#define SCREEN_PIXEL_Y_MAX 20000
#define SCREEN_PIXEL_X_MIN 16
#define SCREEN_PIXEL_Y_MIN 16
#define INDICATOR_MAX_SIDE 32
#define INDICATOR_HALF_SIDE 16
#define INDICATOR_AGENT_SHM_SIZE 64

static DEFINE_MUTEX(g_indicator_agent_lock);

static wait_queue_head_t g_indicator_msg_wq;
static int32_t g_indicator_msg_flag = 0;
static int32_t g_indicator_msg_result = 0;

static struct indicator_ctrl g_indicator_ctrl = { 0 };
static notify_dpu g_init_func = NULL;

static struct indicator_panel_info g_panel_info = { 0 };
static atomic_t g_indicator_state = ATOMIC_INIT(INDICATOR_STATE_UNUSED);

static struct workqueue_struct *g_cmd_indicator_wq = NULL;
static char *g_indicator_dss_power_shm = NULL;

static void indicator_poweron_work_func(struct work_struct *work);
static void indicator_poweroff_work_func(struct work_struct *work);
static void indicator_poweron_abnormal_work_func(struct work_struct *work);
static void indicator_poweroff_abnormal_work_func(struct work_struct *work);
static DECLARE_DELAYED_WORK(indicator_poweron_work, indicator_poweron_work_func);
static DECLARE_DELAYED_WORK(indicator_poweroff_work, indicator_poweroff_work_func);
static DECLARE_DELAYED_WORK(indicator_poweron_abnormal_work, indicator_poweron_abnormal_work_func);
static DECLARE_DELAYED_WORK(indicator_poweroff_abnormal_work, indicator_poweroff_abnormal_work_func);

static int indicator_wait_msg(void)
{
	int ret = wait_event_interruptible_timeout(g_indicator_msg_wq, g_indicator_msg_flag,
		msecs_to_jiffies(INDICATOR_TIMEOUT));
	if (ret > 0)
		return 0;

	if (ret == 0)
		tloge("indicator layer add timeout\n");

	if (ret < 0)
		tloge("interrupted by signal %d\n", ret);

	return -1;
}

static int indicator_layer_rect_calc(struct indicator_layer_rect *rect)
{
	if (g_indicator_ctrl.status.x < SCREEN_PIXEL_X_MIN ||
		g_indicator_ctrl.status.y < SCREEN_PIXEL_Y_MIN ||
		g_indicator_ctrl.status.x > SCREEN_PIXEL_X_MAX ||
		g_indicator_ctrl.status.y > SCREEN_PIXEL_Y_MAX) {
		tloge("Incorrect indicator x or y parameters\n");
		return -1;
	}

	if (g_panel_info.xres < g_indicator_ctrl.status.x + INDICATOR_HALF_SIDE ||
		g_panel_info.yres < g_indicator_ctrl.status.y + INDICATOR_HALF_SIDE) {
		tloge("Invalid xyres, the value is exceeded\n");
		return -1;
	}

	rect->x = g_indicator_ctrl.status.x - INDICATOR_HALF_SIDE;
	rect->y = g_indicator_ctrl.status.y - INDICATOR_HALF_SIDE;
	rect->w = INDICATOR_MAX_SIDE;
	rect->h = INDICATOR_MAX_SIDE;

	return 0;
}

static int add_indicator_layer(void)
{
	int ret = 0;
	struct indicator_layer_rect rect = { 0 };

	if (atomic_read(&g_indicator_state) != INDICATOR_STATE_UNUSED) {
		tloge("indicator has been turned on\n");
		return 0;
	}

	if (indicator_layer_rect_calc(&rect) != 0) {
		tloge("indicator layer rect calc failed\n");
		return -1;
	}

	ret = g_init_func(&rect, INDICATOR_SCENE);
	if (ret != 0) {
		tloge("notify dpu indicator request failed\n");
		atomic_set(&g_indicator_state, INDICATOR_STATE_UNUSED);
		return -1;
	}

	tlogi("indicator wait msg\n");
	ret = indicator_wait_msg();
	if (ret != 0) {
		tloge("indicator wait msg failed\n");
		return -1;
	}

	if (g_indicator_dss_power_shm == NULL) {
		tloge("indicator dss power shm is null\n");
		return -1;
	}

	tlogi("indicator wait msg success\n");
	g_indicator_ctrl.ret = g_indicator_msg_result;
	g_indicator_ctrl.panel_type = g_panel_info.panel_type;
	if (g_indicator_ctrl.ret == 0)
		atomic_set(&g_indicator_state, INDICATOR_STATE_RUNNING);
	else
		atomic_set(&g_indicator_state, INDICATOR_STATE_UNUSED);
	g_indicator_ctrl.shm_phy_addr = virt_to_phys(g_indicator_dss_power_shm);
	g_indicator_ctrl.shm_phy_addr_size = INDICATOR_AGENT_SHM_SIZE;

	return 0;
}

static int del_indicator_layer(void)
{
	int ret = 0;
	if (atomic_read(&g_indicator_state) != INDICATOR_STATE_RUNNING) {
		tloge("indicator is not running\n");
		return 0;
	}

	ret = g_init_func(NULL, INDICATOR_CLOSE);
	if (ret != 0) {
		tloge("notify dpu indicator request failed\n");
		return -1;
	}

	atomic_set(&g_indicator_state, INDICATOR_STATE_UNUSED);

	return 0;
}

static int indicator_agent_init(struct tee_agent_kernel_ops *agent_instance)
{
	(void)agent_instance;
	init_waitqueue_head(&g_indicator_msg_wq);

	g_cmd_indicator_wq = create_singlethread_workqueue("tz_cmd_indicator_wq");
	if (g_cmd_indicator_wq == NULL) {
		tloge("alloc indicator cmd wq failed\n");
		return -1;
	}

	/* shm for dss power state */
	g_indicator_dss_power_shm = kzalloc(INDICATOR_AGENT_SHM_SIZE, GFP_KERNEL);
	if (ZERO_OR_NULL_PTR((unsigned long)(uintptr_t)g_indicator_dss_power_shm)) {
		tloge("alloc indicator shm for dss power state failed\n");
		return -1;
	}

	(void)memset_s(g_indicator_dss_power_shm, INDICATOR_AGENT_SHM_SIZE, 0, INDICATOR_AGENT_SHM_SIZE);
	uint32_t *check_val = (uint32_t*)g_indicator_dss_power_shm;
	*check_val = 0;

	return 0;
}

static int indicator_agent_work_prepare(struct tee_agent_kernel_ops *agent_instance)
{
	struct indicator_ctrl *trans_ctrl = NULL;

	trans_ctrl = (struct indicator_ctrl *)agent_instance->agent_buff;
	if (trans_ctrl->magic != TEE_INDICATOR_AGENT_ID) {
		tloge("check magic error, now is 0x%x\n", trans_ctrl->magic);
		trans_ctrl->ret = TEEC_ERROR_BAD_FORMAT;
		return -1;
	}

	if (memcpy_s((void *)&g_indicator_ctrl, agent_instance->agent_buff_size,
		(void *)trans_ctrl, sizeof(struct indicator_ctrl)) != EOK) {
		tloge("indicator agent work prepare memcpy_s failed\n");
		trans_ctrl->ret = TEEC_ERROR_GENERIC;
		return -1;
	}

	return 0;
}

static int indicator_agent_work_proc_cmd(void)
{
	int ret = 0;
	if (g_indicator_ctrl.cmd >= INDICATOR_LAYER_INVALID || g_indicator_ctrl.cmd < 0) {
		tloge("cmd not supported 0x%x\n", g_indicator_ctrl.cmd);
		return -1;
	}

	g_indicator_msg_flag = 0;
	if (g_indicator_ctrl.cmd == INDICATOR_LAYER_ADD) {
		ret = add_indicator_layer();
		if (ret != 0) {
			g_init_func(NULL, INDICATOR_CLOSE);
			tloge("add indicator layer failed, exit\n");
		}
	} else {
		ret = del_indicator_layer();
	}

	return ret;
}

static int indicator_agent_work_proc(void)
{
	g_indicator_ctrl.ret = 0;

	if (g_init_func == NULL) {
		tloge("dss not register indicator agent driver\n");
		g_indicator_ctrl.ret = -1;
		return -1;
	}

	if (indicator_agent_work_proc_cmd() != 0) {
		g_indicator_ctrl.ret = -1;
		return -1;
	}

	return 0;
}

static int indicator_agent_work_response(struct tee_agent_kernel_ops *agent_instance)
{
	struct indicator_ctrl *trans_ctrl = NULL;

	trans_ctrl = (struct indicator_ctrl *)agent_instance->agent_buff;
	/* response to TEE */
	if (memcpy_s((void *)trans_ctrl, agent_instance->agent_buff_size, (void *)&g_indicator_ctrl,
		sizeof(struct indicator_ctrl)) != EOK) {
		tloge("indicator agent work response memcpy_s failed\n");
		trans_ctrl->ret = TEEC_ERROR_GENERIC;
		return -1;
	}

	return 0;
}

static int indicator_agent_work(struct tee_agent_kernel_ops *agent_instance)
{
	int ret = 0;

	if (agent_instance == NULL || agent_instance->agent_buff == NULL || agent_instance->agent_buff_size != PAGE_SIZE) {
		tloge("agent buff invalid\n");
		return -1;
	}

	mutex_lock(&g_indicator_agent_lock);

	ret = indicator_agent_work_prepare(agent_instance);
	if (ret != 0) {
		tloge("indicator agent work prepare failed\n");
		goto out;
	}

	if (indicator_agent_work_proc() != 0)
		tloge("indicator agent work handle failed\n");

	ret = indicator_agent_work_response(agent_instance);
	if (ret != 0) {
		tloge("indicator agent work response failed\n");
		goto out;
	}

out:
	mutex_unlock(&g_indicator_agent_lock);
	return ret;
}

static int indicator_agent_exit(struct tee_agent_kernel_ops *agent_instance)
{
	(void)agent_instance;
	tloge("indicator agent is exit is being invoked\n");

	cancel_delayed_work(&indicator_poweron_work);
	cancel_delayed_work(&indicator_poweroff_work);
	cancel_delayed_work(&indicator_poweron_abnormal_work);
	cancel_delayed_work(&indicator_poweroff_abnormal_work);
	if (g_cmd_indicator_wq) {
		flush_workqueue(g_cmd_indicator_wq);
		destroy_workqueue(g_cmd_indicator_wq);
		g_cmd_indicator_wq = NULL;
	}

	if (g_indicator_dss_power_shm != NULL) {
		kfree(g_indicator_dss_power_shm);
		g_indicator_dss_power_shm = NULL;
	}

	return 0;
}

static int indicator_agent_crash_work(
	struct tee_agent_kernel_ops *agent_instance,
	struct tc_ns_client_context *context, unsigned int dev_file_id)
{
	(void)context;
	(void)dev_file_id;
	tloge("indicator agent crash\n");
	(void)indicator_agent_exit(agent_instance);

	return 0;
}

static struct tee_agent_kernel_ops g_indicator_agent_ops = {
	.agent_name = "indicator",
	.agent_id = TEE_INDICATOR_AGENT_ID,
	.tee_agent_init = indicator_agent_init,
	.tee_agent_work = indicator_agent_work,
	.tee_agent_exit = indicator_agent_exit,
	.tee_agent_crash_work = indicator_agent_crash_work,
	.agent_buff_size = PAGE_SIZE,
	.list = LIST_HEAD_INIT(g_indicator_agent_ops.list)
};

int register_indicator_agent(void)
{
	tee_agent_kernel_register(&g_indicator_agent_ops);
	return 0;
}

/* DSS driver call when screen on */
int register_secure_display_driver(notify_dpu func, void *pdata)
{
	if (func == NULL || pdata == NULL) {
		tloge("Invalid params\n");
		return -EINVAL;
	}

	mutex_lock(&g_indicator_agent_lock);

	g_init_func = func;
	if (g_init_func == NULL) {
		tloge("Failed to get the callback function.\n");
		mutex_unlock(&g_indicator_agent_lock);
		return -1;
	}

	g_panel_info.panel_type = ((struct indicator_panel_info *)pdata)->panel_type;
	g_panel_info.xres = ((struct indicator_panel_info *)pdata)->xres;
	g_panel_info.yres = ((struct indicator_panel_info *)pdata)->yres;
	if (g_panel_info.xres > SCREEN_PIXEL_X_MAX || g_panel_info.yres > SCREEN_PIXEL_Y_MAX) {
		tloge("The screen resolution parameter is incorrect.\n");
		mutex_unlock(&g_indicator_agent_lock);
		return -1;
	}
	mutex_unlock(&g_indicator_agent_lock);
	tlogi("register secure display driver success\n");
	return 0;
}

/* DSS driver call when screen off */
void unregister_secure_display_driver(void)
{
	mutex_lock(&g_indicator_agent_lock);
	g_init_func = NULL;
	atomic_set(&g_indicator_state, INDICATOR_STATE_UNUSED);
	mutex_unlock(&g_indicator_agent_lock);
	tlogi("unregister secure display driver success\n");
}

int send_secure_display_msg_config(int type)
{
	tlogi("send secure display msg config ret is %d\n", type);
	g_indicator_msg_result = type;
	g_indicator_msg_flag = 1;
	wake_up(&g_indicator_msg_wq);
	return 0;
}

static void notify_dpu_response(int res_cmd)
{
	if (!(res_cmd == INDICATOR_CLOSE || res_cmd == INDICATOR_ABNORMAL)) {
		tloge("notify dpu response cmd %d error\n", res_cmd);
		return;
	}

	mutex_lock(&g_indicator_agent_lock);
	tlogi("notify dpu response cmd %d\n", res_cmd);
	if (g_init_func == NULL) {
		tloge("notify dpu response cmd %d failed\n", res_cmd);
		mutex_unlock(&g_indicator_agent_lock);
		return;
	}

	if (g_init_func(NULL, res_cmd) != 0) {
		tloge("notify dpu response cmd %d failed\n", res_cmd);
	} else {
		tlogi("notify dpu response cmd %d success\n", res_cmd);
		if (res_cmd == INDICATOR_CLOSE)
			atomic_set(&g_indicator_state, INDICATOR_STATE_UNUSED);
	}

	mutex_unlock(&g_indicator_agent_lock);
}

#define INDICATOR_NOTIFY_AGENT_EVENT 0x4FE

static int indicator_agent_work_ca2ta(uint32_t cmd_id)
{
	int ret = 0;
	uint32_t result;
	struct teec_session session = { 0 };
	struct teec_context context = { 0 };
	TEEC_UUID uuid = TEE_PRIVACY_PROTECT_ENHANCE;
	struct teec_operation operation = { 0 };
	uint32_t origin = 0;
	const char *package_name = "indicator_agent";
	uint32_t root_id = 0;

	tlogi("indicator agent work ca2ta begin %u\n", cmd_id);

	result = teek_initialize_context(NULL, &context);
	if (result != TEEC_SUCCESS) {
		tloge("teek initialize context failed 0x%x\n", result);
		return -1;
	}

	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramtypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_MEMREF_TEMP_INPUT, TEEC_MEMREF_TEMP_INPUT);

	operation.params[2].tmpref.buffer = (void *)(&root_id);
	operation.params[2].tmpref.size = (uint32_t)sizeof(root_id);

	operation.params[3].tmpref.buffer = (void *)package_name;
	operation.params[3].tmpref.size = (size_t)(strlen(package_name) + 1);
	result = teek_open_session(&context, &session, &uuid, TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
	if (result != TEEC_SUCCESS) {
		tloge("teek open session failed 0x%x\n", result);
		teek_finalize_context(&context);
		return -1;
	}

	operation.paramtypes = TEEC_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	operation.params[0].value.a = cmd_id;
	result = teek_invoke_command(&session, INDICATOR_NOTIFY_AGENT_EVENT, &operation, &origin);
	if (result != TEEC_SUCCESS) {
		tloge("teek invoke command ret 0x%x\n", result);
		ret = -1;
	}

	teek_close_session(&session);
	teek_finalize_context(&context);

	tlogi("indicator agent work ca2ta end %u, %d\n", cmd_id, ret);

	return ret;
}

#define MAX_INDICATOR_NS_CMD_RETRY 3
static int indicator_agent_event_work(uint32_t cmd_id)
{
	int ret = 0;
	int retry = 0;

	do {
		ret = indicator_agent_work_ca2ta(cmd_id);
		if (ret == 0)
			break;
		retry++;
	} while (retry < MAX_INDICATOR_NS_CMD_RETRY);

	return ret;
}

static void indicator_poweron_work_func(struct work_struct *work)
{
	(void)work;
	tlogi("indicator poweron work func\n");
}

static void indicator_poweron_abnormal_work_func(struct work_struct *work)
{
	(void)work;
	tlogi("indicator poweron abnorma work func\n");

	(void)indicator_agent_event_work(INDICATOR_AGENT_EVENT_DSS_POWERON_ABNORMAL);
}

static void indicator_poweroff_work_func(struct work_struct *work)
{
	(void)work;
	tlogi("indicator poweroff work func\n");

	int ret = indicator_agent_event_work(INDICATOR_AGENT_EVENT_DSS_POWEROFF);
	if (ret == 0)
		notify_dpu_response(INDICATOR_CLOSE);
	else
		tloge("indicator poweroff work func ret %d\n", ret);
}

static void indicator_poweroff_abnormal_work_func(struct work_struct *work)
{
	(void)work;
	tlogi("indicator poweroff abnormal work func\n");

	int ret = indicator_agent_event_work(INDICATOR_AGENT_EVENT_DSS_POWEROFF_ABNORMAL);
	if (ret == 0)
		notify_dpu_response(INDICATOR_ABNORMAL);
	else
		tloge("indicator poweroff work func ret %d\n", ret);
}

void secure_display_poweron_work_start(bool is_normal)
{
	tlogi("secure display poweron work start %d\n", is_normal);
	if (g_cmd_indicator_wq == NULL) {
		tloge("secure display poweron work failed\n");
		return;
	}

	if (is_normal) {
		queue_work(g_cmd_indicator_wq, &indicator_poweron_work.work);
	} else {
		if (g_indicator_dss_power_shm != NULL) {
			(void)memset_s(g_indicator_dss_power_shm, INDICATOR_AGENT_SHM_SIZE, 0, INDICATOR_AGENT_SHM_SIZE);
			uint32_t *check_val = (uint32_t*)g_indicator_dss_power_shm;
			*check_val = 0;
			tlogw("secure_display_check_val ++ %u\n", *check_val);
		}
		queue_work(g_cmd_indicator_wq, &indicator_poweron_abnormal_work.work);
	}
}

void secure_display_poweroff_work_start(bool is_normal)
{
	tlogi("secure display poweroff work start %d\n", is_normal);
	if (g_cmd_indicator_wq == NULL) {
		tloge("secure display poweroff work failed\n");
		return;
	}

	if (is_normal) {
		queue_work(g_cmd_indicator_wq, &indicator_poweroff_work.work);
	} else {
		if (g_indicator_dss_power_shm != NULL) {
			(void)memset_s(g_indicator_dss_power_shm, INDICATOR_AGENT_SHM_SIZE, 0, INDICATOR_AGENT_SHM_SIZE);
			uint32_t *check_val = (uint32_t*)g_indicator_dss_power_shm;
			*check_val = 1;
			tlogw("secure_display_check_val -- %u\n", *check_val);
		}
		queue_work(g_cmd_indicator_wq, &indicator_poweroff_abnormal_work.work);
	}
}

static void indicator_tui_exit_work_func(struct work_struct *work)
{
	(void)work;
	tlogi("indicator tui exit work func\n");
	int ret = indicator_agent_event_work(INDICATOR_AGENT_EVENT_TUI_EXIT);
	tlogi("indicator tui exit work func ret %d\n", ret);
}
 
static DECLARE_DELAYED_WORK(indicator_tui_exit_work, indicator_tui_exit_work_func);
void secure_display_tui_work_start(bool is_tui_entry)
{
	int ret;
	tlogi("secure display tui work start %d\n", is_tui_entry);
	if (g_cmd_indicator_wq == NULL) {
		tloge("secure display tui work failed\n");
		return;
	}
	
	if (is_tui_entry) {
		/* tui on need send message synchronize */
		ret = indicator_agent_event_work(INDICATOR_AGENT_EVENT_TUI_ENTRY);
		tlogi("secure display tui work start syncret %d\n", ret);
	} else {
		queue_work(g_cmd_indicator_wq, &indicator_tui_exit_work.work);
	}
}