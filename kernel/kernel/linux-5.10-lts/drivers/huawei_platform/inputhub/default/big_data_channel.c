/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: big data channel source file
 * Author: DIVS_SENSORHUB
 * Create: 2012-05-29
 */

#include "big_data_channel.h"

#include <linux/device.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/time.h>

#include <huawei_platform/log/hwlog_kernel.h>
#include <huawei_platform/log/imonitor.h>
#include <securec.h>
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_LIBLINUX)
#include <dfx/hiview_hisysevent.h>
#endif

#include "contexthub_boot.h"
#include "contexthub_debug.h"
#include "contexthub_ext_log.h"
#include "contexthub_pm.h"
#include "contexthub_recovery.h"
#include "contexthub_route.h"
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_sysfs.h"
#include <platform_include/smart/linux/base/ap/ddr_config/athena_config.h>

#define NO_TAG (-1)

/* system load bigdata */
#define SYSLOAD_MAX_TASK_NAME 16
#define SYSLOAD_TASK_NAME_BUFF_LENGTH (2 * SYSLOAD_MAX_TASK_NAME)
#define SYSLOAD_PERIOD_UPLOAD_FILED_NUM 8
#define SYSLOAD_PERIOD_UPLOAD_TASK_NUM 5
#define SYSLOAD_IDX0 0
#define SYSLOAD_IDX1 1
#define SYSLOAD_IDX2 2
#define SYSLOAD_IDX3 3
#define SYSLOAD_IDX4 4
#define SYSLOAD_IDX5 5
#define SYSLOAD_IDX6 6
#define SYSLOAD_IDX7 7
#define SYSLOAD_OFFSET 1
#define BUFFER_LEN 512
#define POSTURE_CNT 15
#define POSTURE_BLOCK_CNT 5
#define COPY_DEST_MAX 64
#define THOUSAND 1000
#define DDR_REQ_THRESHOLD 1
#define SUSPEND_TIME_MIN 1000
#define MAX_INT_STR_LEN 12
#define ARRAY_NUM 15
#define DDRREQ_DATA_TYPE 5
#define COMMA_SIZE 2 
#define DDRREQ_DATA_SIZE (ARRAY_NUM * (MAX_INT_STR_LEN + COMMA_SIZE))
#define ATHENA_DATA_SIZE 3400
#define ATHENA_DURATION_SIZE 2120
#define ATHENA_DURATION_ARRAY_SIZE 20
#define STACK_INFO_SIZE 1152
#define STATE_DURATION_ARRAY_SIZE 208
#define APP_STATUS_DATA_SIZE 70

static struct detect_result g_crash_result;
static ddr_request_pack ddr_static_info;
static struct workqueue_struct *ddr_req_statis_workqueue = NULL;
static struct work_struct ddr_req_statis_work;
static struct timeval *ddr_req_statis_resume_time = NULL;
static struct timeval *ddr_req_statis_suspend_time = NULL;
struct sysload_peak {
	uint32_t max_time;
	uint32_t all_time;
	uint32_t rtc;
};

struct sysload_period {
	char task_name[SYSLOAD_MAX_TASK_NAME];
	struct sysload_peak peak;
	uint32_t run_time;
	uint32_t over_time;
	uint32_t over_cnt;
	uint32_t threshold;
};

struct sysload_trigger {
	char task_name[SYSLOAD_MAX_TASK_NAME];
	uint32_t max_time;
	uint32_t all_time;
};

const char *sysload_str[SYSLOAD_PERIOD_UPLOAD_FILED_NUM] = {
	"name_",
	"threshold_",
	"max_time_",
	"all_time_",
	"rtc_",
	"run_time_",
	"over_threshold_cnt_",
	"over_threshold_time_",
};

static char sysload_name_buf[SYSLOAD_TASK_NAME_BUFF_LENGTH];

/* 1.event parameter setting */
static big_data_param_detail_t event_motion_type_param[] = {
	{ "Pickup", INT_PARAM, MOTION_TYPE_PICKUP },
	{ "Flip", INT_PARAM, MOTION_TYPE_FLIP },
	{ "Proximity", INT_PARAM, MOTION_TYPE_PROXIMITY },
	{ "Shake", INT_PARAM, MOTION_TYPE_SHAKE },
	{ "TiltLr", INT_PARAM, MOTION_TYPE_TILT_LR },
	{ "Pocket", INT_PARAM, MOTION_TYPE_POCKET },
	{ "Rotation", INT_PARAM, MOTION_TYPE_ROTATION },
	{ "Activity", INT_PARAM, MOTION_TYPE_ACTIVITY },
	{ "TakeOff", INT_PARAM, MOTION_TYPE_TAKE_OFF },
	{ "HeadDown", INT_PARAM, MOTION_TYPE_HEAD_DOWN },
	{ "PutDown", INT_PARAM, MOTION_TYPE_PUT_DOWN },
	{ "Sidegrip", INT_PARAM, MOTION_TYPE_SIDEGRIP },
};

static big_data_param_detail_t event_ddr_info_param[] = {
	{ "Times", INT_PARAM, NO_TAG },
	{ "Duration", INT_PARAM, NO_TAG }
};

static big_data_param_detail_t event_tof_phonecall_param[] = {
	{ "Closest", INT_PARAM, NO_TAG },
	{ "Farthest", INT_PARAM, NO_TAG }
};

static big_data_param_detail_t event_phonecall_screen_param[] = {
	{"Times", INT_PARAM, NO_TAG}
};
static big_data_param_detail_t event_fold_temp_param[] = {
	{"Temperature", INT_PARAM, NO_TAG},
	{"Apchipstate", INT_PARAM, NO_TAG}
};

static big_data_param_detail_t event_vib_resp_time[] = {
	{ "sh_vib_resp_time", INT_PARAM, NO_TAG }
};

