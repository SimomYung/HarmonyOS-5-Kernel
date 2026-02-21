/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
 * Description: contexthub route source file
 * Author: DIVS_SENSORHUB
 * Create: 2012-05-29
 */

#include <contexthub_route.h>
#include <contexthub_route_plu.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/rtc.h>
#include <linux/time64.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/export.h>
#include <linux/notifier.h>
#include <linux/mtd/hisi_nve_interface.h>
#include <huawei_platform/log/imonitor.h>
#include <securec.h>
#include <platform_include/smart/linux/base/ap/protocol.h>
#include "sensor_config.h"
#include "sensor_detect.h"
#include "sensor_info.h"
#include "sensor_feima.h"
#include "sensor_sysfs.h"
#include <contexthub_drop_event.h>
#ifdef CONFIG_SPI_NOR_AGENT
#include "contexthub_pm.h"
#include <linux/delay.h>
#endif

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
#include <dsm/dsm_pub.h>
#endif

#ifdef CONFIG_HUAWEI_DSM
#include <log/hiview_hievent.h>
#elif (defined CONFIG_HUAWEI_OHOS_DSM)
#include <dfx/hiview_hisysevent.h>
#endif
drop_info_t sensor_drop_data;

static void register_sensor_drop_work_handler(struct work_struct *wk)
{
	hwlog_info("%s\n", __func__);
	int res = call_drop_blocking_notifiers(SENSOR_DROP, (void *)&sensor_drop_data);
	if ((res != NOTIFY_DONE) && (res != NOTIFY_OK))
		hwlog_err("drop notify failed, res %d\n", res);
}

static DECLARE_WORK(register_sensor_drop_work, register_sensor_drop_work_handler);
void schedule_drop_work(drop_info_t drop_data)
{
	sensor_drop_data = drop_data;
	schedule_work(&register_sensor_drop_work);
}

#if (defined CONFIG_HUAWEI_DSM) || (defined CONFIG_HUAWEI_OHOS_DSM)
void drop_fastboot_record(const pkt_drop_data_req_t *head)
{
	static int drop_record_num;
	static struct pkt_drop_fastboot_record_t drop_record;
	struct timespec64 time = {0};

	if (head->data.material != 1 || head->data.height <= DROP_FASTBOOT_RECORD)
		return;

#ifdef CONFIG_SPI_NOR_AGENT
	for (int wait_times = 0; wait_times <= MAX_WAIT_TIMES; wait_times++) {
		if (get_spi_nor_suspend_status() == true) {
			if (wait_times == MAX_WAIT_TIMES) {
				hwlog_info("spi_nor is supend after 3s do return\n");
				return;
			}
			hwlog_info("spi_nor is suspend do %d sleep 100ms\n", wait_times);
			msleep(WAIT_100MS);
		} else {
			hwlog_info("spi_nor is resume\n");
			break;
		}
	}
#endif

	read_calibrate_data_from_nv(DROP_NV_NUM, DROP_NV_SIZE, "DROP");
	if (memcpy_s(&drop_record, sizeof(struct pkt_drop_fastboot_record_t),
		user_info.nv_data, DROP_NV_SIZE) != EOK)
		hwlog_info("drop_fastboot_record copy nv fail\n");

	drop_record_num = drop_record.index;
	ktime_get_real_ts64(&time);
	drop_record.drop_info[drop_record_num].time = time.tv_sec;
	drop_record.drop_info[drop_record_num].height = head->data.height;
	hwlog_info("drop_fastboot_record time: %ld, height: %d index: %d\n",
		drop_record.drop_info[drop_record_num].time,
		drop_record.drop_info[drop_record_num].height,
		drop_record.index);

	drop_record_num++;
	if (drop_record_num >= DROP_FASTBOOT_NUM)
		drop_record_num = 0;

	drop_record.index = drop_record_num;
	if (write_calibrate_data_to_nv(DROP_NV_NUM,
		DROP_NV_SIZE, "DROP", (char *)&drop_record))
		hwlog_info("drop_fastboot_record write nv fail\n");
}
#endif

