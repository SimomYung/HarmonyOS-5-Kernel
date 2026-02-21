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
#define PCIE_DSM_CLIENT "dsm_pcie"
#define DSM_LOG_BUFFER_SIZE 256

static int g_last_dsm_id = 0;
static u32 g_pipe_error = 0;
static struct dsm_client *g_dsm_pcie_client = NULL;

static struct dsm_dev g_dsm_pcie = {
	.name = PCIE_DSM_CLIENT,
	.device_name = NULL,
	.ic_name = NULL,
	.module_name = NULL,
	.fops = NULL,
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

void pcie_dsm_report(u32 link_status)
{
	pcie_dsm_client_notify(DSM_PCIE_INIT_ERROR,
		"Link Fail, status is [0x%x] pipe_clk_err [0x%x]",
		link_status, g_pipe_error);
	g_pipe_error = 0;
}
#else
void pcie_register_dsm_client(void)
{
}

void pcie_dsm_record_pipe_status(u32 val)
{
}

void pcie_dsm_report(u32 link_status)
{
}
#endif
