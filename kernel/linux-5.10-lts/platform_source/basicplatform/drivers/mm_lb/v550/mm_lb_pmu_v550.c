/*
 * pmu config for pmu event statistics
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


#include <securec.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/io.h>
#include <linux/platform_device.h>

#include "mm_lb_priv_v550.h"

/* pmu config register */
#define SC_PMU_FEDB_BUF_SEL         0x2C04
#define SC_PMU_FEDB_BUF_TH          0x2C08
#define SC_PMU_FECQ_BUF_TH          0x2C10
#define SC_PMU_TPIPESPEC_FAIL       0x2C30
#define SC_PMU_TPIPESPEC_REQ        0x2C34
#define SC_PMU_BECQ_TH_CTRL         0x2C38
#define SC_PMU_TAG_HIT_ST           0x2C3C
#define SC_PMU_BECQ_FSM_EN0         0x2C40
#define SC_PMU_BECQ_FSM_EN1         0x2C50
#define SC_PMU_BYPBUF_TH            0x2C68
#define SC_PMU_MSTIF_TH             0x2C6C
#define SC_PMU_DMEMIF_TH            0x2C70
#define SC_PMU_FLT_GID              0x2C74
#define SC_PMU_FLT_MID              0x2C78
#define SC_PMU_FLT_MPAM             0x2C7C
#define SC_PMU_FLT_PRI              0x2C80
#define SC_PMU_TPIPE_HAZ_TYPE       0x2C88
#define SC_PMU_MST2SN_FLT           0x2C8C
#define SC_PMU_BECQ_SLOT_TH         0x2C94
#define SC_PMU_QOSALLOW_SC          0x2C98
#define SC_PMU_FECQ_FLT             0x2C9C
#define SC_PMU_LPID_FLT             0x2CA0
#define SC_PMU_FLT_FECQ_ID          0x2CB4

/* pmu control register */
#define SC_PMU_EVCNT                0x2D80
#define SC_PMU_EVSVR                0x2DA0
#define SC_PMU_EVTYPE               0x2DC0
#define SC_PMU_CNTEN                0x2DE0
#define SC_PMU_OV                   0x2DE4
#define SC_PMU_CFG                  0x2DE8
#define SC_PMU_CAPR                 0x2DEC

/* pmu filter */
#define PMU_FLT_GID              (0x3f)
#define PMU_FLT0_GID_SHIFT       (0)
#define PMU_FLT0_GID_MASK_SHIFT  (16)
#define PMU_FLT1_GID_SHIFT       (6)
#define PMU_FLT1_GID_MASK_SHIFT  (22)

#define PMU_FLT_MID              (0xff)
#define PMU_FLT0_MID_SHIFT       (0)
#define PMU_FLT0_MID_MASK_SHIFT  (16)
#define PMU_FLT1_MID_SHIFT       (8)
#define PMU_FLT1_MID_MASK_SHIFT  (24)

#define PMU_FLT_MPAM             (0xf)
#define PMU_FLT0_MPAM_SHIFT      (0)
#define PMU_FLT0_MPAM_MASK_SHIFT (8)
#define PMU_FLT1_MPAM_SHIFT      (4)
#define PMU_FLT1_MPAM_MASK_SHIFT (12)

#define PMU_FLT_PRI              (0x3)
#define PMU_FLT0_PRI_SHIFT       (0)
#define PMU_FLT0_PRI_MASK_SHIFT  (8)
#define PMU_FLT1_PRI_SHIFT       (2)
#define PMU_FLT1_PRI_MASK_SHIFT  (10)

#define PMU_FLT0_FECQ_ID         (0x3)
#define PMU_FLT1_FECQ_ID         (0xc)

#define PMU_STEP     1
#define LB_E_MAX     80
#define PMU_IDX_MAX  8
#define LB_USER      BIT(0)
#define LB_KERN      BIT(1)
#define LB_INT       BIT(2)
#define LB_POLL      BIT(3)

#define SLC_PMU_ADDR(addr, slc_id, id) \
	((addr) + ((slc_id) + 1) * 0x10000 + (id) * 0x4)

typedef void (*cfg_func)(u32 reg0, u32 reg1, u32 reg2, u32 reg3);

struct lb_event {
	char *name;
	u32 cfg0;
	u32 cfg1;
	u32 cfg2;
	u32 cfg3;
	u32 wk;
	u32 mode;
	u32 countable;
	cfg_func ops;
};

struct lb_counter {
	int stat;
	int eno;
	int cno;
	int mode;