#ifdef CONFIG_HUAWEI_DSM
int process_drop_report(const pkt_drop_data_req_t *head)
{
	struct imonitor_eventobj *obj = NULL;
	int ret = 0;
	int yaw = 0;
	int shell = 0;
	int film = 0;

	if (!head) {
		hwlog_err("%s para error\n", __func__);
		return -1;
	}

	hwlog_info("Kernel get drop type %d, initial_speed %d, height %d, angle_pitch %d,"
		"angle_roll %d, impact %d posture: %d\n",
		head->data.type, head->data.initial_speed, head->data.height, head->data.angle_pitch,
		head->data.angle_roll, head->data.material, head->data.speed);
	schedule_drop_work(head->data);
	obj = imonitor_create_eventobj(SENSOR_DROP_IMONITOR_ID);
	if (!obj) {
		hwlog_err("%s imonitor_create_eventobj failed\n", __func__);
		return -1;
	}

	ret += imonitor_set_param_integer_v2(obj, "Type",
		(long)(head->data.type));
	ret += imonitor_set_param_integer_v2(obj, "InitSpeed",
		(long)(head->data.initial_speed));
	ret += imonitor_set_param_integer_v2(obj, "Height",
		(long)(head->data.height));
	ret += imonitor_set_param_integer_v2(obj, "Pitch",
		(long)(head->data.angle_pitch));
	ret += imonitor_set_param_integer_v2(obj, "Roll",
		(long)(head->data.angle_roll));
	ret += imonitor_set_param_integer_v2(obj, "Material",
		(long)(head->data.material));
	ret += imonitor_set_param_integer_v2(obj, "Speed",
		(long)(head->data.speed));

	ret += imonitor_set_param_integer_v2(obj, "Yaw", (long)(yaw));
	ret += imonitor_set_param_integer_v2(obj, "Shell", (long)(shell));
	ret += imonitor_set_param_integer_v2(obj, "Film", (long)(film));
	if (ret) {
		imonitor_destroy_eventobj(obj);
		hwlog_err("%s imonitor_set_param failed, ret %d\n", __func__, ret);
		return ret;
	}

	ret = imonitor_send_event(obj);
	if (ret < 0)
		hwlog_err("%s imonitor_send_event failed, ret %d\n", __func__, ret);

	imonitor_destroy_eventobj(obj);
	return ret;
}

const uint32_t g_raw_data_event_id[RAW_DATA_EVENTS_NUM] = {
	SENSOR_RAW_DATA_ACC1_IMONITOR_ID,
	SENSOR_RAW_DATA_ACC2_IMONITOR_ID,
	SENSOR_RAW_DATA_ACC3_IMONITOR_ID,
	SENSOR_RAW_DATA_GYRO1_IMONITOR_ID,
	SENSOR_RAW_DATA_GYRO2_IMONITOR_ID,
	SENSOR_RAW_DATA_GYRO3_IMONITOR_ID
};

static int sensor_raw_data_hievent_report(int32_t event_id, int8_t *raw_data, int32_t raw_data_size,
	int32_t *raw_data_offset)
{
	struct hiview_hievent *hi_event = NULL;
	int8_t param[RAW_DATA_PARAM_MAX_SIZE] = { 0 };
	int8_t value[24] = { 0 }; // 24: "18446744073709551615"(0xFFFFFFFFFFFFFFFF) string size
	int32_t i;
	int32_t ret = 0;
	uint8_t cycle_times;

	if (raw_data_size - *raw_data_offset >= RAW_DATA_EVENT_TABLE_SIZE)
		cycle_times = RAW_DATA_EVENT_KEY_NUM;
	else
		cycle_times = (raw_data_size - *raw_data_offset) / sizeof(uint64_t);

	hi_event = hiview_hievent_create(event_id);
	if (!hi_event) {
		hwlog_err("[sensorALG][%s] create hievent fail\n", __func__);
		return -1;
	}

	for (i = 0; i < cycle_times; i++) {
		int print_result = 0;
		print_result = sprintf_s(param, sizeof(param), "DATA%d", i + 1);
		if (print_result < 0)
			hwlog_err("snprintf_s failed,result%d\n", print_result);
		print_result = sprintf_s(value, sizeof(value), "%llu", *(uint64_t *)(raw_data + *raw_data_offset));
		if (print_result < 0)
			hwlog_err("snprintf_s failed,result%d\n", print_result);
		ret += hiview_hievent_put_string(hi_event, param, value);
		*raw_data_offset += sizeof(uint64_t);
	}

	if (ret) {
		hiview_hievent_destroy(hi_event);
		hwlog_err("[sensorALG][%s] hiview_hievent_put_integral failed, ret %d\n", __func__, ret);
		return ret;
	}

	ret = hiview_hievent_report(hi_event);
	if (ret < 0)
		hwlog_err("[sensorALG][%s] report fail ret=%d\n", __func__, ret);

	hiview_hievent_destroy(hi_event);

	return ret;
}

