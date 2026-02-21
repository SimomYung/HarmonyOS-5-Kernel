/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.

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
#include <linux/kernel.h>
#include <securec.h>
#include <dsm/dsm_pub.h>
#include "external_camera_dsm.h"

#define cam_dsm_err(fmt, args...) \
	do { \
		printk(KERN_ERR "[external_camera_DSM ERR] %s: %d " fmt, __func__, __LINE__, ## args); \
	} while (0)

#define cam_dsm_info(fmt, args...) \
	do { \
		printk(KERN_INFO "[external_camera_DSM INFO] %s: %d " fmt, __func__, __LINE__, ## args); \
	} while (0)

static struct dsm_client *g_dsm_client_extcam = NULL;

#ifdef CONFIG_HISYSEVENT
struct error_no_map {
	int error_no;
	char *name;
};

static struct error_no_map g_extcam_error_no_map[] = {
	{DSM_FLASH_OPEN_SHOTR_ERROR_NO, "CAMERA_FLASH_SHORT_OR_CIRCUIT"},
};

static int extcam_errorno_to_str(int error_no, char *str, int buff_len)
{
	int i;
	int extcam_error_no_map_len = sizeof(g_extcam_error_no_map) / sizeof(struct error_no_map);

	if (str == NULL) {
		cam_dsm_err("[E]str is null\n");
		return -EINVAL;
	}

	for (i = 0; i < extcam_error_no_map_len; i++) {
		if (g_extcam_error_no_map[i].error_no == error_no) {
			if (strlen(g_extcam_error_no_map[i].name) >= buff_len) {
				cam_dsm_err("[E]length error\n");
				return -EINVAL;
			}
			if (strncpy_s(str, buff_len, g_extcam_error_no_map[i].name, strlen(g_extcam_error_no_map[i].name)) != EOK) {
				cam_dsm_err("[E]cpy error\n");
				return -EINVAL;
			}
			str[buff_len - 1] = '\0';
			cam_dsm_info("[I]dsm extcam_errorno_to_str str=%s\n", str);
			return EOK;
		}
	}

	return -EINVAL;
}

static struct dsm_client_ops hisi_dsm_ops_extcam = {
	.poll_state = NULL,
	.dump_func = NULL,
	.errorno_to_str = extcam_errorno_to_str,
};
#endif

#define LOG_BUF_SIZE	512
static struct dsm_dev dev_extcam = {
 #ifdef CONFIG_HISYSEVENT
	.name = "CAM_DRV",
 #else
	.name = "dsm_extcam",
 #endif
	.device_name = "UVC video",
	.ic_name = "External camera",
	.module_name = NULL,
 #ifdef CONFIG_HISYSEVENT
     .fops = &hisi_dsm_ops_extcam,
 #else
	.fops = NULL,
 #endif
	.buff_size = LOG_BUF_SIZE,
};

void external_camera_register_dsm_client(void)
{
	if (g_dsm_client_extcam == NULL) {
		cam_dsm_info("extcam_register_dsm_client enter");
		g_dsm_client_extcam = dsm_register_client(&dev_extcam);
	}
}

void external_camera_unregister_dsm_client(void)
{
	if (g_dsm_client_extcam) {
		cam_dsm_info("extcam_unregister_dsm_client enter");
		dsm_unregister_client(g_dsm_client_extcam, &dev_extcam);
		g_dsm_client_extcam = NULL;
	}
}

void external_camera_dsm_client_notify(int dsm_id, const char *fmt, ...)
{
	va_list ap;
	int size, retry = 2;
	char buf[LOG_BUF_SIZE] = {0};

	cam_dsm_info("Enter");
	if (!g_dsm_client_extcam) {
		cam_dsm_err("g_dsm_client_extcam is null ptr");
		return;
	}

	if (fmt != NULL) {
		va_start(ap, fmt);
		size = vsnprintf_s(buf, LOG_BUF_SIZE, LOG_BUF_SIZE-1, fmt, ap);
		va_end(ap);
		if (size < 0) {
			cam_dsm_err("buf copy failed");
			return;
		}
	}

	while (retry > 0) {
		if (!dsm_client_ocuppy(g_dsm_client_extcam)) {
			dsm_client_record(g_dsm_client_extcam, buf);
			dsm_client_notify(g_dsm_client_extcam, dsm_id);
			cam_dsm_info("external_camera report dmd success: %d",dsm_id);
			break;
		}
		dsm_client_unocuppy(g_dsm_client_extcam);
		retry--;
	}
	if (retry == 0) {
		cam_dsm_err("dsm client ocuppy, dsm notify failed, dsm_id=%d", dsm_id);
	}
}