	u32 cfg0;
	u32 cfg1;
	u32 cfg2;
	u32 cfg3;
	struct list_head node;
};

struct lb_pmu_man {
	int mnr;
	int cur;
	int bitmap;
	spinlock_t lock;
	lb_device *lbd;
	struct list_head active;
};

static struct lb_pmu_man man;

static inline u32 pmu_set_bits(u32 old, u32 val, u32 mask, u32 shift)
{
	old &= ~(mask << shift);
	return (old | ((val & mask) << shift));
}

static void pmu_slc_no_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
}

/*
* bit0 ~ bit2: pmu_dmemif_rbuf_th
*/
static void pmu_dmemif_th_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_DMEMIF_TH);
}

/*
* bit0 ~ bit3: pmu_mst2sn_flt
*/
static void pmu_mst2sn_flt_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_MST2SN_FLT);
}

/*
* bit0 ~ bit1: pmu_mstif_pri_bp
* bit8 ~ bit11: pmu_mstif_rbuf_th
*/
static void pmu_mstif_th_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_MSTIF_TH);
}

/*
* bit0 ~ bit6: pmu_bypbuf_th0
* bit8 ~ bit14: pmu_bypbuf_th1
*/
static void pmu_bypbuf_th_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_BYPBUF_TH);
}

/*
* reg0: bit[5:0] gid filter0 config, bit[21:16] gid filter0 mask config.
* reg1: bit[7:0] mid filter0 config, bit[23:16] mid filter0 mask config.
* reg2: bit[3:0] mpam filter0 config, bit[11:8] mpam filter0 mask config.
* reg3: bit[1:0] pri0 filter0 config, bit[9:8] pri0 filter0 mask config.
*/
static void pmu_tagpipe_en0_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	u32 v;

	/* gid filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_GID);
	v = pmu_set_bits(v, reg0, PMU_FLT_GID, PMU_FLT0_GID_SHIFT);
	v = pmu_set_bits(v, reg0, PMU_FLT_GID, PMU_FLT0_GID_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_GID);

	/* mid filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_MID);
	v = pmu_set_bits(v, reg1, PMU_FLT_MID, PMU_FLT0_MID_SHIFT);
	v = pmu_set_bits(v, reg1, PMU_FLT_MID, PMU_FLT0_MID_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_MID);

	/* mpam filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_MPAM);
	v = pmu_set_bits(v, reg2, PMU_FLT_MPAM, PMU_FLT0_MPAM_SHIFT);
	v = pmu_set_bits(v, reg2, PMU_FLT_MPAM, PMU_FLT0_MPAM_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_MPAM);

	/* pri filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_PRI);
	v = pmu_set_bits(v, reg3, PMU_FLT_PRI, PMU_FLT0_PRI_SHIFT);
	v = pmu_set_bits(v, reg3, PMU_FLT_PRI, PMU_FLT0_PRI_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_PRI);

	/* fecq id filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_FECQ_ID);
	v |= PMU_FLT0_FECQ_ID;
	writel(v, man.lbd->base + SC_PMU_FLT_FECQ_ID);
}

/*
* reg0: bit[11:6] gid filter1 config, bit[27:22] gid filter1 mask config.
* reg1: bit[15:8] mid filter1 config, bit[31:24] mid filter1 mask config.
* reg2: bit[7:4] mpam filter1 config, bit[15:12] mpam filter1 mask config.
* reg3: bit[3:2] pri1 filter1 config, bit[11:10] pri1 filter1 mask config.
*/
static void pmu_tagpipe_en1_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	u32 v;

	/* gid filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_GID);
	v = pmu_set_bits(v, reg0, PMU_FLT_GID, PMU_FLT1_GID_SHIFT);
	v = pmu_set_bits(v, reg0, PMU_FLT_GID, PMU_FLT1_GID_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_GID);

	/* mid filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_MID);
	v = pmu_set_bits(v, reg1, PMU_FLT_MID, PMU_FLT1_MID_SHIFT);
	v = pmu_set_bits(v, reg1, PMU_FLT_MID, PMU_FLT1_MID_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_MID);

	/* mpam filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_MPAM);
	v = pmu_set_bits(v, reg2, PMU_FLT_MPAM, PMU_FLT1_MPAM_SHIFT);
	v = pmu_set_bits(v, reg2, PMU_FLT_MPAM, PMU_FLT1_MPAM_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_MPAM);

	/* pri filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_PRI);
	v = pmu_set_bits(v, reg3, PMU_FLT_PRI, PMU_FLT1_PRI_SHIFT);
	v = pmu_set_bits(v, reg3, PMU_FLT_PRI, PMU_FLT1_PRI_MASK_SHIFT);
	writel(v, man.lbd->base + SC_PMU_FLT_PRI);

	/* fecq id filter config */
	v = readl(man.lbd->base + SC_PMU_FLT_FECQ_ID);
	v |= PMU_FLT1_FECQ_ID;
	writel(v, man.lbd->base + SC_PMU_FLT_FECQ_ID);
}

