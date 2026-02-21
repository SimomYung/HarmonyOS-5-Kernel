/*
 * dfx_sh_core.c
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 *
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
#include "dfx_sh.h"
#include "dfx_sh_check.h"
#include "rdr_inner.h"
#include "cpu_improve_yield.h"
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/time64.h>
#include <linux/delay.h>
#include <platform_include/basicplatform/linux/nve/nve_ap_kernel_interface.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <securec.h>
#include <mntn_public_interface.h>
#include <platform_include/basicplatform/linux/util.h>
#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <platform_include/basicplatform/linux/ipc_msg.h>
#include <platform_include/basicplatform/linux/dfx_hw_diag.h>
#include <platform_include/cee/linux/hisi_ddr.h>
#include <linux/mutex.h>
#include <linux/workqueue.h>

#define VALID_SH_RTC_TIME	1700000000 /* 2023-11-15 06:13:20 */

static struct task_struct *g_sh_thread;
struct sh_cfg_info_main g_sh_cfg_info_main;
struct sh_cfg_info_sub g_sh_cfg_info_sub;
static struct wakeup_source *g_dfx_sh_wl;
static bool g_sh_need_update_main_nve = false;
__attribute__((unused)) static bool g_sh_need_update_sub_nve = false;
static DEFINE_MUTEX(g_sh_nv_access_mutex);

static bool g_sh_lu_enable = SH_CFG_DISABLE;
static bool g_sh_lu_flag = SH_PV_OFF;

struct opt_nve_info_user g_nve_sh_cfg_info_main = {
	.nv_number = SH_CFG_INFO,
	.nv_name = "SHCFG",
	.valid_size = sizeof(g_sh_cfg_info_main),
};

struct opt_nve_info_user g_nve_sh_cfg_info_sub = {
	.nv_number = SH_CFG_PV,
	.nv_name = "SHPV",
	.valid_size = sizeof(g_sh_cfg_info_sub),
};

static const unsigned int g_sh_cluster_dmd_cnt[] = 
{
	ACPU_CL0_SH_CNT,
	ACPU_CL1_SH_CNT,
	ACPU_CL2_SH_CNT,
	ACPU_CL3_SH_CNT,
	ACPU_CL4_SH_CNT,
	ACPU_CL5_SH_CNT,
};

static const unsigned int g_dfv_sh_cluster_dmd_cnt[] = 
{
	ACPU_CL0_DFV_SH_CNT,
	ACPU_CL1_DFV_SH_CNT,
	ACPU_CL2_DFV_SH_CNT,
	ACPU_CL3_DFV_SH_CNT,
	ACPU_CL4_DFV_SH_CNT,
	ACPU_CL5_DFV_SH_CNT,
};

static bool is_sh_max_num_match(enum sh_type type)
{
	if (g_sh_cfg_info_main.sw_cnt[type].count == SH_SW_CNT_MAX_NUM)
		return true;
	return false;
}

static bool is_sh_threshold_match(enum sh_type type)
{
	if (g_sh_cfg_info_main.sw_cnt[type].threshold > 0 &&
			g_sh_cfg_info_main.sw_cnt[type].count >= g_sh_cfg_info_main.sw_cnt[type].threshold)
		return true;
	return false;
}

static bool is_dfv_match(enum sh_type type)
{
	if (g_sh_cfg_info_main.dfv_cfg.flag & (1 << type))
		return true;
	return false;
}

static void sh_update_rtc(enum sh_type type, u32 rtc_sec)
{
	pr_info("sh update rtc for type %u, rtc %u\n", type, rtc_sec);
	if (type >= sh_type_max)
		return;
	g_sh_cfg_info_main.panic_time[type] = rtc_sec;
}

static bool sh_check_need_clear(bool enable, unsigned int cur_time, unsigned int last_time, unsigned int interval)
{
	if (!enable || interval == 0)
		return false;
	if (last_time == 0)
		return false;
	if (cur_time > last_time && cur_time - last_time >= interval)
		return true;
	return false;
}

static bool sh_need_clear(enum sh_type type, u32 rtc_sec)
{
	bool need_clear = false;
	if (type >= sh_type_max)
		return need_clear;
	need_clear = sh_check_need_clear(g_sh_cfg_info_sub.clear_interval_enable & (1 << type),
		rtc_sec, g_sh_cfg_info_main.panic_time[type], g_sh_cfg_info_sub.clear_interval[type]);
	if (need_clear)
		pr_info("sh type %d maybe need clear data\n", type);

	return need_clear;
}

