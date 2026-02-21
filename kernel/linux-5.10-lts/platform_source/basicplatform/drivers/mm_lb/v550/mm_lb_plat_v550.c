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

#include <linux/io.h>
#include <linux/delay.h>
#include <linux/oom.h>
#include <securec.h>
#include <linux/pagewalk.h>
#include <dsm/dsm_pub.h>
#include <linux/interrupt.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>

#include "mm_lb_plat_v550.h"

#define POLL_TIMEOUT_US       1000000
#define CGID_BYP_TIMEOUT_US   1000
#define DSM_LB_ECC_SINGLE_BIT 925205200
#define DSM_LB_ECC_MULTI_BIT  925205201
#define SINGLE_BIT_EVENT_MAX  10
#define MULTI_BIT_EVENT_MAX   10
#define ABNML_INT_NS_STEP     32
#define ABNML_INT_NS_IDX      4
#define DFX_INFO_NUM          5
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

typedef enum {
	NON_ERR,
	SINGLE_BIT_ERR,
	MULTI_BIT_ERR,
	OTHER_ERR,
} ecc_err_type;

typedef struct {
	u32 intr;
	char *info_msg;
	void (*post_process)(u32* err_info, u32 info_num);
} lb_dfx_type;

typedef struct {
	u32 intr;
	ecc_err_type err_type;
} lb_ecc_type;

#ifdef CONFIG_MM_LB_V560
lb_dfx_type lb_dfx_info[] = {
	{0,  "access deep sleep sf ram", NULL},
	{1,  "access deep sleep tag ram", NULL},
	{2,  "sf ram 1 bit ecc err", NULL},
	{3,  "sf ram 1 bit ecc err hard", NULL},
	{4,  "sf ram 2 bit ecc err", NULL},
	{5,  "tag ram 1 bit ecc err", NULL},
	{6,  "tag ram 1 bit ecc err hard", NULL},
	{7,  "tag ram 2 bit ecc err", NULL},
	{8,  "ns invalid cfgcmo for sec gid, inv has been converted to clean&inv", NULL},
	{10, "tag multi dirty", NULL},
	{12, "tag hit, cgid unmatch", NULL},
	{14, "buffer mode gid alloc cacheline fail", NULL},
	{29, "receive pgid disable and drop eq 1", NULL},
	{30, "access syncbuf which way is cache mode", NULL},
	{33, "receive srsp txnid err", NULL},
	{79, "access deep sleep waygrp", NULL},
	{80, "access way which is in init status", NULL},
	{81, "access way in static low power state", NULL},
	{83, "dram 1 bit ecc err", NULL},
	{84, "dram 2 bit ecc err", NULL},
};

lb_ecc_type lb_ecc_info[] = {
	{2, SINGLE_BIT_ERR},
	{4, MULTI_BIT_ERR},
	{5, SINGLE_BIT_ERR},
	{7, MULTI_BIT_ERR},
	{83, SINGLE_BIT_ERR},
	{84, MULTI_BIT_ERR},
};
#else
#if defined(CONFIG_MM_LB_ENG_DEBUG) && !defined(CONFIG_LIBLINUX)
#define PTE_PHY_MASK 0xFFFFFFFFF000
struct walk_private {
	struct page *page;
	struct task_struct *task;
};

static void print_kernel_map(unsigned long addr, u64 val, struct mm_walk *walk)
{
	pid_t mm_pid = 0, cur_pid = 0;
	char *mm_comm = "kernel", *cur_comm = "kernel";
	struct task_struct *mm_owner = walk->no_vma ? walk->mm->owner : walk->vma->vm_mm->owner;
	struct task_struct *cur_owner = ((struct walk_private*)walk->private)->task;
	struct page *page = ((struct walk_private*)walk->private)->page;

	if (mm_owner) {
		mm_pid = cur_pid = mm_owner->pid;
		mm_comm = cur_comm = mm_owner->comm;
	}

	if (cur_owner) {
		cur_pid = cur_owner->pid;
		cur_comm = cur_owner->comm;
	}

	if ((val & PTE_PHY_MASK) == page_to_phys(page))
		printk(KERN_ERR "0x%-16llx 0x%-16llx %-8d %-17s %-8d %-17s\n", \
			addr, val, cur_pid, cur_comm, mm_pid, mm_comm);
	return;
}

