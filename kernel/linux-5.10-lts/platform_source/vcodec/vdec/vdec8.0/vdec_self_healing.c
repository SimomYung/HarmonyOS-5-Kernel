/*
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "dbg.h"
#include "vcodec_osal.h"
#include "vcodec_vdec.h"

#ifdef ENABLE_VDEC_SELF_HEALING
#include "platform_include/basicplatform/linux/dfx_bbox_diaginfo.h"
#include "bbox_diaginfo_id_def.h"
#include "platform_include/basicplatform/linux/util.h"
#include "platform_include/basicplatform/linux/ipc_rproc.h"
#include "platform_include/basicplatform/linux/ipc_msg.h"

#define DIAGINFO_MBX_MSG_LEN 2
#define PSCI_MSG_TYPE_AP_VDEC_SH IPC_CMD(OBJ_AP, OBJ_LPM3, CMD_INQUIRY, 8)
#define PSCI_MSG_TYPE_AP_VDEC_IY IPC_CMD(OBJ_AP, OBJ_LPM3, CMD_INQUIRY, 10)

/*
 * notify lpmcu pv vdec
 * Macro definitions of PSCI_MSG_TYPE_AP_VDEC_SH etc. should be same in these files below:
 * kernel\drivers\mntn\dfx_sh\dfx_sh_core.c
 * \confidential\lpmcu\include\psci.h
 */
static void vdec_send_lpm3_msg(vdec_entry *vdec)
{
	int msg[DIAGINFO_MBX_MSG_LEN] = {0};
	int ret;

	msg[0] = PSCI_MSG_TYPE_AP_VDEC_SH;
	msg[1] = SH_PV_ON;
	ret = RPROC_ASYNC_SEND(IPC_ACPU_LPM3_MBX_5, (mbox_msg_t *)msg, DIAGINFO_MBX_MSG_LEN);
	if (ret != 0) {
		dprint(PRN_ERROR, "RPROC_ASYNC_SEND failed! return 0x%x, msg:[0x%x 0x%x]", ret, msg[0], msg[1]);
	} else {
		dprint(PRN_ALWS, "vdec sh notify lpmcu succcess");
	}
}

int early_get_vdec_sh_flag(void);
void early_set_vdec_sh_flag(int state);
static int32_t vdec_read_nv_task(void *buf)
{
	*((int *)buf) = early_get_vdec_sh_flag();
	return 0;
}

static int32_t vdec_write_nv_task(void *buf)
{
	early_set_vdec_sh_flag(*((int *)buf));
	return 0;
}

// The read and write nvme interfaces have permission control. 
// Therefore, the asynchronous task mode is used.
static void vdec_write_nvme(int state)
{
	int ret;
	void *node = NULL;
	vdec_entry *vdec = vdec_get_entry();

	node = vcodec_work_submit(vdec->root_ctx, vdec_write_nv_task, &state);
	ret = vcodec_work_finish(node);
	if (ret) {
		dprint(PRN_ERROR, "set nv flag %d failed ret:%d", state, ret);
	} else {
		vdec->sh.nv_flag = state;
		vdec->sh.is_sh_nv_init = true;
		dprint(PRN_ALWS, "set nv flag %d sucess", state);
	}
}

static int vdec_read_nvme(void)
{
	int ret;
	void *node = NULL;
	int flag = SH_PV_OFF;
	vdec_entry *vdec = vdec_get_entry();

	if (vdec->sh.is_sh_nv_init) {
		flag = vdec->sh.nv_flag;
		dprint(PRN_ALWS, "get nv flag:%d", flag);
	} else {
		node = vcodec_work_submit(vdec->root_ctx, vdec_read_nv_task, &flag);
		ret = vcodec_work_finish(node);
		if (ret) {
			dprint(PRN_ERROR, "direct get nv flag failed:%d", ret);
		} else {
			dprint(PRN_ALWS, "direct get nv flag:%d", flag);
		}
	}

	return flag;
}

void vdec_sh_record_mcu_panic(void)
{
	uint32_t idx;
	vdec_entry *vdec = NULL;
	enum vdec_dmd_type type = VDEC_DMD_MCU_PANIC;

	vdec = vdec_get_entry();
	idx = vdec->sh.nodes[type].dmd_err_cnt % VDEC_MAX_DMD_ERR_CNT;
	vdec->sh.nodes[type].timestamp[idx] = ktime_get_boottime_seconds();
	vdec->sh.nodes[type].dmd_err_cnt++;
	dprint(PRN_ALWS, "type:%d, dmd_err_cnt:%d", type, vdec->sh.nodes[type].dmd_err_cnt);
}