static big_data_param_detail_t event_ps_sound_param[] = {
	{ "Num_Call1", INT_PARAM, NO_TAG },
	{ "Num_Call2", INT_PARAM, NO_TAG },
	{ "Num_Call3", INT_PARAM, NO_TAG },
	{ "Num_Call4", INT_PARAM, NO_TAG },
	{ "Num_Entering1", INT_PARAM, NO_TAG },
	{ "Num_Entering2", INT_PARAM, NO_TAG },
	{ "Num_Entering3", INT_PARAM, NO_TAG },
	{ "Num_Entering4", INT_PARAM, NO_TAG },
	{ "Num_Leaving1", INT_PARAM, NO_TAG },
	{ "Num_Leaving2", INT_PARAM, NO_TAG },
	{ "Num_Leaving3", INT_PARAM, NO_TAG },
	{ "Num_Leaving4", INT_PARAM, NO_TAG },
	{ "Num_Within1", INT_PARAM, NO_TAG },
	{ "Num_Within2", INT_PARAM, NO_TAG },
	{ "Num_Within3", INT_PARAM, NO_TAG },
	{ "Num_Within4", INT_PARAM, NO_TAG }
};

static big_data_param_detail_t event_aod_info_param[] = {
	{ "aod_play_time", INT_PARAM, NO_TAG },
	{ "swing_animation_count", INT_PARAM, NO_TAG }
};

static big_data_param_detail_t event_blpwm_info_param[] = {
	{ "powers", INT_PARAM, NO_TAG },
	{ "dutys", INT_PARAM, NO_TAG },
	{ "flips", INT_PARAM, NO_TAG },
	{ "taps", INT_PARAM, NO_TAG },
};

static big_data_param_detail_t event_cover_type_param[] = {
	{ "type", INT_PARAM, NO_TAG },
	{ "timestamp", INT_PARAM, NO_TAG },
	{ "day", INT_PARAM, NO_TAG },
	{ "offset_base", INT_PARAM, NO_TAG },
	{ "offset_cur", INT_PARAM, NO_TAG },
	{ "offset_day", INT_PARAM, NO_TAG },
	{ "offset_week", INT_PARAM, NO_TAG },
	{ "offset_month", INT_PARAM, NO_TAG },
	{ "offset_quarter", INT_PARAM, NO_TAG },
	{ "offset_year", INT_PARAM, NO_TAG },
	{ "offset_history", INT_PARAM, NO_TAG },
	{ "status", INT_PARAM, NO_TAG },
	{ "chn_datas_0", INT_PARAM, NO_TAG },
	{ "chn_datas_1", INT_PARAM, NO_TAG },
	{ "chn_datas_2", INT_PARAM, NO_TAG },
	{ "chn_datas_3", INT_PARAM, NO_TAG },
	{ "chn_datas_4", INT_PARAM, NO_TAG },
	{ "chn_datas_5", INT_PARAM, NO_TAG },
	{ "chn_datas_6", INT_PARAM, NO_TAG },
	{ "chn_datas_7", INT_PARAM, NO_TAG },
};
/* 2.register event param here {EVENT_ID, param_num, param_detail_struct} */
static const big_data_event_detail_t big_data_event[] = {
	{ BIG_DATA_EVENT_MOTION_TYPE, 12, event_motion_type_param },
	{ BIG_DATA_EVENT_DDR_INFO, 2, event_ddr_info_param },
	{ BIG_DATA_EVENT_TOF_PHONECALL, 2, event_tof_phonecall_param },
	{ BIG_DATA_EVENT_PHONECALL_SCREEN_STATUS, 1,
		event_phonecall_screen_param },
	{ BIG_DATA_EVENT_VIB_RESP_TIME, 1, event_vib_resp_time },
	{ BIG_DATA_FOLD_TEMP, 2, event_fold_temp_param },
	{ BIG_DATA_EVENT_PS_SOUND_INFO, 16, event_ps_sound_param },
	{ BIG_DATA_EVENT_AOD_INFO, 2, event_aod_info_param },
	{ BIG_DATA_EVENT_BLPWM_USED_INFO, 4, event_blpwm_info_param },
	{ BIG_DATA_EVENT_COVER_OFFSET_INFO, 20, event_cover_type_param },
};

/* 3.(optional)map tag to str */
static const char *big_data_str_map[] = {
	[BIG_DATA_STR] = "BIG_DATA_STR",
};

#ifdef CONFIG_LIBLINUX
static void athena_duration_tostr(char **str_offset, const char *const off_off, const char *const on_off,
	const char *const off_on, const char *const on_on, unsigned long long off_off_data, unsigned long long on_off_data,
	unsigned long long off_on_data, unsigned long long on_on_data)
{
	int offset = snprintf_s(*str_offset, STATE_DURATION_ARRAY_SIZE, STATE_DURATION_ARRAY_SIZE - 1,
		"%s,%s,%x/ %s,%s,%x/ %s,%s,%x/ %s,%s,%x/ ", on_off, on_on, off_off_data, off_off, on_on,
		on_off_data, on_off, off_on, off_on_data, off_off, off_on, on_on_data);
	if (offset < 0) {
			hwlog_err("%s, offset %d, athena_duration snprintf_s error\n", __func__, offset);
			return;
	}
	*str_offset += offset;
}

static void athena_stack_tostr(char **str_offset, int inst_id, int lmt_val)
{
	int offset = snprintf_s(*str_offset, ATHENA_DURATION_ARRAY_SIZE,
		ATHENA_DURATION_ARRAY_SIZE - 1, "%x,%x/ ", inst_id, lmt_val);
	if (offset < 0) {
			hwlog_err("%s, offset %d, athena_stack snprintf_s error\n", __func__, offset);
			return;
	}
	*str_offset += offset;
}