static int pageattr_pgd_entry(pgd_t *pgd, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pgd_t val = READ_ONCE(*pgd);
	if (pgd_leaf(val))
		print_kernel_map(addr, pgd_val(val), walk);
	return 0;
}

static int pageattr_p4d_entry(p4d_t *p4d, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	p4d_t val = READ_ONCE(*p4d);
	if (p4d_leaf(val))
		print_kernel_map(addr, p4d_val(val), walk);
	return 0;
}

static int pageattr_pud_entry(pud_t *pud, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pud_t val = READ_ONCE(*pud);
	if (pud_leaf(val))
		print_kernel_map(addr, pud_val(val), walk);
	return 0;
}

static int pageattr_pmd_entry(pmd_t *pmd, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pmd_t val = READ_ONCE(*pmd);
	if (pmd_leaf(val))
		print_kernel_map(addr, pmd_val(val), walk);
	return 0;
}

static int pageattr_pte_entry(pte_t *pte, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
{
	pte_t val = READ_ONCE(*pte);
	print_kernel_map(addr, pte_val(val), walk);
	return 0;
}

static int pageattr_pte_hole(unsigned long addr, unsigned long next,
			     int depth, struct mm_walk *walk)
{
	/* Nothing to do here */
	return 0;
}

static int pageattr_test_walk(unsigned long addr, unsigned long next,
			     struct mm_walk *walk)
{
	/* Nothing to do here */
	return 0;
}

static const struct mm_walk_ops kmap_walk_ops = {
	.pgd_entry = pageattr_pgd_entry,
	.p4d_entry = pageattr_p4d_entry,
	.pud_entry = pageattr_pud_entry,
	.pmd_entry = pageattr_pmd_entry,
	.pte_entry = pageattr_pte_entry,
	.pte_hole = pageattr_pte_hole,
	.test_walk = pageattr_test_walk,
};

static void kmap_walk_mm(struct mm_struct *mm, struct walk_private *private)
{
	struct vm_area_struct *vma;

	if (!mm->mmap)
		(void)walk_page_range_novma(mm, 0, PAGE_OFFSET, &kmap_walk_ops, NULL, private);

	for (vma = mm->mmap; vma != NULL; vma = vma->vm_next)
		(void)walk_page_vma(vma, &kmap_walk_ops, private);
}

static void task_mm_walk(struct page* page)
{
	struct task_struct *p, *t;
	struct walk_private private = {
		.page = page,
	};

	mmap_read_lock(&init_mm);
	(void)walk_page_range_novma(&init_mm, PAGE_OFFSET, ~0UL, &kmap_walk_ops, NULL, &private);
	mmap_read_unlock(&init_mm);

	rcu_read_lock();
	for_each_process(p) {
		if (p->flags & PF_KTHREAD)
			continue;

		t = find_lock_task_mm(p);
		if (!t)
			continue;

		private.task = t;
		(void)kmap_walk_mm(t->mm, &private);
		task_unlock(t);
	};
	rcu_read_unlock();
}

#define dfx_err_info_req_phy(info) \
	((((u64)info[1] & 0x7FUL) << 29) | (info[0] >> 3))
#define dfx_err_info_req_pgid(info) \
	((info[1] & 0x3F000UL) >> 12)
#define dfx_err_info_req_mid(info) \
	((err_info[3] & 0xFE000000UL) >> 25)
#define dfx_err_info_hit_pgid(info) \
	((err_info[4] & 0x7E000UL) >> 13)

static void post_process_pgid_hit_cgid_mismatch(u32* err_info, u32 info_num)
{
	u64 phys;
	struct page* page;

	if (info_num < DFX_INFO_NUM)
		return;

	phys = dfx_err_info_req_phy(err_info);

	if (!pfn_valid(__phys_to_pfn(phys)))
		return;

	page = phys_to_page(phys);
	printk(KERN_ERR "phys[0x%llx] page[0x%llx] req pgid[%d] req mid[%d] hit pgid[%d]\n", \
		phys, page, dfx_err_info_req_pgid(err_info), \
		dfx_err_info_req_mid(err_info), dfx_err_info_hit_pgid(err_info));
	printk(KERN_ERR "virt               pte                cur pid  cur task          mm pid   mm task\n");
	task_mm_walk(page);

	return;
}
#else
static inline void post_process_pgid_hit_cgid_mismatch(u32* err_info, u32 info_num) { };
#endif

lb_dfx_type lb_dfx_info[] = {
	{1,  "access deep sleep tag ram", NULL},
	{2,  "sf ram 1 bit ecc err", NULL},
	{3,  "sf ram 1 bit ecc err hard", NULL},
	{4,  "sf ram 2 bit ecc err", NULL},
	{5,  "tag ram 1 bit ecc err", NULL},
	{6,  "tag ram 1 bit ecc err hard", NULL},
	{7,  "tag ram 2 bit ecc err", NULL},
	{8,  "ns invalid cfgcmo for sec gid, inv has been converted to clean&inv", NULL},
	{9,  "tag multi dirty", NULL},
	{11, "tag hit, cgid unmatch", post_process_pgid_hit_cgid_mismatch},
	{13, "buffer mode gid alloc cacheline fail", NULL},
	{78, "access deep sleep waygrp", NULL},
	{79, "access way which is in initing status", NULL},
	{80, "access way in static low power state", NULL},
	{82, "dram 1 bit ecc err", NULL},
	{83, "dram 2 bit ecc err", NULL},
};

lb_ecc_type lb_ecc_info[] = {
	{2, SINGLE_BIT_ERR},
	{4, MULTI_BIT_ERR},
	{5, SINGLE_BIT_ERR},
	{7, MULTI_BIT_ERR},
	{82, SINGLE_BIT_ERR},
	{83, MULTI_BIT_ERR},
};
#endif

static int single_bit = 0;
static int multi_bit = 0;
static u32 abnml_int_ns_mask[ABNML_INT_NS_IDX] = {0};

#ifdef CONFIG_MM_LB_V560
static void pack_pgid_attr(pgid_t *pgid, hw_reg_pgid *pgid_reg)
{
	if (!pgid || !pgid_reg) {
		lb_print(LB_ERROR, "%s input NULL!");
		return;
	}

	// pack pgid_drv_cfg
	pgid_reg->pgid_drv_cfg.value = 0;
	pgid_reg->pgid_drv_cfg.reg.pgid_paf_en = pgid->paf_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_prefetch_en = pgid->prefetch_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_autoclean_en = pgid->autoclean_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_schint_en = pgid->schint_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_keep_cleansh = pgid->keep_cleansh;
	pgid_reg->pgid_drv_cfg.reg.pgid_vacmo_en = pgid->vacmo_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_stash_en = pgid->stash_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_trigger_en = pgid->trigger_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_mode = pgid->mode;
	pgid_reg->pgid_drv_cfg.reg.pgid_sot_ignore = pgid->sot_ignore;
	pgid_reg->pgid_drv_cfg.reg.pgid_passdirty_en = pgid->passdirty_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_cgid_idx =  pgid->cgid_idx;
	pgid_reg->pgid_drv_cfg.reg.pgid_writeevict_en = pgid->writeevict_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_hit_age_byp = pgid->hit_age_byp;

	// pack pgid_mst_cfg
	pgid_reg->pgid_mst_cfg.value = 0;
	pgid_reg->pgid_mst_cfg.reg.pgid_wr_revisit_age = pgid->wr_revisit_age;
	pgid_reg->pgid_mst_cfg.reg.pgid_nwr_revisit_age = pgid->nwr_revisit_age;
	pgid_reg->pgid_mst_cfg.reg.pgid_initial_age = pgid->initial_age;
	pgid_reg->pgid_mst_cfg.reg.pgid_wr_policy = pgid->wr_policy;
	pgid_reg->pgid_mst_cfg.reg.pgid_rd_policy = pgid->rd_policy;
	pgid_reg->pgid_mst_cfg.reg.pgid_priority = pgid->priority;
	pgid_reg->pgid_mst_cfg.reg.pgid_self_replaceable = pgid->self_replaceable;
	pgid_reg->pgid_mst_cfg.reg.pgid_drop = 0; // do not set Read Only reg
	pgid_reg->pgid_mst_cfg.reg.pgid_drop_clr = !(pgid->drop); // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_drop_set = pgid->drop; // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_disable = 0; // do not set Read Only reg
	pgid_reg->pgid_mst_cfg.reg.pgid_disable_clr = !(pgid->disable); // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_disable_set = pgid->disable; // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_small_burst_size = pgid->small_burst_size;
	pgid_reg->pgid_mst_cfg.reg.pgid_small_burst_na = pgid->small_burst_na;
}
#else
static void pack_pgid_attr(pgid_t *pgid, hw_reg_pgid *pgid_reg)
{
	if (!pgid || !pgid_reg) {
		lb_print(LB_ERROR, "%s input NULL!");
		return;
	}

	// pack pgid_drv_cfg
	pgid_reg->pgid_drv_cfg.value = 0;
	pgid_reg->pgid_drv_cfg.reg.pgid_small_burst_na = pgid->small_burst_na;
	pgid_reg->pgid_drv_cfg.reg.pgid_paf_en = pgid->paf_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_prefetch_en = pgid->prefetch_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_autoclean_en = pgid->autoclean_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_schint_en = pgid->schint_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_keep_cleansh = pgid->keep_cleansh;
	pgid_reg->pgid_drv_cfg.reg.pgid_vacmo_en = pgid->vacmo_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_stash_en = pgid->stash_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_trigger_en = pgid->trigger_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_mode = pgid->mode;
	pgid_reg->pgid_drv_cfg.reg.pgid_sot_ignore = pgid->sot_ignore;
	pgid_reg->pgid_drv_cfg.reg.pgid_passdirty_en = pgid->passdirty_en;
	pgid_reg->pgid_drv_cfg.reg.pgid_cgid_idx =  pgid->cgid_idx;
#ifndef CONFIG_MM_LB_GID_PBHA
	/*
	 * Hardware sgid attr is only used for PA mode.
	 * On PBHA mode, it should be RES0.
	 * We init sgid in dt probe only for gpu's lb_alloc/free_pages.
	 */
	pgid_reg->pgid_drv_cfg.reg.pgid_sgid_idx = pgid->sgid_idx;
#endif
	pgid_reg->pgid_drv_cfg.reg.pgid_writeevict_en = pgid->writeevict_en;

	// pack pgid_mst_cfg
	pgid_reg->pgid_mst_cfg.value = 0;
	pgid_reg->pgid_mst_cfg.reg.pgid_revisit_age = pgid->revisit_age;
	pgid_reg->pgid_mst_cfg.reg.pgid_initial_age0 = pgid->initial_age0;
	pgid_reg->pgid_mst_cfg.reg.pgid_initial_age1 = pgid->initial_age1;
	pgid_reg->pgid_mst_cfg.reg.pgid_wr_policy = pgid->wr_policy;
	pgid_reg->pgid_mst_cfg.reg.pgid_rd_policy = pgid->rd_policy;
	pgid_reg->pgid_mst_cfg.reg.pgid_priority = pgid->priority;
	pgid_reg->pgid_mst_cfg.reg.pgid_self_replaceable = pgid->self_replaceable;
	pgid_reg->pgid_mst_cfg.reg.pgid_drop = 0; // do not set Read Only reg
	pgid_reg->pgid_mst_cfg.reg.pgid_drop_clr = !(pgid->drop); // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_drop_set = pgid->drop; // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_disable = 0; // do not set Read Only reg
	pgid_reg->pgid_mst_cfg.reg.pgid_disable_clr = !(pgid->disable); // W1PULSE
	pgid_reg->pgid_mst_cfg.reg.pgid_disable_set = pgid->disable; // W1PULSE
}
#endif

#ifdef CONFIG_MM_LB_V560
static void pack_cgid_attr(cgid_t *cgid, hw_reg_cgid *cgid_reg)
{
	if (!cgid || !cgid_reg) {
		lb_print(LB_ERROR, "%s input NULL!");
		return;
	}

	cgid_reg->cgid_way_allc.value = 0;
	cgid_reg->cgid_way_search.value = 0;
	if (!cgid->bypass) {
		cgid_reg->cgid_way_allc.reg.cgid_way_enable = cgid->way_enable;
		cgid_reg->cgid_way_search.reg.cgid_way_search = cgid->way_search;
	}

	cgid_reg->cgid_cfg.value = 0;
	if (!cgid->bypass)
		cgid_reg->cgid_quota.reg.cgid_quota = cgid->quota;
	cgid_reg->cgid_cfg.reg.cgid_self_replaceable = cgid->self_replaceable;
	cgid_reg->cgid_cfg.reg.cgid_bypass_en = cgid->bypass;
	cgid_reg->cgid_cfg.reg.cgid_tag_pgid_chg_en = cgid->tag_pgid_chg_en;

	lb_print(LB_ERROR, "cgid_way_allc: 0x%lx, cgid_cfg: 0x%lx\n",
		cgid_reg->cgid_way_allc.value, cgid_reg->cgid_cfg.value);
}
#else
static void pack_cgid_attr(cgid_t *cgid, hw_reg_cgid *cgid_reg)
{
	if (!cgid || !cgid_reg) {
		lb_print(LB_ERROR, "%s input NULL!");
		return;
	}

	cgid_reg->cgid_way_allc.value = 0;
	if (!cgid->bypass) {
		cgid_reg->cgid_way_allc.reg.cgid_way_enable = cgid->way_enable;
		cgid_reg->cgid_way_allc.reg.cgid_way_search = cgid->way_search;
	}

	cgid_reg->cgid_cfg.value = 0;
	if (!cgid->bypass)
		cgid_reg->cgid_cfg.reg.cgid_quota = cgid->quota;
	cgid_reg->cgid_cfg.reg.cgid_self_replaceable = cgid->self_replaceable;
	cgid_reg->cgid_cfg.reg.cgid_bypass_en = cgid->bypass;
	cgid_reg->cgid_cfg.reg.cgid_tag_pgid_chg_en = cgid->tag_pgid_chg_en;

	lb_print(LB_ERROR, "cgid_way_allc: 0x%lx, cgid_cfg: 0x%lx\n",
		cgid_reg->cgid_way_allc.value, cgid_reg->cgid_cfg.value);
}
#endif

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

static int register_check(u64 addr, u32 mask, u32 val, u32 timeout)
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

#ifdef CONFIG_MM_LB_V560
static int lb_cgid_bypass_config(u32 cgid, u32 bypass_en)
{
	u32 i;
	u64 addr;
	u32 quota;
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_quota.value = readl(SOC_SC_CGID_QUOTA_ADDR(lbdev->base, cgid));
	/*
	 * When enabling cgid, we should cfg bypass to 0
	 * before assigning any non-zero quota.
	 * We also assume that when the input bypass_en is 0,
	 * the register value of cgid_reg.bypass_en is currently 1,
	 * and the register value of cgid_reg.quota is 0.
	 */
	quota = cgid_reg.cgid_quota.reg.cgid_quota;
	WARN(quota != 0, "quota is assigned 0x%x before setting bypass_en to 0", quota);

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	cgid_reg.cgid_cfg.reg.cgid_bypass_en = bypass_en;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));

	for (i = 0; i < lbdev->slc_idx; i++) {
		addr = slc_addr(SOC_SC_CGID_STATUS0_ADDR((u64)lbdev->base, cgid), i);
		if (register_check(addr, BIT(16), bypass_en ? BIT(16) : 0, CGID_BYP_TIMEOUT_US)) {
			lb_print(LB_ERROR, "set cgid %u bypass %u failed!\n", cgid, bypass_en);
			return -1;
		}
	}

	return 0;
}

