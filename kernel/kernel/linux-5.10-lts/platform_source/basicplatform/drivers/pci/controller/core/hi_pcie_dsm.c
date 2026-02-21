/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: PCIe controller dsm report functions.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_dsm.h"
#include "../hi_pcie_defines.h"

#if defined(CONFIG_HIPCIE_NSV) && (defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM))
#define DSM_PCIE_INIT_ERROR 909030003
#define DSM_PCIE_INIT_ERROR_STR "DSM_WIFI_CMD52_ERROR"
#define PCIE_DSM_CLIENT "dsm_pcie"
#define DSM_LOG_BUFFER_SIZE 256

static int g_last_dsm_id = 0;
static u32 g_pipe_error = 0;
static u32 g_dsm_report_type = 0;
static struct dsm_client *g_dsm_pcie_client = NULL;

#ifdef CONFIG_HISYSEVENT
struct error_no_map {
    int error_no;
    char *name;
};

struct error_no_map g_pcie_error_no_map[] = {
    {DSM_PCIE_INIT_ERROR, DSM_PCIE_INIT_ERROR_STR},
};

int pcie_errorno_to_str(int error_no, char *str, int buff_len)
{
    int i;
    int isp_error_no_map_len = sizeof(g_pcie_error_no_map) / sizeof(struct error_no_map);

    if (str == NULL) {
        hi_pcie_pr_e("[E]str is null\n");
        return -1;
    }

    for (i = 0; i < isp_error_no_map_len; i++) {
        if (g_pcie_error_no_map[i].error_no == error_no) {
            if (strlen(g_pcie_error_no_map[i].name) >= buff_len) {
                hi_pcie_pr_e("[E]length error\n");
                return -1;
            }
            if (strncpy_s(str, buff_len, g_pcie_error_no_map[i].name, strlen(g_pcie_error_no_map[i].name)) != EOK) {
                hi_pcie_pr_e("[E]cpy error\n");
                return -1;
            }
            str[buff_len - 1] = '\0';
            hi_pcie_pr_i("[I]dsm pcie_errorno_to_str str=%s\n", str);
            return 0;
        }
    }
    return -1;
}

struct dsm_client_ops hisi_dsm_ops_pcie = {
    .poll_state = NULL,
    .dump_func = NULL,
    .errorno_to_str = pcie_errorno_to_str,
};
#endif

static struct dsm_dev g_dsm_pcie = {
	.name = PCIE_DSM_CLIENT,
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
#ifdef CONFIG_HISYSEVENT
    .fops = &hisi_dsm_ops_pcie,
#else
    .fops = NULL,
#endif
	.buff_size = DSM_LOG_BUFFER_SIZE,
};

void pcie_register_dsm_client(void)
{
	if (g_dsm_pcie_client == NULL) {
		hi_pcie_pr_i("pcie_register_dsm_client enter");
		g_dsm_pcie_client = dsm_register_client(&g_dsm_pcie);
	}
}

static void pcie_dsm_client_notify(int dsm_id, const char *fmt, ...)
{
	char buf[DSM_LOG_BUFFER_SIZE] = {0};
	va_list ap;
	int32_t ret = 0;

	if (g_dsm_pcie_client == NULL) {
		hi_pcie_pr_e("dsm dsm_pcie have not registed");
		return;
	}

	if (fmt != NULL) {
		va_start(ap, fmt);
		ret = vsnprintf_s(buf, sizeof(buf), sizeof(buf) - 1, fmt, ap);
		va_end(ap);
		if (ret < 0) {
			hi_pcie_pr_e("vsnprintf_s fail, line[%d]", __LINE__);
			return;
		}
	} else {
		hi_pcie_pr_e("dsm_client_buf is null, line[%d]", __LINE__);
		return;
	}

	if (dsm_client_ocuppy(g_dsm_pcie_client) == 0) {
		dsm_client_record(g_dsm_pcie_client, buf);
		dsm_client_notify(g_dsm_pcie_client, dsm_id);
		g_last_dsm_id = dsm_id;
		hi_pcie_pr_e("dsm_pcie_client_notify success,dsm_id=%d[%s]", dsm_id, buf);
	} else {
		hi_pcie_pr_e("dsm_pcie_client_notify failed,last_dsm_id=%d dsm_id=%d", g_last_dsm_id, dsm_id);

		// retry dmd record
		dsm_client_unocuppy(g_dsm_pcie_client);
		if (dsm_client_ocuppy(g_dsm_pcie_client) == 0) {
			dsm_client_record(g_dsm_pcie_client, buf);
			dsm_client_notify(g_dsm_pcie_client, dsm_id);
			hi_pcie_pr_e("dsm_pcie notify success, dsm_id=%d[%s]", dsm_id, buf);
		} else {
			hi_pcie_pr_e("dsm_pcie client ocuppy, dsm notify failed, dsm_id=%d", dsm_id);
		}
	}
}

void pcie_dsm_record_pipe_status(u32 val)
{
	g_pipe_error = val;
}

void pcie_dsm_client_report_type(u32 type)
{
	g_dsm_report_type = type;
}

void pcie_dsm_report(u32 link_status)
{
	pcie_dsm_client_notify(DSM_PCIE_INIT_ERROR,
		"Link Fail, status is [0x%x] pipe_clk_err [0x%x] notify type [0x%x]",
		link_status, g_pipe_error, g_dsm_report_type);
	g_pipe_error = 0;
	g_dsm_report_type = 0;
}
#else
void pcie_register_dsm_client(void)
{
}

void pcie_dsm_record_pipe_status(u32 val)
{
}

void pcie_dsm_client_report_type(u32 type)
{
}

void pcie_dsm_report(u32 link_status)
{
}
#endif