/*
* bit0 ~ bit3: tpipe_pipe_req_type0
* bit8 ~ bit11: tpipe_pipe_req_type1
*/
static void pmu_tpipespec_req_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_TPIPESPEC_REQ);
}

/*
* bit0 ~ bit2: tpipe_spec_fail_type0
* bit8 ~ bit10: tpipe_spec_fail_type1
*/
static void pmu_tpipespec_fail_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_TPIPESPEC_FAIL);
}

/*
* bit0 ~ bit7: pmu_qos_allow_from_sc
*/
static void pmu_qos_allow_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_QOSALLOW_SC);
}

static void pmu_becq_fsm_en0_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_BECQ_FSM_EN0);
}

static void pmu_becq_fsm_en1_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_BECQ_FSM_EN1);
}

/*
* bit0~bit4: pmu_lpid_msk
*/
static void pmu_lpid_flt_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_LPID_FLT);
}

/*
* bit0~bit19: pmu_tpipe_haz_type
*/
static void pmu_tpipe_haz_type_cfg(u32 reg0, u32 reg1, u32 reg2, u32 reg3)
{
	lb_print(LB_INFO, "0x%x\n", reg0);
	writel(reg0, man.lbd->base + SC_PMU_TPIPE_HAZ_TYPE);
}

static struct lb_event etbl[LB_E_MAX] = {
#include "mm_lb_event_v550.h"
};

/* pmu config */
static void pmu_glb_en(void)
{
	/* PMU clk-gate bypass */
	set_bits(BIT(2), man.lbd->base + SC_PMU_CFG);

	/* PMU enable */
	set_bits(BIT(0), man.lbd->base + SC_PMU_CFG);
}

static void pmu_capture(void)
{
	writel(0x1, man.lbd->base + SC_PMU_CAPR);
}

static void pmu_start_count(int cnt)
{
	u32 reg;

	if (cnt >= PMU_IDX_MAX) {
		lb_print(LB_ERROR, "invalid cnt %d\n", cnt);
		return;
	}

	reg = readl(man.lbd->base + SC_PMU_CNTEN);
	writel((reg | BIT(cnt)), man.lbd->base + SC_PMU_CNTEN);
}


static void pmu_end_count(int cnt)
{
	u32 reg;

	if (cnt >= PMU_IDX_MAX) {
		lb_print(LB_ERROR, "invalid cnt %d\n", cnt);
		return;
	}

	reg = readl(man.lbd->base + SC_PMU_CNTEN);
	writel(reg & (~BIT(cnt)), man.lbd->base + SC_PMU_CNTEN);
}

static void pmu_map_event(int cnt, int event, int mode)
{
	int i;

	if (cnt >= PMU_IDX_MAX) {
		lb_print(LB_ERROR, "invalid cnt %d\n", cnt);
		return;
	}
	/* set pmu event type */
	for (i = 0; i < man.lbd->slc_idx; i += PMU_STEP)
		writel(event, man.lbd->base + SLC_PMU_ADDR(SC_PMU_EVTYPE, i, cnt));
}

static void pmu_unmap_event(int cnt)
{
	int i;

	if (cnt >= PMU_IDX_MAX) {
		lb_print(LB_ERROR, "invalid cnt %d\n", cnt);
		return;
	}
	for (i = 0; i < man.lbd->slc_idx; i += PMU_STEP)
		writel(0x0, man.lbd->base + SLC_PMU_ADDR(SC_PMU_EVTYPE, i, cnt));
}

static u64 pmu_show(int cnt)
{
	int i;
	u64 sum = 0;

	for (i = 0; i < man.lbd->slc_idx; i += PMU_STEP)
		sum += readl(man.lbd->base + SLC_PMU_ADDR(SC_PMU_EVSVR, i, cnt));

	return sum;
}

