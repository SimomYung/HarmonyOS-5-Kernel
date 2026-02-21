/*
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/delay.h>
#include <linux/oom.h>
#include <securec.h>
#include <linux/pagewalk.h>
#include <linux/interrupt.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <platform_include/cee/linux/lpm_thermal.h>
#include <platform_include/cee/linux/rdr_lpm3.h>
#include <platform_include/cee/linux/hisi_ddr.h>

#include "mm_lb_plat.h"

#define POLL_TIMEOUT_US       1000000
#define SINGLE_BIT_EVENT_MAX  10
#define MULTI_BIT_EVENT_MAX   10
#define ABNML_INT_NS_STEP     32
#define ABNML_INT_NS_IDX      4
#define SLICE_IDX             4
#define INT_MASK              0xffffffff

#define SYNC_FINISH            0x1
#define CMO_EVENT_INT_CLR_FLAG 0x3
#define CMOQ_QUOTA_CNT_MASK    0xFFFF
#define CMO_OPERATE_MASK       0xf
#define CMO_BITMAP_MASK        0xffff
#define CMO_GRP_MASK           (0x3 << 16)
#define CMO_64B_PA_SHIFT       6
#define CMO_128B_PA_SHIFT      7
#define CMO_4K_PA_SHIFT        12
#define DFX_INFO_STR_LEN       256

enum temp_cluster {
	TEMP_CLUSTER_0 = 0,
	TEMP_CLUSTER_1 = 1,
	TEMP_CLUSTER_2 = 2,
	TEMP_CLUSTER_MAX = 3,
};

typedef union {
	u64 value;
	union {
		struct {
			u64 opt : 4;
			u64 cmdt : 4;
			u64 way_bitmap : 16;
			u64 res : 40;
		} by_way;

		struct {
			u64 opt : 4;
			u64 cmdt : 4;
			u64 gid_bitmap : 18;
			u64 res : 38;
		} by_gid;

		struct {
			u64 opt : 4;
			u64 cmdt : 4;
			u64 res0 : 2;
			u64 pa : 30;
			u64 res1 : 24;
		} by_64pa;

		struct {
			u64 opt : 4;
			u64 cmdt : 4;
			u64 res0 : 3;
			u64 pa : 29;
			u64 res1 : 24;
		} by_128pa;

		struct {
			u64 opt : 4;
			u64 cmdt : 4;
			u64 spa : 24;
			u64 res : 2;
			u64 epa : 24;
			u64 res1 : 6;
		} by_4xkpa;

		struct {
			u64 opt : 4;
			u64 cmdt : 4;
			u64 rslt : 4;
			u64 id : 16;
			u64 seq : 10;
			u64 res : 26;
		} sync;
	} param;
} lb_cmd;

static int single_bit = 0;
static int multi_bit = 0;
static u32 abnml_int_ns_mask[ABNML_INT_NS_IDX] = {0};

// hardware ops
void pgid_validate(u32 pgid_idx)
{
	hw_reg_pgid pgid_reg = {0};
	pgid_t *pgid = NULL;

	if (!lbdev->pgid_repo.ops) {
		lb_print(LB_ERROR, "error: pgid_repo.ops NULL!");
		return;
	}

	pgid = lbdev->pgid_repo.ops->get(pgid_idx);
	if (!pgid) {
		lb_print(LB_ERROR, "error: pgid null!");
		return;
	}

	pack_pgid_attr(pgid, &pgid_reg);
	lb_print(LB_ERROR, "pgid %u, drv_cfg 0x%lx, mst_cfg 0x%lx\n",
		pgid_idx, pgid_reg.pgid_drv_cfg.value, pgid_reg.pgid_mst_cfg.value);

	writel(pgid_reg.pgid_drv_cfg.value, SOC_SC_GID_SEC_ATTR_S_ADDR(lbdev->base, pgid_idx));
	writel(pgid_reg.pgid_mst_cfg.value, SOC_SC_GID_CFG_ADDR(lbdev->base, pgid_idx));
}

int register_check(u64 addr, u32 mask, u32 val, u32 timeout)
{
	int ret = -1;
	u32 u = 0;
	u32 loop;

	for (loop = 0; loop < timeout; loop++) {
		u = readl(addr);
		if ((u & mask) == val) {
			ret = 0;
			break;
		}
		udelay(1);
	}

	if (ret)
		lb_print(LB_ERROR, "expect:0x%x, actual:0x%x, mask:0x%x check failed\n", val, u, mask);

	return ret;
}

void set_cgid_tag_pgid_chg_en_direct(u32 cgid_idx, bool enable)
{
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
	cgid_reg.cgid_cfg.reg.cgid_tag_pgid_chg_en = enable;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
}

static void lb_dfx_irq_disable(lb_device *lbd)
{
	int i;

	for (i = 0; i < ABNML_INT_NS_IDX; i++) {
		abnml_int_ns_mask[i] = readl(SOC_SC_ABNML_INT_NS_MASK_ADDR(lbd->base, i));
		writel(INT_MASK, SOC_SC_ABNML_INT_NS_MASK_ADDR(lbd->base, i));
	}
}

static void lb_dfx_irq_enable(lb_device *lbd)
{
	int i, j;

	/* org status */
	for (i = 0; i < lbd->slc_idx; i++)
		for (j = 0; j < ABNML_INT_NS_IDX; j++)
			lb_print(LB_ERROR, "slice %d intr org status %d 0x%x\n",
				i, j, readl(slc_addr(SOC_SC_ABNML_INT_NS_ORG_STATUS_ADDR(lbd->base, j), i)));

	writel(0x1, SOC_SC_ABNML_INT_NS_CLR_ADDR(lbd->base));
	/* enable intr */
	for (i = 0; i < ABNML_INT_NS_IDX; i++)
		writel(abnml_int_ns_mask[i], SOC_SC_ABNML_INT_NS_MASK_ADDR(lbd->base, i));
}