static void get_cpu_iy_info(char *str)
{
	char *s = "get CPU improve yield info fail";
	int ret = 0;
	unsigned int length = 0;

	if (IS_ERR_OR_NULL(str))
		return;

	ret = cpu_register_iy_nvme(str, &length);
	if (!ret) {
		if ((length > 0) && (length < CPU_IY_NV_INFO_MAX_LENGTH)) {
			pr_info("cpu had improve yield, get info sucess\n");
			return;
		} else if (length == 0) {
			s = "CPU not happen improve yield";
		}
	}

	ret = strncpy_s(str, CPU_IY_NV_INFO_MAX_LENGTH, s, strlen(s) + 1);
	if (ret)
		pr_err("[%s]strncpy_s error, ret %d\n", __func__, ret);

	return;
}

static void sh_diaginfo_record_cnt(enum sh_type type)
{
	unsigned int qic_buskey = 0;
	bool qice_intr;
	char str[CPU_IY_NV_INFO_MAX_LENGTH + 1] = "get cpu iy info fail";

	if (type >= sh_type_max)
		return;
	pr_info("sh record cnt for type %u, cnt %u\n", type, g_sh_cfg_info_main.sw_cnt[type].count);
	if (type <= sh_type_cluster_max) {
		get_cpu_iy_info(str);
		bbox_diaginfo_record(g_sh_cluster_dmd_cnt[type], NULL,
				"acpu cluster %u sh cnt, ver %u, cnt %d, threshold %d, reason %s:%s, %s",
				type, g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[type].count,
				g_sh_cfg_info_main.sw_cnt[type].threshold,
				rdr_get_reboot_reason(),
				rdr_get_exec_subtype(),
				str);
		return;
	}
	switch (type) {
	case sh_type_l3:
		bbox_diaginfo_record(ACPU_L3_SH_CNT, NULL,
				"acpu L3 sh cnt, ver %u, cnt %d, threshold %d, reason %s:%s",
				g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[type].count,
				g_sh_cfg_info_main.sw_cnt[type].threshold,
				rdr_get_reboot_reason(),
				rdr_get_exec_subtype());
		break;
	case sh_type_peri:
		qice_intr = check_qice_intr();
#ifdef CONFIG_DFX_SEC_QIC
		qic_buskey = dfx_hw_diag_get_qic_buskey();
#endif
		bbox_diaginfo_record(PERI_SH_CNT, NULL,
				"soc peri sh cnt, ver %u, cnt %d, threshold %d, reason %s:%s, qice_intr %d, qic_buskey %u",
				g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[type].count,
				g_sh_cfg_info_main.sw_cnt[type].threshold,
				rdr_get_reboot_reason(),
				rdr_get_exec_subtype(),
				qice_intr, qic_buskey);
		break;
	default:
		break;
	}
}

static void sh_diaginfo_record_pv(enum sh_type type)
{
	char str[CPU_IY_NV_INFO_MAX_LENGTH + 1] = "get cpu iy info fail";

	if (type >= sh_type_max)
		return;
	pr_info("sh record pv for type %u, cnt %u\n", type, g_sh_cfg_info_main.sw_cnt[type].count);
	if (type <= sh_type_cluster_max) {
		get_cpu_iy_info(str);
		bbox_diaginfo_record(g_sh_cluster_dmd_cnt[type], NULL,
				"acpu cluster %u pv, ver %u, cnt %d, threshold %d, %s",
				type, g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[type].count,
				g_sh_cfg_info_main.sw_cnt[type].threshold,
				str);
		return;
	}
	switch (type) {
	case sh_type_l3:
		bbox_diaginfo_record(ACPU_L3_SH_CNT, NULL,
				"acpu L3 sh pv, ver %u, cnt %d, threshold %d",
				g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[type].count,
				g_sh_cfg_info_main.sw_cnt[type].threshold);
		break;
	case sh_type_peri:
		bbox_diaginfo_record(PERI_SH_CNT, NULL,
				"soc peri sh pv, ver %u, cnt %d, threshold %d",
				g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[type].count,
				g_sh_cfg_info_main.sw_cnt[type].threshold);
		break;
	default:
		break;
	}
}

static void sh_diaginfo_record_isp(int change_isp_state)
{
	if (g_sh_cfg_info_main.sw_cnt[sh_type_isp].pv_flag == SH_PV_ON) {
		bbox_diaginfo_record(DMD_ISP_TIMEOUT_SH_CNT, NULL,
				"soc isp sh pv, ver %u, cnt %d, threshold %d, pv flag %d, change state %d",
				g_sh_cfg_info_main.version,
				g_sh_cfg_info_main.sw_cnt[sh_type_isp].count,
				g_sh_cfg_info_main.sw_cnt[sh_type_isp].threshold,
				g_sh_cfg_info_main.sw_cnt[sh_type_isp].pv_flag,
				change_isp_state);
		return;
	}
	bbox_diaginfo_record(DMD_ISP_TIMEOUT_SH_CNT, NULL,
			"soc isp sh cnt, ver %u, cnt %d, threshold %d, pv flag %d, change state %d",
			g_sh_cfg_info_main.version,
			g_sh_cfg_info_main.sw_cnt[sh_type_isp].count,
			g_sh_cfg_info_main.sw_cnt[sh_type_isp].threshold,
			g_sh_cfg_info_main.sw_cnt[sh_type_isp].pv_flag,
			change_isp_state);
}