void cgid_validate(u32 cgid_idx)
{
	cgid_t *cgid = NULL;
	hw_reg_cgid cgid_reg = {0};

	if (!lbdev->cgid_repo.ops) {
		lb_print(LB_ERROR, "error: cgid_repo.ops NULL!");
		return;
	}

	cgid = lbdev->cgid_repo.ops->get(cgid_idx);
	if (!cgid) {
		lb_print(LB_ERROR, "error: cgid null!");
		return;
	}

	pack_cgid_attr(cgid, &cgid_reg);
	writel(cgid_reg.cgid_way_allc.value, SOC_SC_CGID_WAY_ALLC_ADDR(lbdev->base, cgid_idx));
	writel(cgid_reg.cgid_way_search.value, SOC_SC_CGID_WAY_SEARCH_ADDR(lbdev->base, cgid_idx));
	if (!cgid->bypass) {
		/* When enabling cgid, config bypass_en before assigning non-zero quota */
		if (lb_cgid_bypass_config(cgid_idx, cgid->bypass))
			WARN(1, "cgid bypass failed!");
	}
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
	writel(cgid_reg.cgid_quota.value, SOC_SC_CGID_QUOTA_ADDR(lbdev->base, cgid_idx));
}
#else
static int lb_cgid_bypass_config(u32 cgid, u32 bypass_en)
{
	u32 i;
	u64 addr;
	u32 quota;
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	/*
	 * When enabling cgid, we should cfg bypass to 0
	 * before assigning any non-zero quota.
	 * We also assume that when the input bypass_en is 0,
	 * the register value of cgid_reg.bypass_en is currently 1,
	 * and the register value of cgid_reg.quota is 0.
	 */
	quota = cgid_reg.cgid_cfg.reg.cgid_quota;
	WARN(quota != 0, "quota is assigned 0x%x before setting bypass_en to 0", quota);

	cgid_reg.cgid_cfg.reg.cgid_bypass_en = bypass_en;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));

	for (i = 0; i < lbdev->slc_idx; i++) {
		addr = slc_addr(SOC_SC_CGID_BYP_SWITCH_STATUS_ADDR((u64)lbdev->base, cgid), i);
		if (register_check(addr, BIT(0), bypass_en ? 1 : 0, CGID_BYP_TIMEOUT_US)) {
			lb_print(LB_ERROR, "set cgid %u bypass %u failed!\n", cgid, bypass_en);
			return -1;
		}
	}

	return 0;
}

