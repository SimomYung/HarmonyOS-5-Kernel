/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/ktime.h>
#include "dkmd_mntn_rdr.h"
#include "dkmd_log.h"
 
static struct rdr_exception_info_s g_dpu_excetption_info[] = {
	{
	.e_modid			= (u32)MODID_DSS_VACT_IRQ_TIMEOUT,
	.e_modid_end 		= (u32)MODID_DSS_VACT_IRQ_TIMEOUT,
	.e_process_priority = RDR_ERR,
	.e_reboot_priority 	= RDR_REBOOT_NO,
	.e_notify_core_mask = RDR_DSS,
	.e_reset_core_mask 	= RDR_DSS,
	.e_from_core 		= RDR_DSS,
	.e_reentrant 		= (u32)RDR_REENTRANT_DISALLOW,
	.e_exce_type 		= DSS_S_EXCEPTION,
	.e_exce_subtype 	= DSS_VACT_IRQ_TIMEOUT,
	.e_upload_flag 		= (u32)RDR_UPLOAD_YES,
	.e_from_module 		= "DSS",
	.e_save_log_flags   = RDR_SAVE_TRACK_INFO | RDR_SAVE_LOGBUF,
	.e_desc 			= "MODID_DSS_VACT_IRQ_TIMEOUT",
	 },
};
 
static bool g_rdr_inited = false;
static struct rdr_register_module_result g_current_info;

static void dss_dump(u32 modid, u32 etype, u64 coreid,
	char *pathname, pfn_cb_dump_done pfn_cb)
{
	void_unused(modid);
	void_unused(etype);
	void_unused(coreid);
	void_unused(pathname);
	if (pfn_cb)
		pfn_cb(modid, RDR_DSS);
}

static void dss_reset(u32 modid, u32 etype, u64 coreid)
{
	void_unused(modid);
	void_unused(etype);
	void_unused(coreid);
}

static int dkmd_mntn_register_core(void)
{
	struct rdr_module_ops_pub module_ops;
	int ret;
	module_ops.ops_dump = dss_dump;
	module_ops.ops_reset = dss_reset;
	ret = rdr_register_module_ops(RDR_DSS, &module_ops, &g_current_info);
	if (ret < 0) {
		dpu_pr_err("register failed! ret = %d\n", ret);
		return ret;
	}
	return 0;
}

static int dkmd_mntn_register_exception(void)
{
	uint32_t ret = 0;
	size_t size;
	unsigned long index;
	size = sizeof(g_dpu_excetption_info) / sizeof(struct rdr_exception_info_s);
	for (index = 0; index < (unsigned long)size; index++) {
		/* error return 0, ok return modid */
		ret = rdr_register_exception(&g_dpu_excetption_info[index]);
		if (ret == 0) {
			dpu_pr_err("register fail! index = %lu\n", index);
			return -EINVAL;
		}
	}
	return 0;
}

int32_t dkmd_rdr_dump_system_init(void)
{
	int32_t ret = 0;
	if (g_rdr_inited)
		return 0;

	ret = dkmd_mntn_register_core();
	if (ret < 0)
		return ret;

	ret = dkmd_mntn_register_exception();
	if (ret < 0)
		return ret;

	g_rdr_inited = true;
	return 0;
}

void dkmd_rdr_dump_exception_info(uint32_t modid, uint64_t arg1, uint64_t arg2)
{
	// init code cost time 60us.
	dkmd_rdr_dump_system_init();

	if (!g_rdr_inited)
		return ;

	if (modid > MODID_DSS_EXCEPTION_END) {
		dpu_pr_err("Invalid modid:%u", modid);
		return ;
	}
	rdr_system_error(modid, arg1, arg2);
}