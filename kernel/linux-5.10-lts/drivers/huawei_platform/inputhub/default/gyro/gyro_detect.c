/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: gyro detect source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include "gyro_detect.h"

#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/mtd/hisi_nve_interface.h>

#include <securec.h>

#include "contexthub_route.h"
#include "gyro_sysfs.h"

#define GYRO_DEVICE_ID_0 0
#define GYRO_DEVICE_ID_1 1
#define GYRO_DEVICE_ID_2 2
#define GYRO_DEFAULT_RANGE 2000
#define GYR_SENSOR_DEFAULT_ID "4"

static char *gyro_sensor_id = GYR_SENSOR_DEFAULT_ID;
static int g_do_ois_offset_in_ui = 0;
static uint64_t g_gyro_axis_data = 0;
static struct gyro_sect_nv_t gyro_sect_nv[GYRO_DEV_COUNT_MAX] = {
    {GYRO_NV_NAME, GYRO_OFFSET_NV_SIZE, GYRO_OFFSET_NV_NUM,
		GYRO_ONLINE_OFFSET},
    {GYRO1_NV_NAME, GYRO1_OFFSET_NV_SIZE, GYRO1_OFFSET_NV_NUM,
		GYRO1_ONLINE_OFFSET},
    {GYRO2_NV_NAME, GYRO2_OFFSET_NV_SIZE, GYRO2_OFFSET_NV_NUM,
		GYRO2_ONLINE_OFFSET},
};

int get_do_ois_offset_in_ui_val(void)
{
	return g_do_ois_offset_in_ui;
}

uint64_t get_gyro_axis_data_val(void)
{
	return g_gyro_axis_data;
}

static struct gyro_platform_data gyro_data[GYRO_DEV_COUNT_MAX] = {
	{
	.cfg = DEF_SENSOR_COM_SETTING,
	.poll_interval = 10, /* inteval 10 ms */
	.position = 1, /* gyro position for OIS */
	.axis_map_x = 1,
	.axis_map_y = 0,
	.axis_map_z = 2, /* index 2 */
	.negate_x = 1,
	.negate_y = 0,
	.negate_z = 1,
	.gpio_int1 = 210, /* gpio 210 */
	.gpio_int2 = 0,
	.gpio_int2_sh = 0,
	.fac_fix_offset_y = 100, /* 100 times than real value */
	.still_calibrate_threshold = 5, /* 5 dps */
	.calibrate_way = 0,
	.calibrate_thredhold = 572, /* 572 dps */
	.gyro_range = GYRO_DEFAULT_RANGE,
	},
};
static struct gyro_device_info g_gyro_dev_info[GYRO_DEV_COUNT_MAX];
struct gyro_platform_data *gyro_get_platform_data(int32_t tag)
{
	if (tag == TAG_GYRO)
		return (&(gyro_data[GYRO_DEVICE_ID_0]));
	else if (tag == TAG_GYRO1)
		return (&(gyro_data[GYRO_DEVICE_ID_1]));
#ifdef CONFIG_INPUTHUB_30
	else if (tag == TAG_GYRO2)
		return (&(gyro_data[GYRO_DEVICE_ID_2]));
#endif
	hwlog_info("%s error, please check tag %d\n", __func__, tag);
	return NULL;
}

struct gyro_device_info *gyro_get_device_info(int32_t tag)
{
	if (tag == TAG_GYRO)
		return (&(g_gyro_dev_info[GYRO_DEVICE_ID_0]));
	else if (tag == TAG_GYRO1)
		return (&(g_gyro_dev_info[GYRO_DEVICE_ID_1]));
#ifdef CONFIG_INPUTHUB_30
	else if (tag == TAG_GYRO2)
		return (&(g_gyro_dev_info[GYRO_DEVICE_ID_2]));
#endif
	hwlog_info("%s error, please check tag %d\n", __func__, tag);
	return NULL;
}

char *gyro_get_sensors_id_string(void)
{
	return gyro_sensor_id;
}

