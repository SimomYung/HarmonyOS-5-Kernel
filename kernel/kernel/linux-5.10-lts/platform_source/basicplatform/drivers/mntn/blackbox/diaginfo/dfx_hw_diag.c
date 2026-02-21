/*
 *
 * hardware diaginfo record module
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
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
#include <platform_include/basicplatform/linux/dfx_hw_diag.h>
#include <asm/memory.h>
#include <asm/cacheflush.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/dma-direction.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <platform_include/basicplatform/linux/mntn_dump.h>
#include <mntn_public_interface.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <platform_include/basicplatform/linux/dfx_diaginfo_baseinfo.h>
#include <platform_include/basicplatform/linux/util.h>
#include <securec.h>
#include "bbox_diaginfo.h"
#include "../rdr_field.h"

static struct dfx_hw_diag_dev *g_dfx_hw_diag_dev = NULL;

static void clear_diag_trace(struct dfx_hw_diag_trace *trace)
{
	(void)memset_s(trace->data, DFX_HWDIAG_TRACE_DATA_MAXLEN,
		0x0, DFX_HWDIAG_TRACE_DATA_MAXLEN);
	trace->err_id = 0;
	trace->used = 0;
	__dma_map_area((const void *)trace, sizeof(*trace), DMA_TO_DEVICE);
}

static void fill_hw_diag_data(struct dfx_hw_diag_dev *d,
				unsigned int err_id, const union dfx_hw_diag_info *diaginfo)
{
	struct dfx_hw_diag_trace *trace = NULL;
	unsigned long flags;
	int ret;
	char tmpbuf[DFX_HWDIAG_TRACE_DATA_MAXLEN];

	switch (err_id) {
	case CPU_PANIC_INFO:
#ifdef CONFIG_HOTPLUG_CPU_INFO
		ret = snprintf_s(tmpbuf, DFX_HWDIAG_TRACE_DATA_MAXLEN,
			(DFX_HWDIAG_TRACE_DATA_MAXLEN - 1),
			"cpu_num=%u;hpcpu_map=0x%x", diaginfo->cpu_info.cpu_num, diaginfo->cpu_info.hpcpu_map);
#else
		ret = snprintf_s(tmpbuf, DFX_HWDIAG_TRACE_DATA_MAXLEN,
			(DFX_HWDIAG_TRACE_DATA_MAXLEN - 1),
			"cpu_num=%u", diaginfo->cpu_info.cpu_num);
#endif
		if (ret < 0) {
			pr_err("[%s]CPU_PANIC_INFO diaginfo length error\n", __func__);
			return;
		}
		break;
#ifdef CONFIG_DFX_NOC
	case NOC_FAULT_INFO:
		ret = snprintf_s(tmpbuf, DFX_HWDIAG_TRACE_DATA_MAXLEN,
			(DFX_HWDIAG_TRACE_DATA_MAXLEN - 1),
			"init_flow=%s, target_flow=%s",
			diaginfo->noc_info.init_flow,
			diaginfo->noc_info.target_flow);
		if (ret < 0) {
			pr_err("[%s]NOC_FAULT_INFO diaginfo length error\n", __func__);
			return;
		}
		break;
#endif
#ifdef CONFIG_DFX_SEC_QIC
	case QIC_FAULT_INFO:
#ifdef CONFIG_DFX_SEC_QIC_V300
		ret = snprintf_s(tmpbuf, DFX_HWDIAG_TRACE_DATA_MAXLEN,
			(DFX_HWDIAG_TRACE_DATA_MAXLEN - 1),
			"qic_buskey=0x%x,master=%s,err_address=0x%lx",
			diaginfo->qic_info.buskey,
			diaginfo->qic_info.master,
			diaginfo->qic_info.err_address);
#else
		ret = snprintf_s(tmpbuf, DFX_HWDIAG_TRACE_DATA_MAXLEN,
			(DFX_HWDIAG_TRACE_DATA_MAXLEN - 1),
			"qic_buskey=0x%x", diaginfo->qic_info.buskey);
#endif
		if (ret < 0) {
			pr_err("[%s]QIC_FAULT_INFO diaginfo length error\n", __func__);
			return;
		}
		break;
#endif
	default:
		pr_err("[%s]unsupport err_id:%u\n", __func__, err_id);
		return;
	}

	trace = d->trace_addr;
	spin_lock_irqsave(&d->record_lock, flags);
	if (trace->used) {
		spin_unlock_irqrestore(&d->record_lock, flags);
		pr_err("[%s]diaginfo store doing\n", __func__);
		return;
	}
	ret = memcpy_s((void *)trace->data, DFX_HWDIAG_TRACE_DATA_MAXLEN,
		(void *)tmpbuf, DFX_HWDIAG_TRACE_DATA_MAXLEN);
	if (ret != EOK) {
		spin_unlock_irqrestore(&d->record_lock, flags);
		pr_err("[%s]copy trace data error\n", __func__);
		return;
	}
	trace->used = 1;
	trace->err_id = err_id;
	__dma_map_area((const void *)trace, sizeof(*trace), DMA_TO_DEVICE);
	spin_unlock_irqrestore(&d->record_lock, flags);
}

static int g_panic_cpu_num = -1;
int dfx_hw_diag_get_panic_cpunum(void)
{
	return g_panic_cpu_num;
}

#ifdef CONFIG_DFX_SEC_QIC
static unsigned int qic_buskey;
unsigned int dfx_hw_diag_get_qic_buskey(void)
{
	pr_info("hw diag get qic buskey 0x%X\n", qic_buskey);
	return qic_buskey;
}
#endif

#define	PC_INFO_MAX_LEN		128
static int hw_diag_add_pc_info(char *buf, size_t size)
{
	char pc_info_str[PC_INFO_MAX_LEN] = {0};
	int ret;
	size_t len;

	if (buf == NULL)
		return -1;

	ret = sprintf_s(pc_info_str, PC_INFO_MAX_LEN, "%s", "PC:");
	if (ret <= 0) {
		pr_err("%s: sprintf pc: failed\n", __func__);
		return -1;
	}
	len = strnlen(pc_info_str, PC_INFO_MAX_LEN);
	ret = rdr_get_panic_pc(pc_info_str + len, PC_INFO_MAX_LEN - len);
	if (ret < 0) {
		pr_err("%s, rdr get panic pc failed\n", __func__);
		return -1;
	}
	ret = strcat_s(pc_info_str, PC_INFO_MAX_LEN, " ");
	if (ret != 0) {
		pr_err("%s, add ; to string faild, ret %d\n", __func__, ret);
		return -1;
	}
	len = strnlen(pc_info_str, PC_INFO_MAX_LEN);
	if (size < len + 1) {
		pr_err("%s, buf size %lu is too small for string len %lu\n", __func__, size, len);
		return -1;
	}
	ret = strncpy_s(buf, size, pc_info_str, len);
	if (ret != 0) {
		pr_err("%s, copy string failed\n", __func__);
		return -1;
	}

	return 0;
}

static void save_hw_diag_log(struct dfx_hw_diag_dev *d, const char *date)
{
	struct dfx_hw_diag_trace *trace = NULL;
	unsigned long flags;
	char data_buf[MAX_DIAGINFO_LEN] = {0};
	size_t len;
	int ret;

	if (!date) {
		pr_err("%s()invalid par is NULL\n", __func__);
		return;
	}

	trace = d->trace_addr;
	spin_lock_irqsave(&d->record_lock, flags);
	if (!trace->used) {
		pr_err("[%s]no trace data\n", __func__);
		spin_unlock_irqrestore(&d->record_lock, flags);
		return;
	}
	spin_unlock_irqrestore(&d->record_lock, flags);

	/* blow bbox_diaginfo_record just record CPU_PANIC_INFO and NOC_FAULT_INFO ,other not handle */
	switch (trace->err_id) {
	case CPU_PANIC_INFO:
		sscanf_s(trace->data, "cpu_num=%d", &g_panic_cpu_num);
		break;
#ifdef CONFIG_DFX_NOC
	case NOC_FAULT_INFO: /* blow will handle */
		break;
#endif
#ifdef CONFIG_DFX_SEC_QIC
	case QIC_FAULT_INFO:
		sscanf_s(trace->data, "qic_buskey=0x%X", &qic_buskey);
		break;
#endif
	default:
		pr_err("[%s]error err_id = %u\n", __func__, trace->err_id);
		goto clear_trace;
	}
	dfx_diaginfo_get_base_info(data_buf, MAX_DIAGINFO_LEN);
	if (trace->err_id == CPU_PANIC_INFO) {
		len = strnlen(data_buf, MAX_DIAGINFO_LEN);
		hw_diag_add_pc_info(data_buf + len, MAX_DIAGINFO_LEN - len);
	}
	ret = strncat_s(data_buf, MAX_DIAGINFO_LEN, trace->data, strlen(trace->data));
	if (ret != 0)
		pr_err("%s get diaginfo base info failed\n", __func__);
	pr_info("%s, data_buf: %s\n", __func__, data_buf);
	bbox_diaginfo_record(trace->err_id, date, "%s", data_buf);

	pr_info("[%s]err_id = %u\n", __func__, trace->err_id);