static void athena_app_state_tostr(char **str_offset, unsigned long long dataone,
	unsigned long long datatwo, unsigned long long datathree, unsigned long long datafour)
{
	int offset = snprintf_s(*str_offset, APP_STATUS_DATA_SIZE,
		APP_STATUS_DATA_SIZE - 1, "%x,%x,%x,%x/  ", dataone, datatwo, datathree, datafour);
	if (offset < 0) {
			hwlog_err("%s, offset %d, app_state snprintf_s error\n", __func__, offset);
			return;
	}
	*str_offset += offset;
}
#endif

static int iomcu_big_data_fetch(uint32_t event_id, void *data, uint32_t length)
{
	struct write_info pkg_ap;
	struct read_info pkg_mcu;
	int ret;

	memset_s(&pkg_ap, sizeof(pkg_ap), 0, sizeof(pkg_ap));
	memset_s(&pkg_mcu, sizeof(pkg_mcu), 0, sizeof(pkg_mcu));

	pkg_ap.tag = TAG_BIG_DATA;
	pkg_ap.cmd = CMD_BIG_DATA_REQUEST_DATA;
	pkg_ap.wr_buf = &event_id;
	pkg_ap.wr_len = sizeof(event_id);

	if (get_iom3_state() != IOM3_ST_NORMAL) {
		hwlog_err("%s fail g_iom3_state=%d\n", __func__,
			get_iom3_state());
		return -1;
	}

	ret = write_customize_cmd(&pkg_ap, &pkg_mcu, true);
	if (ret != 0) {
		hwlog_err("send big data fetch req type = %d fail\n", event_id);
		return -1;
	} else if (pkg_mcu.errno != 0) {
		hwlog_err("big data fetch req to mcu fail errno = %d\n",
			pkg_mcu.errno);
		return -1;
	} else {
		if (length < MAX_PKT_LENGTH)
			memcpy_s(data, length, pkg_mcu.data, length);
		else
			memcpy_s(data, MAX_PKT_LENGTH, pkg_mcu.data, MAX_PKT_LENGTH);
	}
	return 0;
}

int iomcu_dubai_log_fetch(uint32_t event_type, void *data, uint32_t length)
{
	int ret = -1;

	if (event_type > DUBAI_EVENT_END || event_type < DUBAI_EVENT_NULL) {
		hwlog_err("dubai log fetch event_type: %d illegal\n",
			event_type);
		return ret;
	}
	ret = iomcu_big_data_fetch(event_type, data, length);
	return ret;
}

static const char *const g_athena_event_name_on[] = {
	"Still",
	"Max Power",
	"Peri Vote",
	"IOMCU SLEEP",
	"AP Suspend",
	"Screen On",
	"Syscache Req",
	"PLL On",
	"FLL On",
	"CPU Busy",
};

static const char *const g_athena_event_name_off[] = {
	"MOVE",
	"Non-Max Power",
	"Peri Release",
	"IOMCU NORMAL",
	"AP Resume",
	"Screen Off",
	"Syscache Release",
	"PLL Off",
	"FLL On",
	"CPU Not-Busy",
};

static const char *const g_athena_limit_name[] = {
	"heap",
	"stack",
};
#ifdef CONFIG_LIBLINUX
static void athena_duration_big_data_ohos(athena_stastics_t *p_athena, char *update_state_duration_offset) {
	uint32_t i = 0;
	for (i = 0; i < ATHENA_EVENT_BUTT; i++) {
		athena_event_e depend = p_athena->evt[i].depend;
		if (depend >= ATHENA_EVENT_BUTT) {
			// current event depend on nothing.
			hwlog_info("%s : duration : %ld ms\n", g_athena_event_name_off[i], p_athena->evt[i].duration[0][0]);
			hwlog_info("%s : duration : %ld ms\n", g_athena_event_name_on[i], p_athena->evt[i].duration[1][0]);
		} else {
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_off[i], g_athena_event_name_off[depend], p_athena->evt[i].duration[0][0]);
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_on[i], g_athena_event_name_off[depend], p_athena->evt[i].duration[1][0]);
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_off[i], g_athena_event_name_on[depend], p_athena->evt[i].duration[0][1]);
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_on[i], g_athena_event_name_on[depend], p_athena->evt[i].duration[1][1]);
			athena_duration_tostr(&update_state_duration_offset, g_athena_event_name_on[i], g_athena_event_name_off[i],
				g_athena_event_name_on[depend], g_athena_event_name_off[depend], p_athena->evt[i].duration[0][0],
				p_athena->evt[i].duration[1][0], p_athena->evt[i].duration[0][1], p_athena->evt[i].duration[1][1]);
		}
	}
}
#endif
#ifndef CONFIG_LIBLINUX
static void athena_duration_big_data(athena_stastics_t *p_athena) {
	uint32_t i = 0;
	for (i = 0; i < ATHENA_EVENT_BUTT; i++) {
		athena_event_e depend = p_athena->evt[i].depend;
		if (depend >= ATHENA_EVENT_BUTT) {
			// current event depend on nothing.
			hwlog_info("%s : duration : %ld ms\n", g_athena_event_name_off[i], p_athena->evt[i].duration[0][0]);
			hwlog_info("%s : duration : %ld ms\n", g_athena_event_name_on[i], p_athena->evt[i].duration[1][0]);
		} else {
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_off[i], g_athena_event_name_off[depend], p_athena->evt[i].duration[0][0]);
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_on[i], g_athena_event_name_off[depend], p_athena->evt[i].duration[1][0]);
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_off[i], g_athena_event_name_on[depend], p_athena->evt[i].duration[0][1]);
			hwlog_info("%s when %s : duration : %ld ms\n",
				g_athena_event_name_on[i], g_athena_event_name_on[depend], p_athena->evt[i].duration[1][1]);
		}
	}
}
#endif