void gyro_get_sensors_id_from_dts(struct device_node *dn)
{
	if (dn == NULL)
		return;
	if (of_property_read_string(dn,
		"gyro_sensor_id", (const char **)&gyro_sensor_id)) {
		gyro_sensor_id = GYR_SENSOR_DEFAULT_ID;
		hwlog_err("gyro_sensor_id err.\n");
	}
	hwlog_info("gyro_sensor_id:%s\n", gyro_sensor_id);
}

static void read_gyro_config1_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm)
{
	struct gyro_platform_data *pf_data =
		(struct gyro_platform_data *)sm->spara;
	int temp = 0;

	if (of_property_read_u32(dn, "poll_interval", &temp))
		hwlog_err("%s:read %s mag poll_interval fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->poll_interval = (uint16_t)temp;

	if (of_property_read_u32(dn, "position", &temp)) {
		hwlog_err("%s:read %s gyro position fail,use default position =%d\n",
			__func__, sm->sensor_name_str, pf_data->position);
	} else {
		pf_data->position = (uint8_t)temp;
		hwlog_info("%s:read %s gyro position suc position=%d\n",
			__func__, sm->sensor_name_str, pf_data->position);
	}

	if (of_property_read_u32(dn, "axis_map_x", &temp))
		hwlog_err("%s:read %s gyro axis_map_x fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->axis_map_x = (uint8_t)temp;

	if (of_property_read_u32(dn, "axis_map_y", &temp))
		hwlog_err("%s:read %s gyro axis_map_y fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->axis_map_y = (uint8_t)temp;

	if (of_property_read_u32(dn, "axis_map_z", &temp))
		hwlog_err("%s:read %s gyro axis_map_z fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->axis_map_z = (uint8_t)temp;

	if (of_property_read_u32(dn, "negate_x", &temp))
		hwlog_err("%s:read %s gyro negate_x fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->negate_x = (uint8_t)temp;

	if (of_property_read_u32(dn, "negate_y", &temp))
		hwlog_err("%s:read %s gyro negate_y fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->negate_y = (uint8_t)temp;

	if (of_property_read_u32(dn, "negate_z", &temp))
		hwlog_err("%s:read %s gyro negate_z fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->negate_z = (uint8_t)temp;
}

static void read_gyro_config2_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm)
{
	struct gyro_platform_data *pf_data =
		(struct gyro_platform_data *)sm->spara;
	int temp = 0;
	int aux = 0;

	if (of_property_read_u32(dn, "still_calibrate_threshold", &temp))
		hwlog_err("%s:read %s gyro still_calibrate_threshold fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->still_calibrate_threshold = (uint8_t)temp;

	if (of_property_read_u32(dn, "aux_prop", &aux))
		hwlog_err("%s:read %s aux_prop fail\n",
			__func__, sm->sensor_name_str);
	else
		hwlog_info("%s:read %s aux_prop %d\n",
			__func__, sm->sensor_name_str, aux);

	if (of_property_read_u32(dn, "file_id", &temp))
		hwlog_err("%s:read %s file_id fail\n",
			__func__, sm->sensor_name_str);
	else if (aux)
		read_aux_file_list(temp, sm->tag);
	else
		read_dyn_file_list((uint16_t)temp);

	if (of_property_read_u32(dn, "sensor_list_info_id", &temp))
		hwlog_err("%s:read %s gyro sensor_list_info_id fail\n",
			__func__, sm->sensor_name_str);
	else
		add_sensor_list_info_id((uint16_t)temp);

	temp = of_get_named_gpio(dn, "gpio_int1", 0);
	if (temp < 0)
		hwlog_err("%s:read %s gpio_int1 fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->gpio_int1 = (GPIO_NUM_TYPE)temp;

	if (of_property_read_u32(dn, "fac_fix_offset_Y", &temp)) {
		hwlog_info("%s:read %s fac_fix_offset_Y fail\n",
			__func__, sm->sensor_name_str);
	} else {
		pf_data->fac_fix_offset_y = (uint8_t)temp;
		hwlog_debug("%s:read %s acc fac_fix_offset_y %d\n",
			__func__, sm->sensor_name_str, temp);
	}
	if (of_property_read_u32(dn, "calibrate_way", &temp))
		hwlog_err("%s:read %s gyro calibrate_way fail\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->calibrate_way = (uint8_t)temp;
}

static void read_gyro_config3_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm)
{
	struct gyro_platform_data *pf_data =
		(struct gyro_platform_data *)sm->spara;
	const char *bus_type = NULL;
	int temp = 0;

	if (of_property_read_u32(dn, "calibrate_thredhold", &temp)) {
		hwlog_info("%s:read %s calibrate_thredhold fail\n",
			__func__, sm->sensor_name_str);
	} else {
		pf_data->calibrate_thredhold = (uint16_t)temp;
		hwlog_debug("%s:read %s gyro calibrate_thredhold %d\n",
			__func__, sm->sensor_name_str, temp);
	}

	if (of_property_read_u32(dn, "gyro_range", &temp)) {
		hwlog_debug("%s:read %s gyro_range fail\n",
			__func__, sm->sensor_name_str);
	} else {
		pf_data->gyro_range = (uint16_t)temp;
		hwlog_info("%s:read %s gyro gyro_range %d\n",
			__func__, sm->sensor_name_str, temp);
	}

	if (of_property_read_string(dn, "bus_type", &bus_type)) {
		hwlog_warn("%s:gyro %s bus_type not configured\n",
			__func__, sm->sensor_name_str);
	} else {
		if (get_combo_bus_tag(bus_type, (uint8_t *)&temp))
			hwlog_warn("%s:gyro %s bus_type invalid\n",
				__func__, sm->sensor_name_str);
		else
			pf_data->cfg.bus_type = temp;
	}

	if (of_property_read_u32(dn, "bus_number", &temp))
		hwlog_warn("%s:gyro %s bus_number not configured\n",
			__func__, sm->sensor_name_str);
	else
		pf_data->cfg.bus_num = temp;

	if (pf_data->cfg.bus_type == TAG_I2C ||
		pf_data->cfg.bus_type == TAG_I3C) {
		if (of_property_read_u32(dn, "i2c_address", &temp))
			hwlog_warn("%s:gyro _address not configured\n",
				__func__);
		else
			pf_data->cfg.i2c_address = temp;
	}
}

#ifdef CONFIG_INPUTHUB_30
static void read_gyro_config4_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm)
{
	struct gyro_platform_data *pf_data =
		(struct gyro_platform_data *)sm->spara;
	const char *bus_type = NULL;
	int temp = 0;

	if (of_property_read_u32(dn, "do_ois_offset_in_ui", &temp)) {
		hwlog_info("%s:read %s do_ois_offset_in_ui fail\n",
			__func__, sm->sensor_name_str);
	} else {
		if (temp == 1)
			g_do_ois_offset_in_ui = 1;
		pf_data->do_ois_offset_in_ui = temp;
		hwlog_info("%s:read %s gyro do_ois_offset_in_ui %d\n",
			__func__, sm->sensor_name_str, temp);
	}
}
#endif

#define GYRO_AXIS_Y_BIT_POS	2
#define GYRO_AXIS_Z_BIT_POS	4
#define GYRO_NEGATE_Z_BIT_POS	2
#define GYRO_AXIS_BIT_POS	32
 
static void set_gyro_axis_data_val(
	struct sensor_detect_manager *sm)
{
	struct gyro_platform_data *pf_data =
		(struct gyro_platform_data *)sm->spara;

	if (sm->sensor_id != GYRO)
		return;

	uint64_t axis_map = 0;
	uint64_t negate = 0;
	axis_map |= (uint64_t)(pf_data->axis_map_x);
	axis_map |= (uint64_t)(pf_data->axis_map_y) << GYRO_AXIS_Y_BIT_POS;
	axis_map |= (uint64_t)(pf_data->axis_map_z) << GYRO_AXIS_Z_BIT_POS;
	negate |= (uint64_t)(pf_data->negate_x);
	negate |= (uint64_t)(pf_data->negate_y) << 1;
	negate |= (uint64_t)(pf_data->negate_z) << GYRO_NEGATE_Z_BIT_POS;
	g_gyro_axis_data = ((axis_map << GYRO_AXIS_BIT_POS) | negate);
	hwlog_info("set gyro_axis_data %llu\n", g_gyro_axis_data);
}

void read_gyro_data_from_dts(struct device_node *dn,
	struct sensor_detect_manager *sm)
{
	read_chip_info(dn, sm->sensor_id);
	read_gyro_config1_from_dts(dn, sm);
	read_gyro_config2_from_dts(dn, sm);
	read_gyro_config3_from_dts(dn, sm);
#ifdef CONFIG_INPUTHUB_30
	read_gyro_config4_from_dts(dn, sm);
#endif
	read_sensorlist_info(dn, sm->sensor_id);
	set_gyro_axis_data_val(sm);
}

static int read_gyro_calibrate_data_from_nv(int nv_number, int nv_size,
	const char *nv_name, int nv_name_size, char *data_tmp)
{
    int ret;
    struct opt_nve_info_user gyro_user_info = {0};

    gyro_user_info.nv_operation = NV_READ_TAG;
    gyro_user_info.nv_number = nv_number;
    gyro_user_info.valid_size = nv_size;
    if (strncpy_s(gyro_user_info.nv_name, sizeof(gyro_user_info.nv_name),
		nv_name, nv_name_size - 1))
		return -1;
    ret = nve_direct_access_interface(&gyro_user_info);
    if (ret != 0) {
        hwlog_err("gyro_nve_direct_access_interface read %d error %d\n",
			nv_number, ret);
        return -1;
    }
    if (memcpy_s(data_tmp, nv_size, gyro_user_info.nv_data, nv_size))
        return -1;
    return 0;
}

static int mask_gyro_online_calibrate_data(const char *cali_data,
	char *data_tmp, int len, int nv_id)
{
	int online_offset_idx = gyro_sect_nv[nv_id].online_offset;
	if (len != gyro_sect_nv[nv_id].nv_size)
		return -1;
	if (read_gyro_calibrate_data_from_nv(gyro_sect_nv[nv_id].nv_num, len,
		gyro_sect_nv[nv_id].nv_name, sizeof(gyro_sect_nv[nv_id].nv_name),
		data_tmp))
		return -1;
	if (memcpy_s(&(data_tmp[online_offset_idx]), GYRO_ONLINE_OFFSET_LEN,
		&(cali_data[online_offset_idx]), GYRO_ONLINE_OFFSET_LEN))
		return -1;
	hwlog_info("%s succ\n", __func__);
	return 0;
}

int gyro2_data_from_mcu(const struct pkt_header *head)
{
	char gyro2_calibrate_data_tmp[GYRO2_OFFSET_NV_SIZE] = {0};

	switch (((pkt_gyro2_calibrate_data_req_t *)head)->subcmd) {
	case SUB_CMD_SELFCALI_REQ:
		if (mask_gyro_online_calibrate_data(
			((pkt_gyro_temp_offset_req_t *)head)->calibrate_data,
			gyro2_calibrate_data_tmp, sizeof(gyro2_calibrate_data_tmp),
			GYRO_DEVICE_ID_2))
			return -1;
		return write_gyro2_sensor_offset_to_nv(gyro2_calibrate_data_tmp,
			sizeof(gyro2_calibrate_data_tmp));
	default:
		hwlog_err("uncorrect subcmd 0x%x\n",
			((pkt_gyro2_calibrate_data_req_t *)head)->subcmd);
	}
	return 0;
}

int gyro1_data_from_mcu(const struct pkt_header *head)
{
	char gyro1_calibrate_data_tmp[GYRO1_OFFSET_NV_SIZE] = {0};

	switch (((pkt_gyro1_calibrate_data_req_t *)head)->subcmd) {
	case SUB_CMD_SELFCALI_REQ:
		if (mask_gyro_online_calibrate_data(
			((pkt_gyro_temp_offset_req_t *)head)->calibrate_data,
			gyro1_calibrate_data_tmp, sizeof(gyro1_calibrate_data_tmp),
			GYRO_DEVICE_ID_1))
			return -1;
		return write_gyro1_sensor_offset_to_nv(gyro1_calibrate_data_tmp,
			sizeof(gyro1_calibrate_data_tmp));
	default:
		hwlog_err("uncorrect subcmd 0x%x\n",
			((pkt_gyro1_calibrate_data_req_t *)head)->subcmd);
	}
	return 0;
}

int gyro_data_from_mcu(const struct pkt_header *head)
{
	char gyro_calibrate_data_tmp[GYRO_OFFSET_NV_SIZE] = {0};

	switch (((pkt_gyro_calibrate_data_req_t *)head)->subcmd) {
	case SUB_CMD_SELFCALI_REQ:
		if (mask_gyro_online_calibrate_data(
			((pkt_gyro_temp_offset_req_t *)head)->calibrate_data,
			gyro_calibrate_data_tmp, sizeof(gyro_calibrate_data_tmp),
			GYRO_DEVICE_ID_0))
			return -1;
		return write_gyro_sensor_offset_to_nv(gyro_calibrate_data_tmp,
			sizeof(gyro_calibrate_data_tmp));
	case SUB_CMD_GYRO_TMP_OFFSET_REQ:
		return write_gyro_temperature_offset_to_nv(
			((pkt_gyro_temp_offset_req_t *)head)->calibrate_data,
			(head->length - SUBCMD_LEN));
#ifdef CONFIG_INPUTHUB_30
	case SUB_CMD_AG_2_0_SAVE_TEMP_DATA_REQ:
		return save_gyro_2_0_temperature_data(
			((pkt_gyro_2_0_gyro_data_req_t *)head)->gyro_2_0_gyro_data,
			(head->length - SUBCMD_LEN));
#endif
	default:
		hwlog_err("uncorrect subcmd 0x%x\n",
			((pkt_gyro_calibrate_data_req_t *)head)->subcmd);
	}
	return 0;
}

void gyro_detect_init(struct sensor_detect_manager *sm, uint32_t len)
{
	struct sensor_detect_manager *p = NULL;
	uint8_t i;

	if (len <= GYRO1) {
		hwlog_err("%s:len=%d\n", __func__, len);
		return;
	}
	g_gyro_dev_info[GYRO_DEVICE_ID_0].obj_tag = TAG_GYRO;
	g_gyro_dev_info[GYRO_DEVICE_ID_1].obj_tag = TAG_GYRO1;
	g_gyro_dev_info[GYRO_DEVICE_ID_2].obj_tag = TAG_GYRO2;
	g_gyro_dev_info[GYRO_DEVICE_ID_0].detect_list_id = GYRO;
	g_gyro_dev_info[GYRO_DEVICE_ID_1].detect_list_id = GYRO1;
	g_gyro_dev_info[GYRO_DEVICE_ID_2].detect_list_id = GYRO2;
	for (i = 0; i < GYRO_DEV_COUNT_MAX; i++) {
		g_gyro_dev_info[i].gyro_dev_index = i;
		p = sm + g_gyro_dev_info[i].detect_list_id;
		p->spara = (const void *)&(gyro_data[i]);
		p->cfg_data_length = sizeof(gyro_data[i]);
		if (i == 0)
			continue;
		if (memcpy_s(&(gyro_data[i]), sizeof(gyro_data[i]),
			&(gyro_data[0]), sizeof(gyro_data[0])) != EOK)
			hwlog_info("%s: memcpy_s error i=%d\n", __func__, i);
	}
}