static void vdec_do_self_healing(vdec_entry *vdec, time64_t start, time64_t end)
{
	if (start != 0 && end != 0 &&
		(end - start) <= VDEC_MAX_DMD_CHECK_INTERVAL &&
		vdec_read_nvme() == SH_PV_OFF) {
		bbox_diaginfo_record(DMD_SOC_VDEC_SH_CNT, NULL, "MCU cnt:%u, bsp cnt%u, pxp cnt:%u",
							vdec->sh.nodes[VDEC_DMD_MCU_PANIC].dmd_err_cnt,
							vdec->sh.nodes[VDEC_DMD_BSP_TIMEOUT].dmd_err_cnt,
							vdec->sh.nodes[VDEC_DMD_PXP_TIMEOUT].dmd_err_cnt);
		vdec_send_lpm3_msg(vdec);
		vdec_write_nvme(SH_PV_ON);
		dprint(PRN_ALWS, "write DMD info to nvme, notify lpm3 add voltage");
	}
}

void vdec_sh_report_mcu_panic(void)
{
	uint32_t idx;
	vdec_entry *vdec = NULL;
	time64_t start, end;
	enum vdec_dmd_type type = VDEC_DMD_MCU_PANIC;

	vdec = vdec_get_entry();
	idx = vdec->sh.nodes[type].dmd_err_cnt % VDEC_MAX_DMD_MCU_ERR_CNT;
	start = vdec->sh.nodes[type].timestamp[idx];
	end = vdec->sh.nodes[type].timestamp[(idx + VDEC_MAX_DMD_MCU_ERR_CNT - 1) % VDEC_MAX_DMD_MCU_ERR_CNT];
	if (end > 0) {
		if (vdec->sh.nodes[type].last_report_ts != 0 &&
			(end - vdec->sh.nodes[type].last_report_ts) < VDEC_MAX_DMD_REPORT_INTERVAL) {
			// Avoid frequent reporting
		} else {
			bbox_diaginfo_record(DMD_SOC_VDEC_MCU_CRASH, NULL, "MCU_CRASH cnt:%u", vdec->sh.nodes[type].dmd_err_cnt);
			vdec->sh.nodes[type].last_report_ts = end;
			dprint(PRN_ERROR, "DMD report, type:%d, error cnt:%u", type, vdec->sh.nodes[type].dmd_err_cnt);
		}
	}
	vdec_do_self_healing(vdec, start, end);
}

void vdec_sh_bsp(void)
{
	uint32_t idx;
	vdec_entry *vdec = NULL;
	time64_t start, end;
	enum vdec_dmd_type type = VDEC_DMD_BSP_TIMEOUT;
	uint32_t dmd_id = DMD_SOC_VDEC_BSP_TIMEOUT;

	vdec = vdec_get_entry();
	end = ktime_get_boottime_seconds();
	idx = vdec->sh.nodes[type].dmd_err_cnt % VDEC_MAX_DMD_ERR_CNT;
	vdec->sh.nodes[type].timestamp[idx] = end;
	vdec->sh.nodes[type].dmd_err_cnt++;

	idx = vdec->sh.nodes[type].dmd_err_cnt % VDEC_MAX_DMD_ERR_CNT;
	start = vdec->sh.nodes[type].timestamp[idx];
	if (vdec->sh.nodes[type].last_report_ts != 0 &&
		(end - vdec->sh.nodes[type].last_report_ts) < VDEC_MAX_DMD_REPORT_INTERVAL) {
		// Avoid frequent reporting
	} else {
		bbox_diaginfo_record(dmd_id, NULL, "BSP_TIMEOUT cnt:%u", vdec->sh.nodes[type].dmd_err_cnt);
		vdec->sh.nodes[type].last_report_ts = end;
		dprint(PRN_ERROR, "DMD report, type:%d, error cnt:%u", type, vdec->sh.nodes[type].dmd_err_cnt);
	}
	vdec_do_self_healing(vdec, start, end);
}

void vdec_sh_pxp(void)
{
	uint32_t idx;
	vdec_entry *vdec = NULL;
	time64_t start, end;
	enum vdec_dmd_type type = VDEC_DMD_PXP_TIMEOUT;
	uint32_t dmd_id = DMD_SOC_VDEC_PXP_TIMEOUT;

	vdec = vdec_get_entry();
	end = ktime_get_boottime_seconds();
	idx = vdec->sh.nodes[type].dmd_err_cnt % VDEC_MAX_DMD_ERR_CNT;
	vdec->sh.nodes[type].timestamp[idx] = end;
	vdec->sh.nodes[type].dmd_err_cnt++;

	idx = vdec->sh.nodes[type].dmd_err_cnt % VDEC_MAX_DMD_ERR_CNT;
	start = vdec->sh.nodes[type].timestamp[idx];
	if (vdec->sh.nodes[type].last_report_ts != 0 &&
		(end - vdec->sh.nodes[type].last_report_ts) < VDEC_MAX_DMD_REPORT_INTERVAL) {
		// Avoid frequent reporting
	} else {
		bbox_diaginfo_record(dmd_id, NULL, "PXP_TIMEOUT cnt:%u", vdec->sh.nodes[type].dmd_err_cnt);
		vdec->sh.nodes[type].last_report_ts = end;
		dprint(PRN_ERROR, "DMD report, type:%d, error cnt:%u", type, vdec->sh.nodes[type].dmd_err_cnt);
	}
	vdec_do_self_healing(vdec, start, end);
}