static int __find_bit(struct lb_pmu_man *pman, int spbit, int mode)
{
	int i;
	int s = 0;

	spin_lock(&pman->lock);

	/* static counter map */
	if (spbit) {
		i = spbit;
		goto static_bit;
	}

	/* it is mean first fit */
	for (i = s; i < pman->mnr; i++) {
		if (!(pman->bitmap & BIT(i)))
			break;
	}

	/* i is overflow */
	if (i >= pman->mnr) {
		spin_unlock(&pman->lock);
		return -1;
	}

static_bit:
	pman->bitmap |= BIT(i);

	spin_unlock(&pman->lock);

	return i;
}

static void __reset_bit(struct lb_pmu_man *pman, int i)
{
	if (i >= pman->mnr)
		return;

	spin_lock(&pman->lock);

	pman->bitmap &= ~BIT(i);

	spin_unlock(&pman->lock);
}

static struct lb_counter *create_counter(
	struct lb_pmu_man *pman, struct lb_counter input)
{
	int nr;
	struct lb_counter *lbc = NULL;

	/* just for the static map */
	nr = __find_bit(pman, etbl[input.eno].countable, etbl[input.eno].mode);
	if (nr < 0) {
		lb_print(LB_ERROR, "pmu counter alloc is failed\n");
		goto out;
	}

	lbc = kzalloc(sizeof(*lbc), GFP_KERNEL);
	if (!lbc) {
		lb_print(LB_ERROR, "kzalloc lb counter is failed\n");
		goto find_bit;
	}

	lbc->eno = input.eno;
	lbc->cno = nr;
	lbc->cfg0 = input.cfg0;
	lbc->cfg1 = input.cfg1;
	lbc->cfg2 = input.cfg2;
	lbc->cfg3 = input.cfg3;
	lbc->stat = LB_USER;
	lbc->mode = etbl[input.eno].mode;

	/* map event */
	pmu_map_event(lbc->cno, lbc->eno, lbc->mode);

	return lbc;

find_bit:
	__reset_bit(pman, nr);
out:
	return lbc;
}

static struct lb_counter *find_counter(struct lb_pmu_man *pman, int eno)
{
	bool flag = 0;
	struct lb_counter *pos = NULL;
	struct lb_counter *next = NULL;

	spin_lock(&pman->lock);

	list_for_each_entry_safe(pos, next, &man.active, node) {
		if (pos->eno == eno) {
			flag = true;
			break;
		}
	}

	spin_unlock(&pman->lock);

	return flag ? pos : NULL;
}

static void set_counter(struct lb_pmu_man *pman, struct lb_counter *cnter)
{
	u32 reg0, reg1, reg2, reg3;

	if (cnter->cfg0 || cnter->cfg1 || cnter->cfg2 || cnter->cfg3) {
		reg0 = cnter->cfg0;
		reg1 = cnter->cfg1;
		reg2 = cnter->cfg2;
		reg3 = cnter->cfg3;
	} else {
		reg0 = etbl[cnter->eno].cfg0;
		reg1 = etbl[cnter->eno].cfg1;
		reg2 = etbl[cnter->eno].cfg2;
		reg3 = etbl[cnter->eno].cfg3;
	}

	etbl[cnter->eno].ops(reg0, reg1, reg2, reg3);
}

static void add_counter(struct lb_pmu_man *pman, struct lb_counter *cnter)
{
	spin_lock(&pman->lock);
	list_add(&cnter->node, &pman->active);
	spin_unlock(&pman->lock);
}

static void del_counter(struct lb_pmu_man *pman, struct lb_counter *cnter)
{
	spin_lock(&pman->lock);
	list_del(&cnter->node);
	spin_unlock(&pman->lock);
}

static void reset_counter(struct lb_pmu_man *pman, struct lb_counter *cnter)
{
	etbl[cnter->eno].ops(0, 0, 0, 0);
}

static void distory_counter(struct lb_pmu_man *pman, struct lb_counter *cnter)
{
	int nr = cnter->cno;

	kfree(cnter);

	pmu_unmap_event(nr);

	__reset_bit(pman, nr);
}

static void start_count(struct lb_pmu_man *pman)
{
	struct lb_counter *pos = NULL;
	struct lb_counter *next = NULL;

	spin_lock(&pman->lock);

	pmu_glb_en();

	list_for_each_entry_safe(pos, next, &pman->active, node)
		pmu_start_count(pos->cno);

	spin_unlock(&pman->lock);
}

static void end_count(struct lb_pmu_man *pman)
{
	struct lb_counter *pos = NULL;
	struct lb_counter *next = NULL;

	spin_lock(&pman->lock);
	pmu_capture();
	list_for_each_entry_safe(pos, next, &pman->active, node)
		pmu_end_count(pos->cno);

	spin_unlock(&pman->lock);
}