int sensor_raw_big_data_report(const struct pkt_header *head)
{
	int8_t *raw_data = NULL;
	int32_t raw_data_size;
	int32_t raw_data_offset = 0;
	int32_t i = 0;
	int32_t ret = 0;

	if (!head) {
		hwlog_err("%s para error\n", __func__);
		return -1;
	}

	raw_data = (char*)head + sizeof(pkt_common_data_t);
	raw_data_size = head->length - (sizeof(pkt_common_data_t) - sizeof(struct pkt_header));
	hwlog_info("[sensorALG][%s] head info: %d, %d, %d\n",
		__func__, head->length, sizeof(pkt_common_data_t), sizeof(struct pkt_header));

	for (i = 0; i < RAW_DATA_EVENTS_NUM; i++) {
		if (raw_data_size - raw_data_offset < sizeof(uint64_t))
			break;

		ret = sensor_raw_data_hievent_report(g_raw_data_event_id[i], raw_data, raw_data_size, &raw_data_offset);
		if (ret < 0)
			hwlog_err("[sensorALG][%s] envent_id %d report fail, ret=%d\n", __func__, g_raw_data_event_id[i], ret);

		hwlog_info("[sensorALG][%s] %d finished, total_size=%dByte, cur_offset=%dByte\n",
			__func__, g_raw_data_event_id[i], raw_data_size, raw_data_offset);
	}

	return ret;
}

#elif (defined CONFIG_HUAWEI_OHOS_DSM)
int process_drop_report_ohos(const pkt_drop_data_req_t *head)
{
	int ret = 0;
	int yaw = 0;
	int shell = 0;
	int film = 0;

	if (!head) {
		hwlog_err("%s para error\n", __func__);
		return -1;
	}

	hwlog_info("[sensorALG][process_drop_report_ohos]Kernel get drop type %d, initial_speed %d, height %d, angle_pitch %d,"
		"angle_roll %d, impact %d posture: %d\n",
		head->data.type, head->data.initial_speed, head->data.height, head->data.angle_pitch,
		head->data.angle_roll, head->data.material, head->data.speed);
	schedule_drop_work(head->data);
	struct hiview_hisysevent *event = NULL;
	event = hisysevent_create("SENSOR_DRV", "SENSOR_DROP_HAPPEN_INFO", FAULT);
	if (!event) {
		hwlog_err("[sensorALG] %s hisysevent_create failed\n", __func__);
		return -1;
	}

	ret = hisysevent_put_integer(event, "TYPE", (long long)(head->data.type));
	ret = hisysevent_put_integer(event, "INIT_SPEED", (long long)(head->data.initial_speed));
	ret = hisysevent_put_integer(event, "HEIGHT", (long long)(head->data.height));
	ret = hisysevent_put_integer(event, "PITCH", (long long)(head->data.angle_pitch));
	ret = hisysevent_put_integer(event, "ROLL", (long long)(head->data.angle_roll));
	ret = hisysevent_put_integer(event, "YAW", (long long)(yaw));
	ret = hisysevent_put_integer(event, "MATERIAL", (long long)(head->data.material));
	ret = hisysevent_put_integer(event, "SPEED", (long long)(head->data.speed));
	ret = hisysevent_put_integer(event, "SHELL", (long long)(shell));
	ret = hisysevent_put_integer(event, "FILM", (long long)(film));

	ret = hisysevent_write(event);
	hwlog_info("[sensorALG][process_drop_report_ohos] hisysevent write success\n");

hisysevent_end:
	hisysevent_destroy(&event);
	hwlog_err("[sensorALG][process_drop_report_ohos] hisysevent destroy success\n");
	return ret;
}
#endif

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
void dmd_modem_record(pkt_mode_dmd_log_report_req_t* pkt)
{
	hwlog_info("%s, dmd_case is %d dmd_id %d\n",
		__func__, pkt->dmd_case, pkt->dmd_id);

	dsm_client_record(shb_dclient, "modem_succ:%d,modem_valid:%d \n", pkt->resv1, pkt->resv2);
	dsm_client_notify(shb_dclient, pkt->dmd_id);
	eda_dmd_mode_info* list = pkt->sensorhub_dmd_mode_info;
	dsm_client_record(shb_dclient,"m0_id:%d,m0_valid:%d,m0_channel:%d,m0_tas:%d,m0_mas:%d,m0_trx:%d,\
		m0_0_rat:%d,m0_0_band:%d,m0_0_power:%d,m0_0_rsv:%d,\
		m0_1_rat:%d,m0_1_band:%d,m0_1_power:%d,m0_1_rsv:%d,\
		m0_2_rat:%d,m0_2_band:%d,m0_2_power:%d,m0_2_rsv:%d,\
		m0_3_rat:%d, m0_3_band:%d, m0_3_power:%d, m0_3_rsv:%d\n",
		list[0].modem_id, list[0].validflag, list[0].channel, list[0].tas_staus, list[0].mas_staus, list[0].trx_tas_staus,
		list[0].tx_info[0].rat, list[0].tx_info[0].band, list[0].tx_info[0].power, list[0].tx_info[0].rsv,
		list[0].tx_info[1].rat, list[0].tx_info[1].band, list[0].tx_info[1].power, list[0].tx_info[1].rsv,
		list[0].tx_info[2].rat, list[0].tx_info[2].band, list[0].tx_info[2].power, list[0].tx_info[2].rsv,
		list[0].tx_info[3].rat, list[0].tx_info[3].band, list[0].tx_info[3].power, list[0].tx_info[3].rsv
	);
	dsm_client_notify(shb_dclient, pkt->dmd_id);

	dsm_client_record(shb_dclient,"m1_id:%d,m1_valid:%d,m1_channel:%d,m1_tas:%d,m1_mas:%d,m1_trx:%d,\
		m1_0_rat:%d,m1_0_band:%d,m1_0_power:%d,m1_0_rsv:%d,\
		m1_1_rat:%d,m1_1_band:%d,m1_1_power:%d,m1_1_rsv:%d,\
		m1_2_rat:%d,m1_2_band:%d,m1_2_power:%d,m1_2_rsv:%d,\
		m1_3_rat:%d, m1_3_band:%d, m1_3_power:%d, m1_3_rsv:%d\n",
		list[1].modem_id, list[1].validflag, list[1].channel, list[1].tas_staus, list[1].mas_staus, list[1].trx_tas_staus,
		list[1].tx_info[0].rat, list[1].tx_info[0].band, list[1].tx_info[0].power, list[1].tx_info[0].rsv,
		list[1].tx_info[1].rat, list[1].tx_info[1].band, list[1].tx_info[1].power, list[1].tx_info[1].rsv,
		list[1].tx_info[2].rat, list[1].tx_info[2].band, list[1].tx_info[2].power, list[1].tx_info[2].rsv,
		list[1].tx_info[3].rat, list[1].tx_info[3].band, list[1].tx_info[3].power, list[1].tx_info[3].rsv
	);
	dsm_client_notify(shb_dclient, pkt->dmd_id);

	dsm_client_record(shb_dclient,"m2_id:%d,m2_valid:%d,m2_channel:%d,m2_tas:%d,m2_mas:%d,m2_trx:%d,\
		m2_0_rat:%d,m2_0_band:%d,m2_0_power:%d,m2_0_rsv:%d,\
		m2_1_rat:%d,m2_1_band:%d,m2_1_power:%d,m2_1_rsv:%d,\
		m2_2_rat:%d,m2_2_band:%d,m2_2_power:%d,m2_2_rsv:%d,\
		m2_3_rat:%d, m2_3_band:%d, m2_3_power:%d, m2_3_rsv:%d\n",
		list[2].modem_id, list[2].validflag, list[2].channel, list[2].tas_staus, list[2].mas_staus, list[2].trx_tas_staus,
		list[2].tx_info[0].rat, list[2].tx_info[0].band, list[2].tx_info[0].power, list[2].tx_info[0].rsv,
		list[2].tx_info[1].rat, list[2].tx_info[1].band, list[2].tx_info[1].power, list[2].tx_info[1].rsv,
		list[2].tx_info[2].rat, list[2].tx_info[2].band, list[2].tx_info[2].power, list[2].tx_info[2].rsv,
		list[2].tx_info[3].rat, list[2].tx_info[3].band, list[2].tx_info[3].power, list[2].tx_info[3].rsv
	);
	dsm_client_notify(shb_dclient, pkt->dmd_id);
}
#endif