clear_trace:
	clear_diag_trace(trace);
}

/*
 * trace diaginfo in mntn hwdiag memory which to
 * be saved to dfx_diaginfo.log by dfx_hw_diaginfo_record, trace failed
 * if last trace has not been handle.
 * @param  err_id   : define in bbox_diaginfo_id_def.h
 * @param  diaginfo : for ap_s_panic, trace cpunum;
 *                    for noc, trace init_flow & target_flow
 * @return: none
 */
void dfx_hw_diaginfo_trace(unsigned int err_id, const union dfx_hw_diag_info *diaginfo)
{
	struct dfx_hw_diag_dev *d = g_dfx_hw_diag_dev;

	if (!d) {
		pr_err("[%s]dfx_hw_diag not init\n", __func__);
		return;
	}
	if (!diaginfo) {
		pr_err("[%s]diag info is NULL\n", __func__);
		return;
	}
	pr_info("[%s]err_id:%u\n", __func__, err_id);
	if (!check_mntn_switch(MNTN_GOBAL_RESETLOG))
                return;
	fill_hw_diag_data(d, err_id, diaginfo);
}

/*
 * record the traced info to
 *
 * @param  date : the rtc time of recording, if input is null,
 *         tag the current rtc time.
 *
 * @return none
 */
void dfx_hw_diaginfo_record(const char *date)
{
	struct dfx_hw_diag_dev *d = g_dfx_hw_diag_dev;

	if (!d || !date) {
		pr_err("[%s]dfx_hw_diag not init\n", __func__);
		return;
	}

	save_hw_diag_log(d, date);
}