static void lb_dfx_parse(u32 intr_bitmap, u32 idx, u32 *dfx_err_info, u32 info_num)
{
	u32 i;
	u32 num, intr;

	num = sizeof(lb_dfx_info) / sizeof(lb_dfx_type);
	for (i = 0; i < num; i++) {
		if (lb_dfx_info[i].intr < idx * ABNML_INT_NS_STEP ||
			lb_dfx_info[i].intr >= (idx + 1) * ABNML_INT_NS_STEP)
			continue;

		intr = lb_dfx_info[i].intr - idx * ABNML_INT_NS_STEP;
		if(intr_bitmap & BIT(intr)) {
			lb_print(LB_ERROR, "lb intr %u, %s\n",
				lb_dfx_info[i].intr, lb_dfx_info[i].info_msg);
			if (lb_dfx_info[i].post_process != NULL)
				lb_dfx_info[i].post_process(dfx_err_info, info_num);
		}
	}
}

DEFINE_SPINLOCK(dfx_info_lock);
struct lb_exception_irq_info_t {
	u32 irq_msk_s[ABNML_INT_NS_IDX];
	u32 irq_org_s[ABNML_INT_NS_IDX];
	u32 dfx_info[DFX_INFO_NUM];
} g_lb_exception_irq_info[SLICE_IDX];

static void lb_dfx_print(lb_device *lbd)
{
	u32 i, j;

	spin_lock(&dfx_info_lock);
	for (i = 0; i < lbd->slc_idx; i++) {
		/* read the intr status */
		lb_print(LB_ERROR, "slice %u intr type 0x%x\n", i,
			readl(slc_addr(SOC_SC_ABNML_INT_NS_TYPE_ADDR(lbd->base), i)));

		for (j = 0; j < DFX_INFO_NUM; j++) {
			g_lb_exception_irq_info[i].dfx_info[j] = readl(slc_addr(SOC_SC_ABNML_INT_NS_INFO_ADDR(lbd->base, j), i));
			lb_print(LB_ERROR, "slice %u dfx info %u 0x%x\n", i, j, g_lb_exception_irq_info[i].dfx_info[j]);
		}

		for (j = 0; j < ABNML_INT_NS_IDX; j++) {
			g_lb_exception_irq_info[i].irq_org_s[j] = readl(slc_addr(SOC_SC_ABNML_INT_NS_ORG_STATUS_ADDR(lbd->base, j), i));
			g_lb_exception_irq_info[i].irq_msk_s[j] = readl(slc_addr(SOC_SC_ABNML_INT_NS_MASK_STATUS_ADDR(lbd->base, j), i));
			lb_print(LB_ERROR, "slice %u intr org status%u 0x%x\n", i, j, g_lb_exception_irq_info[i].irq_org_s[j]);
			lb_print(LB_ERROR, "slice %u intr mask status%u 0x%x\n", i, j, g_lb_exception_irq_info[i].irq_msk_s[j]);
		}
	}
	spin_unlock(&dfx_info_lock);

#ifdef CONFIG_MM_LB_SUPPORT_REMOTE_SC
	lb_print(LB_ERROR, "using remote sc pgid bitmap:0x%llx\n", lbdev->using_remote_pgid);
#endif
}