void light_sensor_set_rst_gpio(struct device_node *dn)
{
	int temp;
	uint8_t lsensor_rst_level = 0;
	GPIO_NUM_TYPE lsensor_rst_gpio;

	if (dn == NULL)
		return;

	if (of_property_read_u32(dn, "lsensor_rst_gpio", &temp)) {
		hwlog_info("%s:read lsensor_rst_gpio fail\n", __func__);
		return;
	}
	lsensor_rst_gpio = (GPIO_NUM_TYPE)temp;
	if (of_property_read_u32(dn, "lsensor_rst_level", &temp))
		hwlog_info("%s:read lsensor_rst_level fail\n", __func__);
	else
		lsensor_rst_level = (uint8_t)temp;

	int ret = gpio_request(lsensor_rst_gpio, "lsensor_rst_gpio");
	if (ret < 0)
		hwlog_info("%s, fail request lsensor_rst_gpio = %d\n", __func__, ret);

	int gpio_ori_val = gpio_get_value(lsensor_rst_gpio);
	if (lsensor_rst_level == gpio_ori_val) {
		hwlog_info("%s lsensor_rst_gpio %d value is equal: %d\n",
			__func__, lsensor_rst_gpio, gpio_ori_val);
		return;
	}
	hwlog_info("%s lsensor_rst_gpio %d lsensor_rst_gpio value: %d\n",
		__func__, lsensor_rst_gpio, gpio_ori_val);
	gpio_direction_output(lsensor_rst_gpio, lsensor_rst_level); // 1:high, 0:low
	gpio_ori_val = gpio_get_value(lsensor_rst_gpio);
	hwlog_info("%s lsensor_rst_gpio1 value : %d\n", __func__, gpio_ori_val);
	msleep(4);
}