static void process_athena_big_data(uint32_t event_id, void *data, uint32_t data_len)
{
	uint32_t j = 0;
	athena_stastics_t *p_athena = (athena_stastics_t *)data;

	if (data_len != sizeof(athena_stastics_t)) {
		hwlog_err("%s : data error %d\n", __func__, data_len);
		return;
	}
#ifdef CONFIG_LIBLINUX
	struct hiview_hisysevent *event = NULL;
	event = hisysevent_create("SENSORHUB", "SHB_OPT_STATIS", STATISTIC);
	char *update_state_duration = kzalloc(ATHENA_DATA_SIZE, GFP_KERNEL);
	if (update_state_duration == NULL) {
		hwlog_err("%s athena_big_data allocate failed\n",  __func__);
		return ;
	}
	char *update_state_duration_offset = update_state_duration;
	char *update_stack_offset = update_state_duration + ATHENA_DURATION_SIZE;
	char *update_app_state_offset = update_state_duration +
		ATHENA_DURATION_SIZE + STACK_INFO_SIZE;
	athena_duration_big_data_ohos(p_athena, update_state_duration_offset);
#endif
	hwlog_info("%s.......\n", __func__);
#ifndef CONFIG_LIBLINUX
	athena_duration_big_data(p_athena);
#endif
	for (j = 0; j < ATHENA_LIMIT_MAX_INST; j++) {
		if (p_athena->lmt[j].lmt_val != 0) {
			hwlog_info("id 0x%x limit usage %d\n",
				p_athena->lmt[j].inst_id, p_athena->lmt[j].lmt_val);
#ifdef CONFIG_LIBLINUX
			athena_stack_tostr(&update_stack_offset, p_athena->lmt[j].inst_id, p_athena->lmt[j].lmt_val);
#endif
		}
	}
	hwlog_info("APP Open State : %lx_%lx_%lx_%lx",
		p_athena->app_sts[0], p_athena->app_sts[1], p_athena->app_sts[2], p_athena->app_sts[3]);
#ifdef CONFIG_LIBLINUX
	athena_app_state_tostr(&update_app_state_offset, p_athena->app_sts[0],
		p_athena->app_sts[1], p_athena->app_sts[2], p_athena->app_sts[3]);
	hisysevent_put_string(event, "STATE_DURATION", update_state_duration);
	hisysevent_put_string(event, "ID_LIMIT_USAGE", update_state_duration + ATHENA_DURATION_SIZE);
	hisysevent_put_string(event, "APP_STATUS", update_state_duration +
		ATHENA_DURATION_SIZE + STACK_INFO_SIZE);
	hisysevent_write(event);
	hisysevent_destroy(&event);
	kfree(update_state_duration);
#endif
}

#ifdef CONFIG_HUAWEI_DSM
static int process_big_data(uint32_t event_id, void *data)
{
	struct imonitor_eventobj *obj = NULL;
	int i;
	int ret = 0;
	int tag = 0;
	uint32_t *raw_data = (uint32_t *)data;
	big_data_event_detail_t event_detail;
	big_data_param_detail_t param_detail;

	memset_s(&event_detail, sizeof(event_detail), 0, sizeof(event_detail));
	memset_s(&param_detail, sizeof(param_detail), 0, sizeof(param_detail));

	if (!data) {
		hwlog_err("%s para error\n", __func__);
		return -1;
	}

	obj = imonitor_create_eventobj(event_id);
	if (!obj) {
		hwlog_err("%s imonitor_create_eventobj failed\n", __func__);
		return -1;
	}
	for (i = 0; i < sizeof(big_data_event) / sizeof(big_data_event[0]); ++i) {
		if (big_data_event[i].event_id == event_id) {
			memcpy_s(&event_detail, sizeof(event_detail), &big_data_event[i],
				sizeof(event_detail));
			break;
		}
	}

	for (i = 0; i < event_detail.param_num; ++i) {
		memcpy_s(&param_detail, sizeof(param_detail), &event_detail.param_data[i],
			sizeof(param_detail));
		tag = (param_detail.tag == NO_TAG) ? i : param_detail.tag;
		if (param_detail.param_type == INT_PARAM) {
			ret += imonitor_set_param_integer_v2(obj,
				param_detail.param_name, raw_data[tag]);
		} else if (param_detail.param_type == STR_PARAM) {
			if (big_data_str_map[raw_data[tag]])
				ret += imonitor_set_param_string_v2(obj,
					param_detail.param_name,
					big_data_str_map[raw_data[tag]]);
		}
	}

	if (ret) {
		imonitor_destroy_eventobj(obj);
		hwlog_err("%s imonitor_set_para fail, ret %d\n", __func__, ret);
		return ret;
	}

	ret = imonitor_send_event(obj);
	hwlog_info("big data imonitor send event id: %d success\n", event_id);
	if (ret < 0)
		hwlog_err("%s imonitor_send_event fail, ret %d\n",
			__func__, ret);

	imonitor_destroy_eventobj(obj);
	return ret;
}

static void process_carcrash_big_data(uint32_t event_id, void *data, uint32_t data_len)
{
	uint32_t *raw_data = (uint32_t *)data;
	int ret;

	if (!data) {
		hwlog_err("%s data error\n", __func__);
		return;
	}

	if (data_len == sizeof(g_crash_result)) {
		ret = memcpy_s(&g_crash_result, sizeof(g_crash_result), raw_data, data_len);
		if (ret != EOK) {
			hwlog_err("%s: memcpy_s fail, err:%d\n", __func__, ret);
			return;
		}
	} else {
		hwlog_err("%s: data_len invalid, data_len = %u\n", __func__, data_len);
		return;
	}

	hwlog_info("carcrash big data finish");
}