static ecc_err_type lb_ecc_parse(u32 *bitmap)
{
	u32 i, j, intr, num;

	num = sizeof(lb_ecc_info) / sizeof(lb_ecc_type);
	for (i = 0; i < ABNML_INT_NS_IDX; i++) {
		if (bitmap[i] == 0)
			continue;

		for (j = 0; j < num; j++) {
			if (lb_ecc_info[j].intr < i * ABNML_INT_NS_STEP ||
				lb_ecc_info[j].intr >= (i + 1) * ABNML_INT_NS_STEP)
				continue;

			intr = lb_ecc_info[j].intr - i * ABNML_INT_NS_STEP;
			if (bitmap[i] & BIT(intr)) {
				if (lb_ecc_info[j].err_type == SINGLE_BIT_ERR && single_bit >= SINGLE_BIT_EVENT_MAX)
					abnml_int_ns_mask[i] |= BIT(intr);
					
				return lb_ecc_info[j].err_type;
			}
		}
		return OTHER_ERR;
	}
	return NON_ERR;
}

static int lb_peri_dfx_info(char *buf, size_t size)
{
	u32 i;
	int ret;
	int temp = 0;
	u32 freq = 0;
	size_t cur_len;
	struct peri_data data = {0};
	int temp_vals[TEMP_CLUSTER_MAX] = {0};
	u32 sensors[TEMP_CLUSTER_MAX] = {TEMP_CLUSTER_0, TEMP_CLUSTER_1, TEMP_CLUSTER_2};

	if (buf == NULL)
		return -1;

	for (i = 0; i < TEMP_CLUSTER_MAX; ++i) {
		ret = ipa_get_sensor_value(sensors[i], &temp);
		if (ret) {
			lb_print(LB_ERROR, "get sensor temp info failed\n");
			return -1;
		} else {
			temp_vals[i] = temp;
		}
	}

	if (!get_cur_qice_freq_info(&freq)){
		lb_print(LB_ERROR, "get qice freq info failed\n");
		return -1;
	}

	ret = get_peri_data(&data);
	if (ret) {
		lb_print(LB_ERROR, "get peri data failed\n");
		return -1;
	}

	cur_len = strcspn(buf, "\n");
	if (cur_len > size) {
		lb_print(LB_ERROR, "lenth of buf is too long\n");
		return -1;
	}
	ret = sprintf_s(buf + cur_len, size - cur_len,
		"freq:%d; peri:%d; peri_mem:%d; temp_c0:%d; temp_c1:%d; temp_c2:%d\n",
		freq, data.volt.peri, data.volt.peri_mem, temp_vals[TEMP_CLUSTER_0],
		temp_vals[TEMP_CLUSTER_1], temp_vals[TEMP_CLUSTER_2]);
	if (ret < 0) {
		lb_print(LB_ERROR, "concat dfx info failed\n");
		return -1;
	}

	return 0;
}

static int lb_get_ecc_dfx_info(char *buf, size_t size, ecc_err_type err_type)
{
	int ret;
	const char *single_bit_str = "[lb ecc 1bit]\n";
	const char *multi_bit_str = "[lb ecc multi bit]\n";

	if (buf == NULL || size == 0)
		return -1;

	if (err_type == MULTI_BIT_ERR) {
		ret = strncpy_s(buf, size, multi_bit_str, strlen(multi_bit_str));
		if (ret != EOK) {
			lb_print(LB_ERROR, "multi bit ecc str copy fail %d\n", ret);
			return -1;
		}
	} else if (err_type == SINGLE_BIT_ERR) {
		ret = strncpy_s(buf, size, single_bit_str, strlen(single_bit_str));
		if (ret != EOK) {
			lb_print(LB_ERROR, "1 bit ecc str copy fail %d\n", ret);
			return -1;
		}
	} else {
		lb_print(LB_ERROR, "invalid err type\n");
		return -1;
	}

	return 0;
}