void dfx_hw_diag_init(void)
{
	int ret;
	struct dfx_hw_diag_dev *d = NULL;

	pr_info("[%s]init start\n", __func__);
	d = kzalloc(sizeof(*d), GFP_KERNEL);
	if (!d) {
		pr_err("[%s]kzalloc failed\n", __func__);
		return;
	}

	ret = register_mntn_dump(MNTN_DUMP_HWDIAG,
		MNTN_DUMP_HWDIAG_SIZE, (void **)&d->trace_addr);
	if (ret < 0) {
		pr_err("[%s]register_mntn_dump failed\n", __func__);
		goto error_handler;
	}
	if (!d->trace_addr) {
		pr_err("[%s]trace addr invalid\n", __func__);
		goto error_handler;
	}

	d->trace_size = MNTN_DUMP_HWDIAG_SIZE;
	d->trace_max_num = (unsigned int)(d->trace_size / sizeof(struct dfx_hw_diag_trace));
	if (!d->trace_max_num) {
		pr_err("[%s]trace memory size lack\n", __func__);
		goto error_handler;
	}
	pr_info("[%s]diag info trace_size:%u, trace_max_num:%u\n", __func__,
		d->trace_size, d->trace_max_num);

	spin_lock_init(&d->record_lock);

	g_dfx_hw_diag_dev = d;
	pr_info("[%s]init ok\n", __func__);
	return;

error_handler:
	kfree(d);
}
