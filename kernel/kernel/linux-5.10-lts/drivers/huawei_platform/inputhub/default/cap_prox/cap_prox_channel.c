/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: cap prox channel source file
 * Author: linjianpeng <linjianpeng1@huawei.com>
 * Create: 2020-05-25
 */

#include "cap_prox_channel.h"

#include <linux/err.h>
#include <linux/mtd/hisi_nve_interface.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <securec.h>

#include "cap_prox_sysfs.h"
#include "contexthub_route.h"

int send_cap_prox1_calibrate_data_to_mcu(void)
{
	struct cap_prox_device_info *dev_info = NULL;

	dev_info = cap_prox_get_device_info(TAG_CAP_PROX1);
	if (!dev_info)
		return 0;
	if (strlen(get_sensor_chip_info_address(CAP_PROX1)) == 0) {
		hwlog_info("is not overseas phone,cap_prox sensor is not in board\n");
		return 0;
	}

	if (read_calibrate_data_from_nv(CAP_PROX1_CALIDATA_NV_NUM,
		CAP_PROX1_CALIDATA_NV_SIZE, CAP_PROX1_NV_NAME))
		return -1;

	/* send to mcu */
	if (memcpy_s(&dev_info->sar_calibrate_datas,
		sizeof(dev_info->sar_calibrate_datas),
		user_info.nv_data, sizeof(dev_info->sar_calibrate_datas)) != EOK)
		return -1;
	if (send_calibrate_data_to_mcu(TAG_CAP_PROX1, SUB_CMD_SET_OFFSET_REQ,
		(const void *)&dev_info->sar_calibrate_datas,
		sizeof(dev_info->sar_calibrate_datas), false))
		return -1;
	return 0;
}

int send_cap_prox_calibrate_data_to_mcu(void)
{
	struct cap_prox_device_info *dev_info = NULL;
	char *p_cover_cali_data = NULL;

	dev_info = cap_prox_get_device_info(TAG_CAP_PROX);
	if (!dev_info)
		return 0;
	if (strlen(get_sensor_chip_info_address(CAP_PROX)) == 0) {
		hwlog_info("is not overseas phone,cap_prox sensor is not in board\n");
		return 0;
	}

	if (read_calibrate_data_from_nv(CAP_PROX_CALIDATA_NV_NUM,
		CAP_PROX_CALIDATA_NV_SIZE, "Csensor"))
		return -1;

	// initialize g_cover_cali_data
	p_cover_cali_data = get_cover_cali_data();
	if (p_cover_cali_data != NULL) {
		if (memcpy_s(p_cover_cali_data, MAX_CAP_PROX_CALIBRATE_DATA_LENGTH,
			user_info.nv_data, MAX_CAP_PROX_CALIBRATE_DATA_LENGTH) != EOK)
			return -1;
	}

	if (memcpy_s(&dev_info->sar_calibrate_datas,
		sizeof(dev_info->sar_calibrate_datas),
		user_info.nv_data, sizeof(dev_info->sar_calibrate_datas)) != EOK)
		return -1;

	hwlog_info("cap_prox :offset1=%d offset2=%d diff1=%d diff2=%d len:%ld\n",
		dev_info->sar_calibrate_datas.offset[CAP_PROX_PHASE0],
		dev_info->sar_calibrate_datas.offset[CAP_PROX_PHASE1],
		dev_info->sar_calibrate_datas.diff[CAP_PROX_PHASE0],
		dev_info->sar_calibrate_datas.diff[CAP_PROX_PHASE1],
		sizeof(dev_info->sar_calibrate_datas));

	if (send_calibrate_data_to_mcu(TAG_CAP_PROX, SUB_CMD_SET_OFFSET_REQ,
		(const void *)&dev_info->sar_calibrate_datas,
		sizeof(dev_info->sar_calibrate_datas), false))
		return -1;
	return 0;
}

void get_cover_calibrate_data(void)
{
	struct cover_nv_data_t cover_nv_data;
	static uint8_t cover_first_start_flag;
	if(cover_first_start_flag == 0) {
		read_calibrate_data_from_nv(COVER_CALIDATA_NV_NUM, COVER_CALIDATA_NV_SIZE, "RSV471");
		if (memcpy_s(&cover_nv_data, sizeof(struct cover_nv_data_t), user_info.nv_data,
				sizeof(struct cover_nv_data_t)) != EOK){
					hwlog_info("get_cover_calibrate_data nv memcpy_s fail\n");
					return;
				}
		if (send_calibrate_data_to_mcu(TAG_COVER, SUB_CMD_SET_OFFSET_REQ,
				(const void *)&cover_nv_data, sizeof(struct cover_nv_data_t), false)) {
					hwlog_info("get_cover_calibrate_data send_calibrate_data_to_mcu fail\n");
					return;
			}
		cover_first_start_flag = 1;
	}
	return;
}

void reset_cap_prox_calibrate_data(void)
{
	struct cap_prox_device_info *dev_info = NULL;

	dev_info = cap_prox_get_device_info(TAG_CAP_PROX);
	if (!dev_info)
		return;
	if (strlen(get_sensor_chip_info_address(CAP_PROX)))
		send_calibrate_data_to_mcu(TAG_CAP_PROX,
			SUB_CMD_SET_OFFSET_REQ, &dev_info->sar_calibrate_datas,
			sizeof(dev_info->sar_calibrate_datas), true);
}

void reset_cap_prox1_calibrate_data(void)
{
	struct cap_prox_device_info *dev_info = NULL;

	dev_info = cap_prox_get_device_info(TAG_CAP_PROX1);
	if (!dev_info)
		return;
	if (strlen(get_sensor_chip_info_address(CAP_PROX1)))
		send_calibrate_data_to_mcu(TAG_CAP_PROX1,
			SUB_CMD_SET_OFFSET_REQ, &dev_info->sar_calibrate_datas,
			sizeof(dev_info->sar_calibrate_datas), true);
}

int write_cap_prox_calibrate_data_to_nv(const char *src, int length)
{
	hwlog_info("enter write_cap_prox_calibrate_data_to_nv\n");
	if (!src) {
		hwlog_err("write_cap_prox_calibrate_data_to_nv fail, invalid para\n");
		return -1;
	}
	if (write_calibrate_data_to_nv(CAP_PROX_CALIDATA_NV_NUM, length, "Csensor", src))
		return -1;
	hwlog_info("write_cap_prox_calibrate_data_to_nv suc len=%d\n", length);
	return 0;
}

int write_cover_calibrate_data_to_nv(const char *src, int length)
{
	hwlog_info("enter write_cover_calibrate_data_to_nv\n");
	if (!src) {
		hwlog_err("write_cover_calibrate_data_to_nv fail, invalid para\n");
		return -1;
	}
	if (write_calibrate_data_to_nv(COVER_CALIDATA_NV_NUM, COVER_CALIDATA_NV_SIZE,
			"RSV471", src));
		return -1;
	hwlog_info("write_cover_calibrate_data_to_nv suc len=%d\n", length);
	return 0;
}