void lb_ecc_diaginfo_record(ecc_err_type err_type)
{
	char dfx_info_str[DFX_INFO_STR_LEN] = {0};
	if (lb_get_ecc_dfx_info(dfx_info_str, DFX_INFO_STR_LEN, err_type))
		lb_print(LB_ERROR, "get ecc dfx info fail\n");
	if ((err_type == MULTI_BIT_ERR) &&
		(multi_bit < MULTI_BIT_EVENT_MAX)) {
		bbox_diaginfo_record(DMD_SC_ECC_MULTI_BIT, NULL, dfx_info_str);
		multi_bit++;
	} else if ((err_type == SINGLE_BIT_ERR) &&
		(single_bit < SINGLE_BIT_EVENT_MAX)) {
		bbox_diaginfo_record(DMD_SC_ECC_SINGLE_BIT, NULL, dfx_info_str);
		single_bit++;
	}
#ifdef CONFIG_MM_LB_ENG_DEBUG
	if (err_type == MULTI_BIT_ERR || err_type == OTHER_ERR)
		rdr_syserr_process_for_ap(MODID_AP_S_PANIC_LB, 0ULL, 0ULL);
#endif
}

static void lb_ecc_notify(ecc_err_type err_type)
{
	lbdev->bbox.err_type = err_type;
	schedule_work(&(lbdev->bbox.bbox_work));
}

static void lb_ecc_handle(lb_device *lbd)
{
	u32 i, j;
	u32 ecc_info[ABNML_INT_NS_IDX] = {0};
	ecc_err_type err_type;

	for (i = 0; i < lbd->slc_idx; i++) {
		for (j = 0; j < ABNML_INT_NS_IDX; j++) {
			ecc_info[j] = readl(slc_addr(SOC_SC_ABNML_INT_NS_MASK_STATUS_ADDR(lbd->base, j), i));
			lb_print(LB_ERROR, "slice%d intr mask status %d 0x%x\n", i, j, ecc_info[j]);
		}

		err_type = lb_ecc_parse(ecc_info);
		if (err_type)
			lb_ecc_notify(err_type);
	}
}

static irqreturn_t lb_dfx_handle(int irq, void *lbd)
{
	lb_print(LB_ERROR, "begin\n");

	lb_dfx_irq_disable(lbd);

	lb_dfx_print(lbd);

	lb_ecc_handle(lbd);

	return IRQ_WAKE_THREAD;
}

static irqreturn_t lb_dfx_thread(int irq, void *lbd)
{
	u32 i, j;
	struct lb_exception_irq_info_t irq_info[SLICE_IDX];

	lb_print(LB_ERROR, "thread begin\n");

	spin_lock(&dfx_info_lock);
	(void)memcpy_s(irq_info, sizeof(irq_info), g_lb_exception_irq_info, sizeof(g_lb_exception_irq_info));
	spin_unlock(&dfx_info_lock);

	for (i = 0; i < ((lb_device *)lbd)->slc_idx; i++) {
		for (j = 0; j < ABNML_INT_NS_IDX; j++) {
			if(irq_info[i].irq_msk_s[j] != 0) {
				lb_print(LB_ERROR, "parsed info:\n");
				lb_dfx_parse(irq_info[i].irq_msk_s[j], j, \
					irq_info[i].dfx_info, DFX_INFO_NUM);
			}
		}
	}

	lb_dfx_irq_enable(lbd);

	return IRQ_HANDLED;
}

int init_irq(lb_device *lbd)
{
	int ret;

	lb_dfx_irq_disable(lbd);
	ret = devm_request_threaded_irq(lbd->dev, lbd->dfx_irq, lb_dfx_handle,
		lb_dfx_thread, IRQF_ONESHOT, "dfx-intr", lbd);
	if (ret) {
		lb_print(LB_ERROR, "failed to enable dfx irq\n");
		return -EINVAL;
	}
	lb_dfx_irq_enable(lbd);
	return 0;
}