/* list all events */
static ssize_t list_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int i;
	int ret = 0;
	char *str = buf;
	ssize_t len = PAGE_SIZE;

	if (!man.lbd) {
		lb_print(LB_ERROR, "lb device is not init\n");
		return -EINVAL;
	}

	for (i = 0; i < LB_E_MAX; i++) {
		ret += scnprintf(str + ret, len - ret,
				"[%d]:%s\n", i, etbl[i].name);
		if (unlikely(len < ret))
			break;
	}

	return ret;
}

/* cat the enable event */
static ssize_t poll_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int ret = 0;
	char *str = buf;
	ssize_t len = PAGE_SIZE;
	struct lb_counter *pos = NULL;
	struct lb_counter *next = NULL;

	if (!man.lbd) {
		lb_print(LB_ERROR, "lb device is not init\n");
		return -EINVAL;
	}

	spin_lock(&man.lock);
	list_for_each_entry_safe(pos, next, &man.active, node) {
		ret += scnprintf(str + ret, len - ret,
				"event[%d]:%s counter[%d] result[0x%llx]\n",
				pos->eno, etbl[pos->eno].name, pos->cno, pmu_show(pos->cno));
		if (unlikely(ret >= len - 1))
			break;
	}
	spin_unlock(&man.lock);

	return ret;
}

/* create events */
static ssize_t set_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	char c;
	struct lb_counter input;
	struct lb_counter *cnter = NULL;

	if (!man.lbd) {
		lb_print(LB_ERROR, "pmu is not init\n");
		return -EINVAL;
	}

	/* parse the buf or called cmd */
	(void)sscanf_s(buf, "%c %d %u %u %u %u",
		&c, sizeof(c), &input.eno, &input.cfg0, &input.cfg1, &input.cfg2, &input.cfg3);
	if (input.eno > LB_E_MAX - 1 || input.eno < 0) {
		lb_print(LB_ERROR, "invalid eno %d\n", input.eno);
		return -EINVAL;
	}

	switch (c) {
	case 'a':
		/* if countable != 0, it mean event use static counter */
		cnter = create_counter(&man, input);
		if (!cnter) {
			lb_print(LB_ERROR, "failed to create counter\n");
			return -EINVAL;
		}

		set_counter(&man, cnter);
		add_counter(&man, cnter);

		break;

	case 'd':
		cnter = find_counter(&man, input.eno);
		if (!cnter)
			return -EINVAL;

		reset_counter(&man, cnter);
		del_counter(&man, cnter);
		distory_counter(&man, cnter);

		break;

	default:
		return -EINVAL;
	}

	return len;
}

static ssize_t onoff_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t len)
{
	char c;

	if (!man.lbd) {
		lb_print(LB_ERROR, "pmu is not init\n");
		return -EINVAL;
	}

	/* parse the buf or called cmd */
	(void)sscanf_s(buf, "%c", &c, sizeof(c));
	switch (c) {
	/* on */
	case 's':
		start_count(&man);

		lb_print(LB_ERROR, "on branch\n");
		break;

	/* off */
	case 'e':
		end_count(&man);

		lb_print(LB_ERROR, "off branch\n");
		break;

	default:

		lb_print(LB_ERROR, "default branch\n");
		return -EINVAL;
	}

	return len;
}

static DEVICE_ATTR_RO(list);
static DEVICE_ATTR_RO(poll);
static DEVICE_ATTR_WO(set);
static DEVICE_ATTR_WO(onoff);

static struct attribute *lb_pmu_attrs[] = {
	&dev_attr_list.attr,
	&dev_attr_poll.attr,
	&dev_attr_set.attr,
	&dev_attr_onoff.attr,

	NULL,
};

static struct attribute_group lb_pmu_attr_group = {
	.attrs = lb_pmu_attrs,
};

int lb_pmu_init(struct platform_device *pdev, lb_device *lbd)
{
	int ret;

	man.lbd = lbd;
	man.bitmap = 0;
	man.mnr = PMU_IDX_MAX;
	man.cur = 0;

	spin_lock_init(&man.lock);
	INIT_LIST_HEAD(&man.active);

	ret = sysfs_create_group(&pdev->dev.kobj, &lb_pmu_attr_group);
	if (ret < 0) {
		lb_print(LB_ERROR, "failed to create sysfs group for device\n");
		goto sysfs_err;
	}

	return 0;

sysfs_err:
	memset_s(&man, sizeof(man), 0x0, sizeof(man));

	return ret;
}