void cgid_validate(u32 cgid_idx)
{
	cgid_t *cgid = NULL;
	hw_reg_cgid cgid_reg = {0};

	if (!lbdev->cgid_repo.ops) {
		lb_print(LB_ERROR, "error: cgid_repo.ops NULL!");
		return;
	}

	cgid = lbdev->cgid_repo.ops->get(cgid_idx);
	if (!cgid) {
		lb_print(LB_ERROR, "error: cgid null!");
		return;
	}

	pack_cgid_attr(cgid, &cgid_reg);
	writel(cgid_reg.cgid_way_allc.value, SOC_SC_CGID_WAY_ALLC_ADDR(lbdev->base, cgid_idx));
	if (!cgid->bypass) {
		/* When enabling cgid, config bypass_en before assigning non-zero quota */
		if (lb_cgid_bypass_config(cgid_idx, cgid->bypass))
			WARN(1, "cgid bypass failed!");
	}
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
}
#endif

void set_cgid_tag_pgid_chg_en_direct(u32 cgid_idx, bool enable)
{
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
	cgid_reg.cgid_cfg.reg.cgid_tag_pgid_chg_en = enable;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid_idx));
}

#ifdef CONFIG_MM_LB_V560
u32 clear_quota(u32 cgid)
{
	u32 r_quota;
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_quota.value = readl(SOC_SC_CGID_QUOTA_ADDR(lbdev->base, cgid));
	r_quota = cgid_reg.cgid_quota.reg.cgid_quota;
	cgid_reg.cgid_quota.reg.cgid_quota = 0;
	writel(cgid_reg.cgid_quota.value, SOC_SC_CGID_QUOTA_ADDR(lbdev->base, cgid));

	return r_quota;
}