void lb_cmo_sync(void)
{
	u32 status;
	u32 cmoid;
	int timeout;
	unsigned long flags;
	lb_cmd sync;

	preempt_disable();
	local_irq_save(flags);

	cmoid = 0;
	sync.value = 0;
	sync.param.sync.opt = 1;
	sync.param.sync.cmdt = 1;
	sync.param.sync.rslt = cmoid;
	sync.param.sync.seq = 1;
	writeq(sync.value, SOC_GLB_CFGCMO_CMD_L_ADDR(lbdev->base));

	/* ensure cmo cmd complete */
	mb();

	timeout = POLL_TIMEOUT_US;

	while (!(readl(SOC_CFG_CMO_STATUS_ADDR(lbdev->base, cmoid)) & SYNC_FINISH)) {
		if (!--timeout) {
			lb_print(LB_ERROR, "time out\n");
			break;
		}
		udelay(1);
	}

	writel(CMO_EVENT_INT_CLR_FLAG, SOC_CFG_CMO_CLEAR_ADDR(lbdev->base, cmoid));

	status = readl(SOC_CFG_CMO_STATUS_ADDR(lbdev->base, cmoid));

	WARN_ON(status & SYNC_FINISH);

	local_irq_restore(flags);
	preempt_enable();
}

static int build_cmo_cmd(ops_type ops, cmo_type by_x,
			u32 bitmap, u64 pa, size_t sz, u64 *value)
{
	int ret = 0;
	lb_cmd cmo;

	cmo.value = 0;

	switch (by_x) {
	case CMO_BY_WAY:
		cmo.param.by_way.opt = ops & CMO_OPERATE_MASK;
		cmo.param.by_way.cmdt = by_x;
		cmo.param.by_way.way_bitmap = bitmap & CMO_BITMAP_MASK;
		break;

	case CMO_BY_GID:
		cmo.param.by_gid.opt = ops & CMO_OPERATE_MASK;
		cmo.param.by_gid.cmdt = by_x;
		cmo.param.by_gid.gid_bitmap = bitmap & (CMO_BITMAP_MASK | CMO_GRP_MASK);
		break;

	case CMO_BY_64PA:
		cmo.param.by_64pa.opt = ops & CMO_OPERATE_MASK;
		cmo.param.by_64pa.cmdt = by_x;
		cmo.param.by_64pa.pa = pa >> CMO_64B_PA_SHIFT;
		if (!IS_ALIGNED(pa, SZ_64)) {
			lb_print(LB_ERROR, "pa is not align 64\n");
			ret = -EINVAL;
		}
		break;

	case CMO_BY_128PA:
		cmo.param.by_128pa.opt = ops & CMO_OPERATE_MASK;
		cmo.param.by_128pa.cmdt = by_x;
		cmo.param.by_128pa.pa = pa >> CMO_128B_PA_SHIFT;
		if (!IS_ALIGNED(pa, SZ_128)) {
			lb_print(LB_ERROR, "pa is not align 64\n");
			ret = -EINVAL;
		}
		break;
	case CMO_BY_4XKPA:
		cmo.param.by_4xkpa.opt = ops & CMO_OPERATE_MASK;
		cmo.param.by_4xkpa.cmdt = by_x;
		cmo.param.by_4xkpa.spa = (pa >> CMO_4K_PA_SHIFT);
		cmo.param.by_4xkpa.epa = ((pa + sz) >> CMO_4K_PA_SHIFT) - 1;
		if (!IS_ALIGNED(pa, PAGE_SIZE)
			|| !IS_ALIGNED(pa + sz, PAGE_SIZE)
			|| pa >= pa + sz) {
			WARN_ON(1);

			ret = -EINVAL;
		}
		break;

	default:
		lb_print(LB_ERROR, "invalid type %d\n", by_x);
		ret = -EINVAL;
		break;
	}

	*value = cmo.value;

	return ret;
}

int lb_ops_cache(ops_type ops, cmo_type by_x,
				u32 bitmap, u64 pa, size_t sz)
{
	int ret;
	lb_cmd cmo;

	if ((ops > QUOTA_CNT) || by_x > CMO_BY_4XKPA) {
		lb_print(LB_ERROR, "%d %d invalid param\n", ops, by_x);
		return -EINVAL;
	}

	cmo.value = 0;

	ret = build_cmo_cmd(ops, by_x, bitmap, pa, sz, &cmo.value);
	if (ret)
		return ret;

	writeq(cmo.value, SOC_GLB_CFGCMO_CMD_L_ADDR(lbdev->base));

	return ret;
}