detect_result get_crash_result(void)
{
	return g_crash_result;
}
EXPORT_SYMBOL(get_crash_result);

static int do_sysload_trigger_upload(struct imonitor_eventobj *obj, void *data)
{
	struct sysload_trigger *raw_data = (struct sysload_trigger *)data;
	char *task_name = raw_data->task_name;
	int ret = 0;

	ret += imonitor_set_param_string_v2(obj, "name", task_name);
	ret += imonitor_set_param_integer_v2(obj, "max_time",
					     raw_data->max_time);
	ret += imonitor_set_param_integer_v2(obj, "all_time",
					     raw_data->all_time);

	return ret;
}

static int do_sysload_period_upload(struct imonitor_eventobj *obj, void *data)
{
	struct sysload_period *raw_data = (struct sysload_period *)data;
	char *param_name = &sysload_name_buf[0];
	int ret = 0;
	int i;

	for (i = 0; i < SYSLOAD_PERIOD_UPLOAD_TASK_NUM; i++) {
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX0], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_string_v2(obj, param_name,
						    raw_data[i].task_name);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX1], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].threshold);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX2], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].peak.max_time);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX3], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].peak.all_time);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX4], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].peak.rtc);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX5], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].run_time);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX6], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].over_cnt);
		if (sprintf_s(param_name, sizeof(sysload_name_buf), "%s%d",
		    sysload_str[SYSLOAD_IDX7], i + SYSLOAD_OFFSET) < 0)
			return 1;
		ret += imonitor_set_param_integer_v2(obj, param_name,
						     raw_data[i].over_time);
	}
	return ret;
}

static int process_sysload_big_data(uint32_t event_id, void *data)
{
	struct imonitor_eventobj *obj = NULL;
	int ret = 0;

	if (!data) {
		hwlog_err("%s para error\n", __func__);
		return -1;
	}

	obj = imonitor_create_eventobj(event_id);
	if (!obj) {
		hwlog_err("%s imonitor_create_eventobj failed\n", __func__);
		return -1;
	}

	if (event_id == BIG_DATA_EVENT_SYSLOAD_TRIGGER)
		ret = do_sysload_trigger_upload(obj, data);
	else if (event_id == BIG_DATA_EVENT_SYSLOAD_PERIOD)
		ret = do_sysload_period_upload(obj, data);

	if (ret) {
		imonitor_destroy_eventobj(obj);
		hwlog_err("%s imonitor_set_para fail, ret %d\n", __func__, ret);
		return ret;
	}

	ret = imonitor_send_event(obj);
	if (ret < 0)
		hwlog_err("%s imonitor_send_event fail, ret %d\n", __func__,
			  ret);

	imonitor_destroy_eventobj(obj);
	hwlog_info("%s big data imonitor send event id: %d success\n", __func__,
		   event_id);

	return ret;
}
#elif (defined CONFIG_HUAWEI_OHOS_DSM)
static void process_cover_big_data_ohos(uint32_t event_id, void *data, uint32_t data_len)
{
	if (!data) {
		hwlog_err("%s data error\n", __func__);
		return;
	}

	cover_offset_big_data_t *cover_data = (cover_offset_big_data_t *)data;
	int ret = 0;
	struct hiview_hisysevent *event = NULL;

	if (data_len != sizeof(cover_offset_big_data_t)) {
		hwlog_err("%s: data_len invalid, data_len = %u\n", __func__, data_len);
		return;
	}
	event = hisysevent_create("SENSORHUB", "SENSOR_COVER_STATIS", STATISTIC);
	if (!event) {
		hwlog_err("[cover] %s hisysevent_create failed\n", __func__);
		return;
	}

	ret += hisysevent_put_integer(event, "TYPE", (long long)(cover_data->type));
	ret += hisysevent_put_integer(event, "TIMESTAMP", (long long)(cover_data->timestamp));
	ret += hisysevent_put_integer(event, "DAY", (long long)(cover_data->day));
	ret += hisysevent_put_integer(event, "OFFSET_BASE", (long long)(cover_data->offset_base));
	ret += hisysevent_put_integer(event, "OFFSET_CUR", (long long)(cover_data->offset_cur));
	ret += hisysevent_put_integer(event, "OFFSET_DAY", (long long)(cover_data->offset_day));
	ret += hisysevent_put_integer(event, "OFFSET_WEEK", (long long)(cover_data->offset_week));
	ret += hisysevent_put_integer(event, "OFFSET_MONTH", (long long)(cover_data->offset_month));
	ret += hisysevent_put_integer(event, "OFFSET_QUARTER", (long long)(cover_data->offset_quarter));
	ret += hisysevent_put_integer(event, "OFFSET_YEAR", (long long)(cover_data->offset_year));
	ret += hisysevent_put_integer(event, "OFFSET_HISTORY", (long long)(cover_data->offset_history));
	ret += hisysevent_put_integer(event, "STATUS", (long long)(cover_data->status));
	ret += hisysevent_put_integer(event, "CHN_DATAS_0", (long long)(cover_data->chn_datas_0));
	ret += hisysevent_put_integer(event, "CHN_DATAS_1", (long long)(cover_data->chn_datas_1));
	ret += hisysevent_put_integer(event, "CHN_DATAS_2", (long long)(cover_data->chn_datas_2));
	ret += hisysevent_put_integer(event, "CHN_DATAS_3", (long long)(cover_data->chn_datas_3));
	ret += hisysevent_put_integer(event, "CHN_DATAS_4", (long long)(cover_data->chn_datas_4));
	ret += hisysevent_put_integer(event, "CHN_DATAS_5", (long long)(cover_data->chn_datas_5));
	ret += hisysevent_put_integer(event, "CHN_DATAS_6", (long long)(cover_data->chn_datas_6));
	ret += hisysevent_put_integer(event, "CHN_DATAS_7", (long long)(cover_data->chn_datas_7));
	ret += hisysevent_write(event);

	hisysevent_destroy(&event);
	hwlog_info("[process_cover_big_data_ohos]cover big data finish in ohos! ret %d", ret);
}