static void sh_diaginfo_record_gpu_pim_pv(void)
{
	pr_info("sh record gpu pim sh pv, cnt %u, pv_flag %u\n",
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].count,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].pv_flag);
	bbox_diaginfo_record(DMD_HIGPU_SH_INFO, NULL,
			"gpu pim sh pv, ver %u, cnt %u, threshold %u, pv flag %u",
			g_sh_cfg_info_main.version,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].count,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].threshold,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].pv_flag);
	return;
}

static void sh_diaginfo_record_gpu_pim_cnt(int change_state)
{
	pr_info("sh record gpu pim sh cnt, cnt %u, pv_flag %u, change state %d\n",
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].count,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].pv_flag,
			change_state);
	bbox_diaginfo_record(DMD_HIGPU_SH_INFO, NULL,
			"gpu pim sh cnt, ver %u, cnt %u, threshold %u, pv flag %u, change state %d",
			g_sh_cfg_info_main.version,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].count,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].threshold,
			g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].pv_flag,
			change_state);
}

void sh_diaginfo_record_lu(enum sh_type type)
{
	pr_info("sh record LU sh for type %u, cnt %u\n", type, g_sh_cfg_info_main.sw_cnt[type].count);
	bbox_diaginfo_record(DMD_SOC_ACPU_LU_SH_CNT, NULL,
			"acpu cluster %u LU sh, ver %u, cnt %d",
			type, g_sh_cfg_info_main.version,
			g_sh_cfg_info_main.sw_cnt[type].count);
	return;
}

static void sh_diaginfo_record_dfv(enum sh_type type)
{
	char str[CPU_IY_NV_INFO_MAX_LENGTH + 1] = "get cpu iy info fail";

	if (type > sh_type_cluster_max)
		return;
	pr_info("sh record close dfv for type %u, cnt %u\n", type, g_sh_cfg_info_main.sw_cnt[type].count);
	get_cpu_iy_info(str);
	bbox_diaginfo_record(g_dfv_sh_cluster_dmd_cnt[type], NULL,
			"acpu cluster %u close dfv, ver %u, cnt %d, dfv threshold %d, %s",
			type, g_sh_cfg_info_main.version,
			g_sh_cfg_info_main.sw_cnt[type].count,
			g_sh_cfg_info_main.dfv_cfg.threshold,
			str);
	return;
}

static void check_dfv_flag_after_cnt(enum sh_type type)
{
	if (!g_sh_cfg_info_main.dfv_cfg.enable)
		return;

	if (is_dfv_match(type)) {
		sh_diaginfo_record_dfv(type);
		return;
	}

	if (g_sh_cfg_info_main.sw_cnt[type].enable &&
		g_sh_cfg_info_main.dfv_cfg.threshold > 0 &&
		g_sh_cfg_info_main.sw_cnt[type].count >= g_sh_cfg_info_main.dfv_cfg.threshold) {
		g_sh_need_update_main_nve = true;
		pr_info("sh type %d need set dfv flag on\n", type);
		g_sh_cfg_info_main.dfv_cfg.flag |= (1 << type);
	}
}

static void check_lu_flag_after_cnt(enum sh_type type)
{
	if (!g_sh_cfg_info_main.lu_cfg.enable)
		return;

	if (g_sh_cfg_info_main.lu_cfg.flag) {
		sh_diaginfo_record_lu(type);
		return;
	}

	if (g_sh_cfg_info_main.sw_cnt[type].enable &&
		g_sh_cfg_info_main.sw_cnt[type].threshold > 0 &&
		g_sh_cfg_info_main.sw_cnt[type].count >= g_sh_cfg_info_main.sw_cnt[type].threshold) {
		g_sh_need_update_main_nve = true;
		pr_info("sh type %d need set LU flag on\n", type);
		g_sh_cfg_info_main.lu_cfg.flag = SH_PV_ON;
	}
}