u32 check_pgid_cnt(void)
{
	int timeout;
	u32 cnt;
	u32 sum = 0;
	u32 i;

	timeout = POLL_TIMEOUT_US;
	for (i = 0 ; i < lbdev->slc_idx; i++) {
		while (true) {
			cnt = readl(slc_addr(SOC_SC_CMOQ_QUOTA_CNT_ADDR(lbdev->base), i));
			if (cnt & BIT(SOC_SC_CMOQ_QUOTA_CNT_cmoq_quota_cnt_vld_START)) {
				lb_print(LB_INFO, "slc-%u: cacheline cnt = 0x%x\n",
					i, cnt & CMOQ_QUOTA_CNT_MASK);
				sum += cnt & CMOQ_QUOTA_CNT_MASK;
				break;
			}

			if (!--timeout) {
				lb_print(LB_ERROR, "slc-%u time out\n", i);
				return 0;
			}
			udelay(1);
		}
	}

	return sum;
}

void end_flow(void)
{
	writel(0, SOC_SC_STATIS_GLB_EN_ADDR(lbdev->base));
}

u32 lb_get_flow_in(u32 statis_num)
{
	int i;
	u32 cnt = 0;

	if (statis_num >= MAX_STATIS_NUM) {
		lb_print(LB_ERROR, "invalid statis num\n");
		return -1;
	}

	end_flow();

	for (i = 0; i < lbdev->slc_idx; i++)
		cnt += readl(slc_addr(SOC_SC_STATIS_CNT_IN_ADDR(lbdev->base, statis_num), i));

	return cnt;
}

u32 lb_get_flow_out(u32 statis_num)
{
	int i;
	u32 cnt = 0;

	if (statis_num >= MAX_STATIS_NUM) {
		lb_print(LB_ERROR, "invalid statis num\n");
		return -1;
	}

	end_flow();

	for (i = 0; i < lbdev->slc_idx; i++)
		cnt += readl(slc_addr(SOC_SC_STATIS_CNT_OUT_ADDR(lbdev->base, statis_num), i));

	return cnt;
}

int cgid_csz_show(int cgid)
{
	int i;
	u32 csz;
	u32 sum = 0;

	if (!lbdev) {
		lb_print(LB_ERROR, "point is NULL\n");
		return -1;
	}

	if (cgid < 0 || cgid >= MAX_CGID_CNT) {
		lb_print(LB_ERROR, "invalid cgid\n");
		return -1;
	}

	for (i = 0; i < lbdev->slc_idx; i++) {
		csz = readl(slc_addr(SC_CGID_CNT_ADDR(lbdev->base, cgid), i)) & 0xffff;
		sum += csz;
		lb_print(LB_ERROR, "gid-%d slice-%d 0x%lx\n", cgid, i, csz);
	}

	return sum;
}

#ifdef CONFIG_MM_LB_GM_TEST
/* FPGA debug funcs */
u32 lb_cgid_cfg(u32 cgid)
{
	if (cgid >= MAX_CGID_CNT)
		return (u32)-1;

	return readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
}

void lb_cgid_cfg_set(u32 cgid, u32 val)
{
	if (cgid >= MAX_CGID_CNT)
		return;

	writel(val, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
}

u32 lb_pgid_mst_cfg(u32 pgid)
{
	if (pgid >= MAX_PGID_CNT)
		return (u32)-1;

	return readl(SOC_SC_GID_CFG_ADDR(lbdev->base, pgid));
}

void lb_pgid_mst_cfg_set(u32 pgid, u32 val)
{
	if (pgid >= MAX_PGID_CNT)
		return;

	writel(val, SOC_SC_GID_CFG_ADDR(lbdev->base, pgid));
}

u32 lb_pgid_drv_cfg(u32 pgid)
{
	if (pgid >= MAX_PGID_CNT)
		return (u32)-1;

	return readl(SOC_SC_GID_SEC_ATTR_S_ADDR(lbdev->base, pgid));
}

void lb_pgid_drv_cfg_set(u32 pgid, u32 val)
{
	if (pgid >= MAX_PGID_CNT)
		return;

	writel(val, SOC_SC_GID_SEC_ATTR_S_ADDR(lbdev->base, pgid));
}
#endif