static void process_carcrash_big_data_ohos(uint32_t event_id, void *data, uint32_t data_len)
{
	if (!data) {
		hwlog_err("%s data error\n", __func__);
		return;
	}

	uint32_t *raw_data = (uint32_t *)data;
	int ret;
	struct hiview_hisysevent *event = NULL;

	if (data_len == sizeof(g_crash_result)) {
		ret = memcpy_s(&g_crash_result, sizeof(g_crash_result), raw_data, data_len);
		if (ret != EOK) {
			hwlog_err("%s: memcpy_s fail, err:%d\n", __func__, ret);
			return;
		}
	} else {
		hwlog_err("%s: data_len invalid, data_len = %u\n", __func__, data_len);
		return;
	}

	if (event_id == BIG_DATA_EVENT_CARCRASH_HAPPENED)
		event = hisysevent_create("SENSOR_DRV", "SENSOR_CARCRASH_INFO", FAULT);
	else // event_id == BIG_DATA_EVENT_CARCRASH_SUSPECTED
		event = hisysevent_create("SENSOR_DRV", "SENSOR_CARCRASH_SUSPECTED_INFO", FAULT);

	if (!event) {
		hwlog_err("[sensorALG] %s hisysevent_create failed\n", __func__);
		return;
	}

	ret = hisysevent_put_integer(event, "TYPE", (long long)(g_crash_result.crash_signal));
	ret = hisysevent_put_integer(event, "PHONE_POSITION", (long long)(g_crash_result.phone_postion));
	ret = hisysevent_put_integer(event, "CRASH_CONFIDENCE", (long long)(g_crash_result.confidence));
	ret = hisysevent_put_integer(event, "GRAVITY_LOST_TIME", (long long)(g_crash_result.gravity_lostime));
	ret = hisysevent_put_integer(event, "GPS_INITIAL_SPEED", (long long)(g_crash_result.gps_initial_speed));
	ret = hisysevent_put_integer(event, "GPS_END_SPEED", (long long)(g_crash_result.gps_end_speed));
	ret = hisysevent_put_integer(event, "BARO_MAX_VALUE", (long long)(g_crash_result.baro_max_value));
	ret = hisysevent_put_integer(event, "BARO_MIN_VALUE", (long long)(g_crash_result.baro_min_value));
	ret = hisysevent_put_integer(event, "AR_STATUS", (long long)(g_crash_result.ar_status));
	ret = hisysevent_put_integer(event, "AR_CONFIDENCE", (long long)(g_crash_result.ar_confidence));
	ret = hisysevent_put_integer(event, "STATIC_TIME_AFTER_COLLISION",
		(long long)(g_crash_result.static_time_after_collision));
	ret = hisysevent_put_integer(event, "AUDIO_EVENT", (long long)(g_crash_result.audio_event));
	ret = hisysevent_put_integer(event, "AUDIO_CONFIDENCE", (long long)(g_crash_result.audio_confidence));
	ret = hisysevent_write(event);
hisysevent_end:
	hisysevent_destroy(&event);
	hwlog_info("[sensorALG][process_carcrash_big_data_ohos]carcrash big data finish in ohos");
}
#endif

static void decode_posture_for_dubai(const pkt_big_data_report_t *report_t)
{
	dubai_posture_t *posture_data = (dubai_posture_t *)&report_t[1];
	char dst[BUFFER_LEN] = {0};
	int16_t idx;
	int32_t ret;
	int16_t i = 0;
	char *ptr = dst;
	(void)memset_s(dst, BUFFER_LEN, '=', BUFFER_LEN);
	for (idx = 0; idx < POSTURE_CNT; idx++) {
		if (posture_data[idx].happen_time > 0) {
			ret = snprintf_s(ptr, COPY_DEST_MAX, COPY_DEST_MAX - 1, "i:%d,s:%d,t:%lld,d:%d", i,
				posture_data[idx].posture_state, posture_data[idx].happen_time, posture_data[idx].duration);
			hwlog_info("%s: %d %d %lld %d \n", __func__, idx, posture_data[idx].posture_state,
				posture_data[idx].happen_time, posture_data[idx].duration);
			if (ret != -1) { // string len < COPY_DEST_MAX
				*(ptr + ret) = '=';
				ptr += (ret + 1);
			} else { // string len >= COPY_DEST_MAX
				*(ptr + COPY_DEST_MAX - 1) = '=';
				ptr += COPY_DEST_MAX;
			}
			i++;
		}
		if ((idx % POSTURE_BLOCK_CNT == 4) && (dst[0] != '=')) {
			*ptr = '\0';
			dst[BUFFER_LEN - 1] = '\0';
			HWDUBAI_LOGE("DUBAI_TAG_POSTURE_STATE", "source=%s", dst);
			hwlog_info("%s %s\n", __func__, dst);
			(void)memset_s(dst, BUFFER_LEN, '=', BUFFER_LEN);
			ptr = dst;
			i = 0;
		}
	}
}

#define SHAKE_ORIENT_BUFF_LEN 2
static void decode_shake_orient_for_dubai(const pkt_big_data_report_t *report_t)
{
	int16_t i;
	dubai_posture_t *posture_data = (dubai_posture_t *)&report_t[1];

	for (i = 0; i < SHAKE_ORIENT_BUFF_LEN; i++) {
		HWDUBAI_LOGE("DUBAI_TAG_POSTURE_STATE", "source=i:0,s:%d,t:%lld,d:%d=", posture_data[i].posture_state,
			posture_data[i].happen_time, posture_data[i].duration);
		hwlog_info("%s: %d %lld %d \n", __func__, posture_data[i].posture_state,
			posture_data[i].happen_time, posture_data[i].duration);
	}
}