// The nvme is not ready when the vdec probe.
// This interface should be invoked when vdec
// is powered on for the first time.
void vdec_sh_init(void)
{
	int flag;
	vdec_entry *vdec = vdec_get_entry();

	if (vdec->sh.is_sh_nv_init) {
		dprint(PRN_ALWS, "vdec sh is done");
		return;
	}

	flag = vdec_read_nvme();
	vdec->sh.nv_flag = flag;
	vdec->sh.is_sh_nv_init = true;
	if (flag == SH_PV_ON) {
		vdec_send_lpm3_msg(vdec);
	}
	vdec_init_iy_nvme();
}

void vdec_sh_print(void)
{
	int i, j;
	vdec_entry *vdec = vdec_get_entry();
	dprint(PRN_ALWS, "==================VDEC DMD STATUS:===============");
	dprint(PRN_ALWS, "nvme flag:%d,%d,is_sh_nv_init:%d", vdec_read_nvme(), vdec->sh.nv_flag, vdec->sh.is_sh_nv_init);
	for (i = 0; i < VDEC_DMD_BUTT; ++i) {
		dprint(PRN_ALWS,
			"[%d] dmd_err_cnt:%d,last_report_ts:%lld",
			i,
			vdec->sh.nodes[i].dmd_err_cnt,
			vdec->sh.nodes[i].last_report_ts);
		for (j = 0; j < VDEC_MAX_DMD_ERR_CNT; ++j) {
			dprint(PRN_ALWS, "timestamp[%d]:%lld", j, vdec->sh.nodes[i].timestamp[j]);
		}
	}
	dprint(PRN_ALWS, "=================================");
}
#else
void vdec_sh_init(void)
{}
void vdec_sh_record_mcu_panic(void){};
void vdec_sh_report_mcu_panic(void){};
void vdec_sh_bsp(void){};
void vdec_sh_pxp(void){};
#endif

#ifdef CONFIG_IMPROVE_YIELD
static int32_t vdec_send_iy_lpm3_msg(void)
{
	int msg[DIAGINFO_MBX_MSG_LEN] = {0};
	int32_t ret;

	msg[0] = PSCI_MSG_TYPE_AP_VDEC_IY;
	ret = RPROC_ASYNC_SEND(IPC_ACPU_LPM3_MBX_5, (mbox_msg_t *)msg, DIAGINFO_MBX_MSG_LEN);
	if (ret != 0) {
		dprint(PRN_ERROR, "RPROC_ASYNC_SEND failed! return 0x%x, msg:[0x%x]", ret, msg[0]);
		return -EINVAL;
	} else {
		dprint(PRN_ALWS, "vdec iy notify lpmcu succcess");
	}
	return 0;
}

static int32_t vdec_read_iy_nvme_task(void *arg)
{
	int32_t ret;
	improve_yield_vdec_info *iy_vdec_info = (improve_yield_vdec_info *)arg;

	if (iy_vdec_info == NULL) {
		dprint(PRN_ERROR, "vdec_info is NULL");
		return -EINVAL;
	}

	ret = get_improve_yield_nv_info(MODULE_ID_VDEC, (char *)iy_vdec_info, sizeof(improve_yield_vdec_info));
	if (ret != 0) {
		dprint(PRN_ERROR, "get nv info failed :%d", ret);
		return -EFAULT;
	}

	dprint(PRN_ALWS, "vdec read iy nvme info nv_flag:%u", iy_vdec_info->nv_flag.value);
	dprint(PRN_ALWS, "vdec read iy nvme info error_flag:%u", iy_vdec_info->error_flag.value);
	dprint(PRN_ALWS, "vdec read iy nvme info reserved:%u", iy_vdec_info->reserved.value);
	return 0;
}

static int32_t vdec_read_iy_nvme(improve_yield_vdec_info *iy_vdec_info)
{
	int32_t ret;
	void *node = NULL;
	vdec_entry *vdec = vdec_get_entry();

	node = vcodec_work_submit(vdec->root_ctx, vdec_read_iy_nvme_task, iy_vdec_info);
	ret = vcodec_work_finish(node);
	if (ret)
		dprint(PRN_ERROR, "read vdec iy nv failed:%d", ret);
	else
		dprint(PRN_ALWS, "read vdec iy nv success");
	return ret;
}

void vdec_init_iy_nvme(void)
{
	int32_t ret;
	improve_yield_vdec_info iy_vdec_info;

	ret = vdec_read_iy_nvme(&iy_vdec_info);
	if (ret != 0) {
		dprint(PRN_ERROR, "get nv info failed:%d", ret);
		return;
	}
	if (iy_vdec_info.nv_flag.bits.enable_flag == 1 && iy_vdec_info.nv_flag.bits.pv_flag == 1) {
		dprint(PRN_ALWS, "have done rtb improve yield, need raise voltage");
		ret = vdec_send_iy_lpm3_msg();
		if (ret != 0) {
			dprint(PRN_ERROR, "vdec send iy lpm3 msg failed:%d", ret);
			return;
		}
	} else {
		dprint(PRN_ALWS, "have not done rtb improve yield");
	}
}

#endif