void set_quota(u32 cgid, u32 quota)
{
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_quota.value = readl(SOC_SC_CGID_QUOTA_ADDR(lbdev->base, cgid));
	cgid_reg.cgid_quota.reg.cgid_quota = quota;
	writel(cgid_reg.cgid_quota.value, SOC_SC_CGID_QUOTA_ADDR(lbdev->base, cgid));
}
#else
u32 clear_quota(u32 cgid)
{
	u32 r_quota;
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	r_quota = cgid_reg.cgid_cfg.reg.cgid_quota;
	cgid_reg.cgid_cfg.reg.cgid_quota = 0;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));

	return r_quota;
}

void set_quota(u32 cgid, u32 quota)
{
	hw_reg_cgid cgid_reg;

	cgid_reg.cgid_cfg.value = readl(SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
	cgid_reg.cgid_cfg.reg.cgid_quota = quota;
	writel(cgid_reg.cgid_cfg.value, SOC_SC_CGID_CFG_ADDR(lbdev->base, cgid));
}
#endif

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

static void lb_dfx_parse(u32 intr_bitmap, u32 idx, u32* dfx_err_info, u32 info_num)
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
					set_bit(intr, SOC_SC_ABNML_INT_NS_MASK_ADDR(lbdev->base, i));

				return lb_ecc_info[j].err_type;
			}
		}
		return OTHER_ERR;
	}
	return NON_ERR;
}