static void iomcu_dubai_event_process(const pkt_big_data_report_t *report_t)
{
	uint32_t *data = NULL;

	data = (uint32_t *)&report_t[1];
	switch (report_t->event_id) {
	case DUBAI_EVENT_AOD_PICKUP:
		hwlog_info("DUBAI_EVENT_AOD_PICKUP: %d\n", data[0]);
		HWDUBAI_LOGE("DUBAI_TAG_TP_AOD", "type=%d data=%d", DUBAI_EVENT_AOD_PICKUP, data[0]);
		break;
	case DUBAI_EVENT_AOD_PICKUP_NO_FINGERDOWN:
		hwlog_info("DUBAI_EVENT_AOD_PICKUP_NO_FINGERDOWN: %d\n", data[0]);
		HWDUBAI_LOGE("DUBAI_TAG_TP_AOD", "type=%d data=%d", DUBAI_EVENT_AOD_PICKUP_NO_FINGERDOWN, data[0]);
		break;
	case DUBAI_EVENT_POSTURE_TIME:
		decode_posture_for_dubai(report_t);
		break;
	case DUBAI_EVENT_SHAKE_ORIENT_TIME:
		decode_shake_orient_for_dubai(report_t);
		break;
	case DUBAI_EVENT_ACTIVITY_RECORD:
		HWDUBAI_LOGE("DUBAI_TAG_AR_DURATION", "source=VEHICLE:%u,RIDING:%u,WALK_SLOW:%u,RUN_FAST:%u,STATIONARY:%u",
			data[VEHICLE], data[RIDING], data[WALK_SLOW], data[RUN_FAST], data[STATIONARY]);
		break;
	default:
		hwlog_info("iomcu_dubai_event_process no matched event id: %d\n", report_t->event_id);
		break;
	}
}

static void iomcu_big_data_event_process(const pkt_big_data_report_t *report_t)
{
	uint32_t *data = NULL;
	uint32_t data_len;

	data = (uint32_t *)&report_t[1];
	data_len = report_t->hd.length - (sizeof(pkt_big_data_report_t) - sizeof(struct pkt_header));
	switch (report_t->event_id) {
	case BIG_DATA_EVENT_MOTION_TYPE:
	case BIG_DATA_EVENT_DDR_INFO:
	case BIG_DATA_EVENT_TOF_PHONECALL:
	case BIG_DATA_EVENT_PHONECALL_SCREEN_STATUS:
	case BIG_DATA_EVENT_VIB_RESP_TIME:
	case BIG_DATA_FOLD_TEMP:
	case BIG_DATA_EVENT_AOD_INFO:
#ifdef CONFIG_HUAWEI_DSM
	case BIG_DATA_EVENT_BLPWM_USED_INFO:
	case BIG_DATA_EVENT_PS_SOUND_INFO:
	case BIG_DATA_EVENT_COVER_OFFSET_INFO:
		process_big_data(report_t->event_id, data);
		break;
	case BIG_DATA_EVENT_SYSLOAD_PERIOD:
	case BIG_DATA_EVENT_SYSLOAD_TRIGGER:
		process_sysload_big_data(report_t->event_id, data);
		break;
	case BIG_DATA_EVENT_CARCRASH_HAPPENED:
	case BIG_DATA_EVENT_CARCRASH_SUSPECTED:
		process_carcrash_big_data(report_t->event_id, data, data_len);
		break;
#elif (defined CONFIG_HUAWEI_OHOS_DSM)
		break;
	case BIG_DATA_EVENT_COVER_OFFSET_INFO:
		process_cover_big_data_ohos(report_t->event_id, data, data_len);
		break;
	case BIG_DATA_EVENT_CARCRASH_HAPPENED:
	case BIG_DATA_EVENT_CARCRASH_SUSPECTED:
		process_carcrash_big_data_ohos(report_t->event_id, data, data_len);
		break;
#endif
	case BIG_DATA_EVENT_ATHENA_STASTIC:
		process_athena_big_data(report_t->event_id, data, data_len);
		break;
	default:
		hwlog_info("iomcu_big_data_event_process no matched event id: %d\n", report_t->event_id);
		break;
	}
}

static int iomcu_big_data_process(const struct pkt_header *head)
{
	uint32_t *data = NULL;
	pkt_big_data_report_t *report_t = NULL;

	if (!head)
		return -1;

	report_t = (pkt_big_data_report_t *)head;
	data = (uint32_t *)&report_t[1];
	if (report_t->event_id <= DUBAI_EVENT_END)
		iomcu_dubai_event_process(report_t);
	else
		iomcu_big_data_event_process(report_t);

	return 0;
}
#ifdef CONFIG_LIBLINUX
static void ddr_request_inttostr(char **str_offset, int data)
{
	int offset;
	offset = snprintf_s(*str_offset, MAX_INT_STR_LEN + COMMA_SIZE,
		MAX_INT_STR_LEN + COMMA_SIZE - 1, "%d,", data);
	if (offset < 0) {
			hwlog_err("%s, offset %d, snprintf_s error\n", __func__, offset);
			return;
	}
	*str_offset += offset;
}

