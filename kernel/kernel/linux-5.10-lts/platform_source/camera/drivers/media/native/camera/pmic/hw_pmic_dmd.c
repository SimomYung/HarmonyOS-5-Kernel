/*
 * hw_pmic_dmd.c
 *
 * pmic dmd driver source file
 *
 * Copyright (C) 2024-2024 Huawei Technology Co., Ltd.
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/slab.h>
#include <log/hiview_hievent.h>
#include <securec.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>

#include "hw_pmic_dmd.h"

#define PMIC_DSM_BUF_SIZE_1024 1024
#define PMIC_DMD_INFO_LENGTH 50

static struct dsm_client *g_pmic_dsm_client;

typedef struct errno_string_map {
	int error_no;
	char *name;
} errno_string_map;

static struct errno_string_map err_to_str_map[] = {
	{ DSM_CAMPMIC_I2C_ERROR_NO, "camera pmic i2c error" },
	{ DSM_CAMPMIC_ENABLE_REEOR_NO, "Fault ldo:" },
	{ DSM_CAMPMIC_OVER_CURRENT_ERROR_NO, "Fault ldo:" },
	{ DSM_CAMPMIC_TSD_ERROR_NO, "Fault ldo:" },
    { DSM_CAMPMIC_UNDER_VOLTAGE_LOCKOUT_ERROR_NO, "Fault ldo:" },
};

static int get_errno_str(int err_no, char *info, int val)
{
    unsigned int i, size;
    int ret;
    char val_str[10] = {0}; // storage char size

    size = sizeof(err_to_str_map) / sizeof(errno_string_map);
 
	for (i = 0; i < size; ++i) {
		if (err_no == err_to_str_map[i].error_no)
			break;
	}
	if (i == size) {
		dmd_log_err("error type error\n");
		return -EPERM;
	}
    ret = sprintf_s(val_str, sizeof(val_str), "0x%02x", val);
	if (ret < 0 || ret >= sizeof(val_str)) {
		dmd_log_err("error with sprintf_s");
	}
 
	ret = strncat_s(info, PMIC_DMD_INFO_LENGTH,
		err_to_str_map[i].name, PMIC_DMD_INFO_LENGTH - strlen(info) - 1);
	if (ret != EOK) {
		dmd_log_err("strncat_s failed");
		return ret;
	}
    ret = strncat_s(info, PMIC_DMD_INFO_LENGTH,
        val_str, PMIC_DMD_INFO_LENGTH - strlen(info) - 1);
	if (ret != EOK) {
		dmd_log_err("strncat_s failed");
		return ret;
	}	
	return EOK;
}

int hw_pmic_dmd(int err_no, int val)
{
    char info[PMIC_DMD_INFO_LENGTH] = {0};
    struct dsm_client *client = g_pmic_dsm_client;
 
    if (!client) {
        dmd_log_err("client is null\n");
        return -EPERM;
    }
 
    if (get_errno_str(err_no, info, val)) {
        dmd_log_err("get info failed\n");
        return -EPERM;
    }
 
    if (!dsm_client_ocuppy(client)) {
        dsm_client_record(client, "%s", info);
        dsm_client_notify(client, err_no);
        dmd_log_inf("Fault LDO%d, err_no:%d\n", val, err_no);
        return 0;
    }
 
    dmd_log_err("pmic dsm client is busy\n");
    return -EPERM;
}

static int __init pmic_dsm_init(void)
{
	struct dsm_dev pmic_dsm_dev_pmic = {
		.name = "dsm_pmic",
		.device_name = NULL,
		.ic_name = NULL,
		.module_name = NULL,
		.fops = NULL,
		.buff_size = PMIC_DSM_BUF_SIZE_1024,
	};
 
	g_pmic_dsm_client = dsm_register_client(&pmic_dsm_dev_pmic);
 
	if (!g_pmic_dsm_client)
		dmd_log_inf("%s dsm register fail\n", g_pmic_dsm_client->client_name);
	else
		dmd_log_inf("%s dsm register success\n", g_pmic_dsm_client->client_name);
 
	return 0;
}

static void __exit pmic_dsm_exit(void)
{
	g_pmic_dsm_client = NULL;
}

subsys_initcall_sync(pmic_dsm_init);
module_exit(pmic_dsm_exit);

MODULE_AUTHOR("HUAWEI");
MODULE_DESCRIPTION("dsm for pmic module driver");
MODULE_LICENSE("GPL");