#ifdef CONFIG_IPC_RPROC
#define DIAGINFO_MBX_MSG_LEN  2
/*
 * notify lpmcu pv isp
 *
 * Macro definitions of PSCI_MSG_TYPE_AP_ISP_SH etc. should be same in these files below:
 *
 * kernel\drivers\mntn\dfx_sh\dfx_sh_core.c
 * \confidential\lpmcu\include\psci.h
 */
#define PSCI_MSG_TYPE_AP_ISP_SH IPC_CMD(OBJ_AP, OBJ_LPM3, CMD_INQUIRY, 7)
static void isp_pv_notify(int pv_flag)
{
	int msg[DIAGINFO_MBX_MSG_LEN] = {0};
	int ret;

	msg[0] = PSCI_MSG_TYPE_AP_ISP_SH;
	msg[1] = pv_flag;
	ret = RPROC_ASYNC_SEND(IPC_ACPU_LPM3_MBX_5, (mbox_msg_t *)msg, DIAGINFO_MBX_MSG_LEN);
	if (ret != 0) {
		pr_err("RPROC_ASYNC_SEND failed! return 0x%x, msg:[0x%x 0x%x]\n",
				ret, msg[0], msg[1]);
		return;
	}
	pr_err("isp sh notify lpmcu succcess\n");
}
#else
static inline void isp_pv_notify(int pv_flag) {}
#endif

static int sh_switch_state(u32 type, u32 rtc_sec)
{
	bool need_clear = false;

	g_sh_cfg_info_main.sw_cnt[type].count++;
	pr_info("sh type %u count add\n", type);
	if (g_sh_cfg_info_main.panic_time[type] != 0) {
		need_clear = sh_need_clear(type, rtc_sec);
		if (need_clear) {
			pr_info("sh type %u need clear cnt to 1\n", type);
			g_sh_cfg_info_main.sw_cnt[type].count = 1;
		}
	}
	g_sh_cfg_info_main.panic_time[type] = rtc_sec;
 
	if (!is_sh_threshold_match(type))
		return SH_STATE_NO_CHANGE;
 
	/* if cnt match threshold check pv or cancel pv and panic rtc set to 0 */
	pr_info("sh type %u change pv flag\n", type);
	g_sh_cfg_info_main.sw_cnt[type].pv_flag = ~g_sh_cfg_info_main.sw_cnt[type].pv_flag;
	g_sh_cfg_info_main.sw_cnt[type].count = 0;
	g_sh_cfg_info_main.panic_time[type] = 0;
	return SH_STATE_CHANGE;
}

static void sh_do_fast_pv(enum sh_type type, bool threshold_match) {
	if (type == sh_type_peri && sh_peri_need_fast_pv()) {
		pr_info("peri need fast pv, set sh peri cnt to threshold\n");
		if (!threshold_match)
			g_sh_cfg_info_main.sw_cnt[type].count = g_sh_cfg_info_main.sw_cnt[type].threshold;
	}
}

static bool is_sh_switch_state_type(u32 type)
{
	if (type == sh_type_isp || type == sh_type_gpu_pim)
		return true;
	return false;
}

static void do_sh(enum sh_type type, u32 rtc_sec)
{
	bool need_clear = false;
	bool threshold_match = false;

	pr_info("sh for type %d\n", type); 
	if (is_sh_switch_state_type(type)) {
		pr_info("sh state switch type %d, need back\n", type);
		return;
	}

	if (!g_sh_cfg_info_main.sw_cnt[type].enable) {
		pr_err("sh type %d enable %u\n", type, g_sh_cfg_info_main.sw_cnt[type].enable);
		return;
	}

	if (is_sh_threshold_match(type)) {
		threshold_match = true;
		sh_diaginfo_record_pv(type);
	}
 
	if (is_sh_max_num_match(type))
		return;

	if (rtc_sec <= VALID_SH_RTC_TIME) {
		pr_err("invalid rtc time %u\n", rtc_sec);
		return;
	}
	if (!sh_check_panic(type))
		return;
 
	g_sh_cfg_info_main.sw_cnt[type].count++;
	g_sh_need_update_main_nve = true;
	pr_info("sh type %d panic happened, cnt %u\n", type, g_sh_cfg_info_main.sw_cnt[type].count);
	if (g_sh_cfg_info_main.sw_cnt[type].threshold == 1) {
		sh_update_rtc(type, rtc_sec);
	} else if (g_sh_cfg_info_main.sw_cnt[type].threshold > 1) {
		need_clear = sh_need_clear(type, rtc_sec);
		sh_update_rtc(type, rtc_sec);
		if (!threshold_match && need_clear) {
			pr_info("sh type %d need clear cnt to 1\n", type);
			g_sh_cfg_info_main.sw_cnt[type].count = 1;
		}
	}
	sh_do_fast_pv(type, threshold_match);
	sh_diaginfo_record_cnt(type);
}