static void ddr_request_big_data_ohos(void)
{
	struct hiview_hisysevent *event = NULL;
	event = hisysevent_create("KERNEL_XPOWER", "DDR_REQ_STATISTICS", BEHAVIOR);
	char *usr_name = kzalloc(DDRREQ_DATA_SIZE * DDRREQ_DATA_TYPE, GFP_KERNEL);
	if (usr_name == NULL) {
		hwlog_err("%s usr_name allocate failed\n",  __func__);
		return ;
	}
	char *usr_count = usr_name + ARRAY_NUM * (MAX_INT_STR_LEN + COMMA_SIZE);
	char *pc = usr_count + ARRAY_NUM * (MAX_INT_STR_LEN + COMMA_SIZE);
	char *pc_count = pc + ARRAY_NUM * (MAX_INT_STR_LEN + COMMA_SIZE);
	char *ddr_time = pc_count + ARRAY_NUM * (MAX_INT_STR_LEN + COMMA_SIZE);
	int i, j, k;
	char *usr_name_offset = usr_name;
	char *usr_count_offset = usr_count;
	char *pc_offset = pc;
	char *pc_count_offset = pc_count;
	char *ddr_time_offset = ddr_time;
	for (i = 0; i < USR_NUM; i++) {
		for (j = 0; j < PC_NUM; j++) {
			ddr_request_inttostr(&usr_name_offset, (int)ddr_static_info.ddr_usr[i].usr_name);
			ddr_request_inttostr(&usr_count_offset, (int)ddr_static_info.ddr_usr[i].total_count);
			ddr_request_inttostr(&pc_offset, ddr_static_info.ddr_usr[i].pc1[j].pc_index);
			ddr_request_inttostr(&pc_count_offset, (int)ddr_static_info.ddr_usr[i].pc1[j].pc_num);
			ddr_request_inttostr(&ddr_time_offset, ddr_static_info.ddr_request_time);
			}
	}
	hisysevent_put_string(event, "USR_NAME", usr_name);
	hisysevent_put_string(event, "USR_COUNT", usr_count);
	hisysevent_put_string(event, "PC", pc);
	hisysevent_put_string(event, "PC_COUNT", pc_count);
	hisysevent_put_string(event, "DDR_TIME", ddr_time);
	hisysevent_write(event);
	hisysevent_destroy(&event);
	kfree(usr_name);
}
#endif

static void iomcu_dubai_ddr_log_fetch(void)
{
	hwlog_info("ddr_req_statis_work_func success\n");
	uint64_t begin_time, end_time;
	uint64_t interval_time = 0;
	if (ddr_req_statis_suspend_time == NULL || ddr_req_statis_resume_time == NULL)
		return;
	begin_time = (ddr_req_statis_suspend_time->tv_sec * THOUSAND) +
		(ddr_req_statis_suspend_time->tv_usec / THOUSAND);
	end_time = (ddr_req_statis_resume_time->tv_sec * THOUSAND) +
		(ddr_req_statis_resume_time->tv_usec / THOUSAND);
	if (end_time > begin_time)
		interval_time = end_time - begin_time;
	else
		return;
	hwlog_info("interval_time is %d\n", interval_time);
	memset_s(&ddr_static_info, sizeof(ddr_request_pack), 0, sizeof(ddr_request_pack));
	iomcu_dubai_log_fetch(DUBAI_EVENT_DDR_REQ_STATISTICS, &ddr_static_info,
		sizeof(ddr_request_pack));
	unsigned int i, j;
	unsigned int count_sum = 0;
	ddr_static_info.ddr_request_time = ddr_static_info.ddr_request_time / THOUSAND;
	for (i = 0; i < USR_NUM; i++)
		count_sum = count_sum + ddr_static_info.ddr_usr[i].total_count;
	if (((count_sum * THOUSAND / interval_time) >= DDR_REQ_THRESHOLD)
		&& interval_time >= SUSPEND_TIME_MIN) {
		/* interval_time is given by millisecond */	
#ifdef CONFIG_LIBLINUX
		ddr_request_big_data_ohos();
#else
		/* interval_time is given by millisecond */
		for (i = 0; i < USR_NUM; i++) {
			for (j = 0; j < PC_NUM; j++) {
				HWDUBAI_LOGE("DUBAI_TAG_DDR_REQ_STATISTICS", "index=%d usr=%d count=%d pc=%d pc_num=%d time=%d",
					i, ddr_static_info.ddr_usr[i].usr_name, ddr_static_info.ddr_usr[i].total_count,
					ddr_static_info.ddr_usr[i].pc1[j].pc_index, ddr_static_info.ddr_usr[i].pc1[j].pc_num,
					ddr_static_info.ddr_request_time);
			}
		}
#endif
	}
	ddr_req_statis_suspend_time = NULL;
	ddr_req_statis_resume_time = NULL;
}

static void ddr_req_statis_work_func(struct work_struct *work)
{
	hwlog_info("ddr_req_statis_work_func success\n");
	iomcu_dubai_ddr_log_fetch();
}

static int iomcu_big_data_init(void)
{
	if (is_sensorhub_disabled())
		return -1;

	register_mcu_event_notifier(TAG_BIG_DATA,
		CMD_BIG_DATA_SEND_TO_AP_RESP, iomcu_big_data_process);
	hwlog_info("iomcu_big_data_init success\n");
	ddr_req_statis_workqueue = create_singlethread_workqueue("ddr_req_statis_workqueue");
	INIT_WORK(&ddr_req_statis_work, ddr_req_statis_work_func);
	return 0;
}

void ddr_req_statis_que_work(void)
{
	if (is_sensorhub_disabled())
		return;

	queue_work(ddr_req_statis_workqueue, &ddr_req_statis_work);
	hwlog_info("ddr_req_statis_que_work success\n");
}

void set_resume_time(struct timeval *ddr_req_tv_end)
{
	ddr_req_statis_resume_time = ddr_req_tv_end;
}

void set_suspend_time(struct timeval *ddr_req_tv_begin)
{
	ddr_req_statis_suspend_time = ddr_req_tv_begin;
}

late_initcall_sync(iomcu_big_data_init);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("SensorHub big data channel");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