static void lb_ecc_notify(ecc_err_type err_type)
{
	struct dsm_client *ai_client;

	if (!lbdev->dsm.ai_client)
		schedule_work(&(lbdev->dsm.dsm_work));

	ai_client = lbdev->dsm.ai_client;

	if (!ai_client) {
		lb_print(LB_ERROR, "ai_client is null\n");
		return;
	}

#ifdef CONFIG_MM_LB_ENG_DEBUG
	if (err_type == MULTI_BIT_ERR)
		rdr_syserr_process_for_ap(MODID_AP_S_PANIC_LB, 0ULL, 0ULL);
#else
	if ((err_type == MULTI_BIT_ERR) &&
		(multi_bit < MULTI_BIT_EVENT_MAX)) {
		if (!dsm_client_ocuppy(ai_client)) {
			dsm_client_record(ai_client, "lb ecc multi bit\n");
			dsm_client_notify(ai_client, DSM_LB_ECC_MULTI_BIT);
			multi_bit++;
		}
	} else if ((err_type == SINGLE_BIT_ERR) &&
		(single_bit < SINGLE_BIT_EVENT_MAX)) {
		if (!dsm_client_ocuppy(ai_client)) {
			dsm_client_record(ai_client, "lb ecc 1bit\n");
			dsm_client_notify(ai_client, DSM_LB_ECC_SINGLE_BIT);
			single_bit++;
		}
	}
#endif
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

static inline void start_flow(void)
{
	writel(1, SOC_SC_STATIS_GLB_EN_ADDR(lbdev->base));
}

static inline void end_flow(void)
{
	writel(0, SOC_SC_STATIS_GLB_EN_ADDR(lbdev->base));
}

u32 lb_set_flow(struct lb_flow_ctrl *flow)
{
	u32 idx = 0;
	SOC_SC_STATIS_SLICE_CTRL0_UNION ctrl0 = {0};
	SOC_SC_STATIS_SLICE_CTRL0_UNION ctrl1 = {0};

	ctrl0.value = flow->ctrl0;
	ctrl1.value = flow->ctrl1;
	ctrl0.reg.gid_statis_en = BIT(flow->pid);

	writel(ctrl0.value, SOC_SC_STATIS_SLICE_CTRL0_ADDR(lbdev->base, idx));
	writel(ctrl1.value, SOC_SC_STATIS_SLICE_CTRL1_ADDR(lbdev->base, idx));

	start_flow();

	return 0;
}

u32 lb_get_flow_in(void)
{
	int i;
	u32 idx = 0;
	u32 cnt = 0;

	end_flow();

	for (i = 0; i < lbdev->slc_idx; i++)
		cnt += readl(slc_addr(SOC_SC_STATIS_CNT_IN_ADDR(lbdev->base, idx), i));

	return cnt;
}

u32 lb_get_flow_out(void)
{
	int i;
	u32 idx = 0;
	u32 cnt = 0;

	end_flow();

	for (i = 0; i < lbdev->slc_idx; i++)
		cnt += readl(slc_addr(SOC_SC_STATIS_CNT_OUT_ADDR(lbdev->base, idx), i));

	return cnt;
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