static int sh_get_main_nve_info(void)
{
	struct opt_nve_info_user *nve_sh_cfg_info = &g_nve_sh_cfg_info_main;
	int ret;

	nve_sh_cfg_info->nv_operation = NV_READ;
	ret = nve_direct_access_interface(nve_sh_cfg_info);
	if (ret < 0) {
		pr_err("[%s]read main nve_sh_cfg_info error\n", __func__);
		return ret;
	}

	ret = memcpy_s(&g_sh_cfg_info_main, sizeof(g_sh_cfg_info_main), nve_sh_cfg_info->nv_data, sizeof(g_sh_cfg_info_main));
	if (ret) {
		pr_err("[%s]copy main nve_sh_cfg_info data error, ret %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static int sh_get_sub_nve_info(void)
{
	struct opt_nve_info_user *nve_sh_cfg_info = &g_nve_sh_cfg_info_sub;
	int ret;

	nve_sh_cfg_info->nv_operation = NV_READ;
	ret = nve_direct_access_interface(nve_sh_cfg_info);
	if (ret < 0) {
		pr_err("[%s]read sub nve_sh_cfg_info error\n", __func__);
		return ret;
	}

	ret = memcpy_s(&g_sh_cfg_info_sub, sizeof(g_sh_cfg_info_sub), nve_sh_cfg_info->nv_data, sizeof(g_sh_cfg_info_sub));
	if (ret) {
		pr_err("[%s]copy sub nve_sh_cfg_info data error, ret %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static void sh_update_main_nve_info(void)
{
	struct opt_nve_info_user *nve_sh_cfg_info = &g_nve_sh_cfg_info_main;
	int ret;

	ret = memcpy_s(nve_sh_cfg_info->nv_data, NVE_NV_DATA_SIZE, &g_sh_cfg_info_main, sizeof(g_sh_cfg_info_main));
	if (ret) {
		pr_err("[%s]copy main nve_sh_cfg_info data error, ret %d\n", __func__, ret);
		return;
	}

	nve_sh_cfg_info->nv_operation = NV_WRITE;
	__pm_stay_awake(g_dfx_sh_wl);
	ret = nve_direct_access_interface(nve_sh_cfg_info);
	__pm_relax(g_dfx_sh_wl);
	if (ret < 0) {
		pr_err("[%s]update main nve_sh_cfg_info error\n", __func__);
		return;
	}

	return;
}

static int sh_update_sub_nve_info(void)
{
	struct opt_nve_info_user *nve_sh_cfg_info = &g_nve_sh_cfg_info_sub;
	int ret;

	ret = memcpy_s(nve_sh_cfg_info->nv_data, NVE_NV_DATA_SIZE, &g_sh_cfg_info_sub, sizeof(g_sh_cfg_info_sub));
	if (ret) {
		pr_err("[%s]copy sub nve_sh_cfg_info data error, ret %d\n", __func__, ret);
		return ret;
	}

	nve_sh_cfg_info->nv_operation = NV_WRITE;
	__pm_stay_awake(g_dfx_sh_wl);
	ret = nve_direct_access_interface(nve_sh_cfg_info);
	__pm_relax(g_dfx_sh_wl);
	if (ret < 0) {
		pr_err("[%s]update sub nve_sh_cfg_info error\n", __func__);
		return ret;
	}

	return 0;
}

#ifdef CONFIG_DFX_SH_DDR
static void update_ddr_sh(void) {
	struct ddr_dfs_fail_info *g_ddr_dfs_fail_info = NULL;

	if (!g_sh_cfg_info_sub.ddr_nv_info.dfs_enable) {
		pr_err("dfs_enable %u\n", g_sh_cfg_info_sub.ddr_nv_info.dfs_enable);
		return;
	}

	g_ddr_dfs_fail_info = (struct ddr_dfs_fail_info *)ioremap_wc(RDR_DDR_DFS_EXCEPTION_INFO_BASE, RDR_DDR_DFS_EXCEPTION_INFO_SIZE);
	if (g_ddr_dfs_fail_info == NULL) {
		pr_err("get dfs info fail.\n");
		return;
	}

	if (g_ddr_dfs_fail_info->magic_num_start != DDR_DFS_EXCE_MAGICNUM) {
		iounmap(g_ddr_dfs_fail_info);
		g_ddr_dfs_fail_info = NULL;
		return;
	}

	if (g_sh_cfg_info_sub.ddr_nv_info.dfs_cnts > 0)
		bbox_diaginfo_record(DMD_DDR_DFS_ERR_INFO, NULL,
				"ddr dfs cnt %d, channel %d",
				g_sh_cfg_info_sub.ddr_nv_info.dfs_cnts,
				g_sh_cfg_info_sub.ddr_nv_info.dfs_channel);

	iounmap(g_ddr_dfs_fail_info);
	g_ddr_dfs_fail_info = NULL;
	return;
}
#endif

static void isp_sh_do_notify_dmd(int change_isp_state)
{
	if (g_sh_cfg_info_main.sw_cnt[sh_type_isp].enable == SH_CFG_DISABLE)
		return;

	if (change_isp_state == SH_STATE_CHANGE)
		isp_pv_notify(g_sh_cfg_info_main.sw_cnt[sh_type_isp].pv_flag);

	sh_diaginfo_record_isp(change_isp_state);
	return;
}

static void isp_sh_boot_notify(void) {
	if (g_sh_cfg_info_main.sw_cnt[sh_type_isp].enable == SH_CFG_DISABLE)
		return;

	if (g_sh_cfg_info_main.sw_cnt[sh_type_isp].pv_flag == SH_PV_ON) {
		isp_pv_notify(SH_PV_ON);
		pr_err("isp notify pv when boot\n");
	}
	return;
}
 
static void isp_sh_wk(struct work_struct *wk)
{
	struct timespec64 tv = {0};
	int change_isp_state = SH_STATE_NO_CHANGE;
 
	pr_err("isp timeout happen\n");
	if (g_sh_cfg_info_main.sw_cnt[sh_type_isp].enable == 0) {
		pr_err("isp sh is disable\n");
		return;
	}
	ktime_get_real_ts64(&tv);
	if (tv.tv_sec <= VALID_SH_RTC_TIME) {
		pr_err("invalid rtc time %lld\n", tv.tv_sec);
		return;
	}

	mutex_lock(&g_sh_nv_access_mutex);
	if (sh_get_main_nve_info())
		goto error;
	if (sh_get_sub_nve_info())
		goto error;

	change_isp_state = sh_switch_state(sh_type_isp, tv.tv_sec);
	isp_sh_do_notify_dmd(change_isp_state);
	sh_update_main_nve_info();
 
error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return;
}

static DECLARE_WORK(isp_sh_wk_queue, isp_sh_wk);
static ssize_t isp_timeout_proc_write(struct file *file, const char __user *buf,
								size_t nr, loff_t *off)
{
	schedule_work(&isp_sh_wk_queue);
	return nr;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
static const struct proc_ops isp_timeout_proc_fops = {
		.proc_write = isp_timeout_proc_write,
#else
static const struct file_operations isp_timeout_proc_fops = {
		.write = isp_timeout_proc_write,
#endif
};

static void isp_timeout_proc_init(void)
{
	dfx_create_stats_proc_entry("isp_timeout", (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH), &isp_timeout_proc_fops, NULL);
	return;
}

void gpu_exc_do_peri_sh()
{
	struct timespec64 tv = {0};
	u32 rtc_sec;

	ktime_get_real_ts64(&tv);
	rtc_sec = (u32)tv.tv_sec;
	if (rtc_sec <= VALID_SH_RTC_TIME) {
		pr_err("invalid rtc time %u\n", rtc_sec);
		return;
	}

	mutex_lock(&g_sh_nv_access_mutex);
	if (sh_get_main_nve_info())
		goto error;

	if (g_sh_cfg_info_main.sw_cnt[sh_type_peri].enable && !is_sh_threshold_match(sh_type_peri)) {
		pr_info("gpu exc, set sh peri cnt to threshold\n");
		g_sh_cfg_info_main.sw_cnt[sh_type_peri].count = g_sh_cfg_info_main.sw_cnt[sh_type_peri].threshold;
		sh_update_rtc(sh_type_peri, rtc_sec);
		sh_update_main_nve_info();
	}

error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return;
}

void cpu_ecc_do_sh(int ecc_type, int cluster)
{
	struct timespec64 tv = {0};
	u32 rtc_sec;
	bool need_updater_main_nve = false;
	enum sh_type type;

	if (cluster < sh_type_cluster_0 || cluster > sh_type_l3) {
		pr_err("invalid cluster %d\n", cluster);
		return;
	}
	type = cluster;
	ktime_get_real_ts64(&tv);
	rtc_sec = (u32)tv.tv_sec;
	if (rtc_sec <= VALID_SH_RTC_TIME) {
		pr_err("invalid rtc time %u\n", rtc_sec);
		return;
	}

	mutex_lock(&g_sh_nv_access_mutex);
	if (sh_get_main_nve_info())
		goto error;

	if (type <= sh_type_cluster_max && g_sh_cfg_info_main.dfv_cfg.enable && !is_dfv_match(type)) {
		pr_info("cpu cache ecc-ue happened, set sh cpu dfv flag on\n");
		need_updater_main_nve = true;
		g_sh_cfg_info_main.dfv_cfg.flag |= (1 << type);
	}

	if (g_sh_cfg_info_main.sw_cnt[type].enable &&
		!is_sh_threshold_match(type) &&
		ecc_type == SH_CPU_ECC_UE) {
		pr_info("cpu cache ecc-ue happened, set sh cpu cnt to threshold\n");
		need_updater_main_nve = true;
		g_sh_cfg_info_main.sw_cnt[type].count = g_sh_cfg_info_main.sw_cnt[type].threshold;
		sh_update_rtc(type, rtc_sec);
	}

	if (type >= sh_type_cluster_big &&
		type <= sh_type_cluster_max &&
		ecc_type == SH_CPU_ECC_UE) {
		if (g_sh_cfg_info_main.lu_cfg.enable && !g_sh_cfg_info_main.lu_cfg.flag) {
			pr_info("cpu cache ecc-ue happened, set LU sh flag on\n");
			need_updater_main_nve = true;
			g_sh_cfg_info_main.lu_cfg.flag = SH_PV_ON;
		}
	}

	if (need_updater_main_nve)
		sh_update_main_nve_info();

error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return;
}

static void lu_sh_init(void)
{
	g_sh_lu_enable = g_sh_cfg_info_main.lu_cfg.enable;
	g_sh_lu_flag = g_sh_cfg_info_main.lu_cfg.flag;
}

int get_lu_sh_flag(int cluster)
{
	if (cluster < sh_type_cluster_big || cluster > sh_type_cluster_max)
		return SH_UNSUPPORTED;

	if (!g_sh_lu_enable)
		return SH_CFG_DISABLE;

	return g_sh_lu_flag;
}

int early_set_vdec_sh_flag(int state)
{
	int ret;

	mutex_lock(&g_sh_nv_access_mutex);
	ret = sh_get_sub_nve_info();
	if (ret)
		goto error;

	if (g_sh_cfg_info_sub.vdec_cfg.enable != SH_CFG_ENABLE) {
		ret = -EINVAL;
		goto error;
	}

	g_sh_cfg_info_sub.vdec_cfg.flag = state;
	ret = sh_update_sub_nve_info();

error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return ret;
}

int early_get_vdec_sh_flag(void)
{
	int ret;
	int flag = SH_PV_OFF;

	mutex_lock(&g_sh_nv_access_mutex);
	ret = sh_get_sub_nve_info();
	if (ret)
		goto error;

	if (g_sh_cfg_info_sub.vdec_cfg.enable != SH_CFG_ENABLE)
		goto error;

	flag = g_sh_cfg_info_sub.vdec_cfg.flag;

error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return flag;
}

void gpu_pim_do_sh(u32 rtc_sec)
{
	int change_state = SH_STATE_NO_CHANGE;

	if (!g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].enable) {
		pr_err("sh gpu pim is disable %u\n", g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].enable);
		return;
	}

	if (g_sh_cfg_info_main.sw_cnt[sh_type_gpu_pim].pv_flag == SH_PV_ON)
		sh_diaginfo_record_gpu_pim_pv();

	if (rtc_sec <= VALID_SH_RTC_TIME) {
		pr_err("invalid rtc time %u\n", rtc_sec);
		return;
	}

	if (is_sh_gpu_pim_exc()) {
		g_sh_need_update_main_nve = true;
		change_state = sh_switch_state(sh_type_gpu_pim, rtc_sec);
		sh_diaginfo_record_gpu_pim_cnt(change_state);
		return;
	}
}

#ifdef CONFIG_DFX_SH_HFRC
static void sh_diaginfo_record_hfrc_pv(void)
{
	pr_info("sh record hfrc sh pv, cnt %u, threshold %u\n",
			g_sh_cfg_info_sub.hfrc_cfg.count,
			g_sh_cfg_info_sub.hfrc_cfg.threshold);
	bbox_diaginfo_record(DMD_SOC_HFRC_SH_CNT, NULL,
			"hfrc sh pv, ver %u, cnt %u, threshold %u",
			g_sh_cfg_info_main.version,
			g_sh_cfg_info_sub.hfrc_cfg.count,
			g_sh_cfg_info_sub.hfrc_cfg.threshold);
}

static void sh_diaginfo_record_hfrc_cnt(void)
{
	pr_info("sh record hfrc sh cnt, cnt %u, threshold %u\n",
			g_sh_cfg_info_sub.hfrc_cfg.count,
			g_sh_cfg_info_sub.hfrc_cfg.threshold);
	bbox_diaginfo_record(DMD_SOC_HFRC_SH_CNT, NULL,
			"hfrc sh cnt, ver %u, cnt %u, threshold %u",
			g_sh_cfg_info_main.version,
			g_sh_cfg_info_sub.hfrc_cfg.count,
			g_sh_cfg_info_sub.hfrc_cfg.threshold);
}

void hfrc_do_sh(void)
{
	if (!g_sh_cfg_info_sub.hfrc_cfg.enable) {
		pr_info("hfrc_sh_enable %u\n", g_sh_cfg_info_sub.hfrc_cfg.enable);
		return;
	}

	if (g_sh_cfg_info_sub.hfrc_cfg.threshold > 0 &&
		g_sh_cfg_info_sub.hfrc_cfg.count >= g_sh_cfg_info_sub.hfrc_cfg.threshold)
		sh_diaginfo_record_hfrc_pv();

	if (g_sh_cfg_info_sub.hfrc_cfg.count == SH_SW_CNT_MAX_NUM)
		return;

	if(!is_sh_hfrc_panic())
		return;

	g_sh_cfg_info_sub.hfrc_cfg.count++;
	g_sh_need_update_sub_nve = true;
	sh_diaginfo_record_hfrc_cnt();
}
#endif

int early_get_dss_sh_flag(void)
{
	int ret;
	int flag = SH_PV_OFF;

	mutex_lock(&g_sh_nv_access_mutex);
	ret = sh_get_sub_nve_info();
	if (ret)
		goto error;

	if (g_sh_cfg_info_sub.dss_cfg.enable != SH_CFG_ENABLE)
		goto error;

	flag = g_sh_cfg_info_sub.dss_cfg.flag;

error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return flag;
}

int early_set_dss_sh_flag(int state)
{
	int ret;

	mutex_lock(&g_sh_nv_access_mutex);
	ret = sh_get_sub_nve_info();
	if (ret)
		goto error;

	if (g_sh_cfg_info_sub.dss_cfg.enable != SH_CFG_ENABLE) {
		ret = -EINVAL;
		goto error;
	}

	g_sh_cfg_info_sub.dss_cfg.flag = state;
	ret = sh_update_sub_nve_info();

error:
	mutex_unlock(&g_sh_nv_access_mutex);
	return 0;
}

static int sh_thread_body(void *arg)
{
	enum sh_type type;
	struct timespec64 tv = {0};

	ssleep(30); /* wait for rtc and nve ready */

	BUILD_BUG_ON(sizeof(g_sh_cfg_info_main) != NVE_NV_DATA_SIZE);
	BUILD_BUG_ON(sizeof(g_sh_cfg_info_sub) != NVE_NV_DATA_SIZE);
	BUILD_BUG_ON(sh_type_max > SH_CFG_INFO_TYPE_MAX);

	ktime_get_real_ts64(&tv);
	pr_info("dfx sh process, rtc %lld\n", tv.tv_sec);
	if (sh_get_main_nve_info())
		return -EINVAL;
	if (sh_get_sub_nve_info())
		return -EINVAL;

	g_sh_need_update_main_nve = false;
	for (type = sh_type_start; type < sh_type_max; type++)
		do_sh(type, tv.tv_sec);

	for (type = sh_type_start; type <= sh_type_cluster_max; type++)
		check_dfv_flag_after_cnt(type);

	for (type = sh_type_cluster_big; type <= sh_type_cluster_max; type++)
		check_lu_flag_after_cnt(type);

	gpu_pim_do_sh(tv.tv_sec);

#ifdef CONFIG_DFX_SH_HFRC
	hfrc_do_sh();
#endif

#ifdef CONFIG_DFX_SH_DDR
	update_ddr_sh();
#endif
	isp_sh_boot_notify();
	if (g_sh_need_update_main_nve)
		sh_update_main_nve_info();
	if (g_sh_need_update_sub_nve)
		sh_update_sub_nve_info();

	lu_sh_init();
	isp_timeout_proc_init();
	return 0;
}

static int __init sh_init(void)
{
	g_dfx_sh_wl = wakeup_source_register(NULL, "dfx_sh");
	g_sh_thread = kthread_run(sh_thread_body, NULL, "sh_thread");
	if (g_sh_thread == NULL) {
		wakeup_source_unregister(g_dfx_sh_wl);
		pr_err("create sh thread failed\n");
		return -EINVAL;
	}

	return 0;
}

static void __exit sh_exit(void)
{
	if (g_sh_thread != NULL)
		kthread_stop(g_sh_thread);
}

late_initcall(sh_init);
module_exit(sh_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("DFX_SH MODULE");
