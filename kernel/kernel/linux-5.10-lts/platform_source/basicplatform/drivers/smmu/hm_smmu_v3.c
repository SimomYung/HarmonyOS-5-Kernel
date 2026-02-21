/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: hisi smmu v3 driver
 * Date: 2024/12/07
 */

#define pr_fmt(fmt) "smmuv3:" fmt

#include <linux/acpi.h>
#include <linux/acpi_iort.h>
#include <linux/bitfield.h>
#include <linux/bitops.h>
#include <linux/crash_dump.h>
#include <linux/delay.h>
#include <linux/dma-iommu.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/iommu.h>
#include <linux/iopoll.h>
#include <linux/module.h>
#include <linux/msi.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_iommu.h>
#include <linux/of_platform.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/amba/bus.h>
#include <linux/sched/mm.h>
#include <linux/iommu/mm_iommu_dma.h>
#include <asm/pgtable.h>
#include <asm/cacheflush.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/bitops.h>
#include <uapi/linux/sched/types.h>
#include <linux/iommu/mm_svm.h>
#include <asm/memory.h>
#include <linux/io-pgtable.h>
#ifdef CONFIG_DMABUF_RECLAIM
#include <linux/dmabuf/dmabuf_reclaim.h>
#endif
#ifdef CONFIG_DMABUF_SWAP
#include <linux/dmabuf/dmabuf_swap.h>
#endif
#ifdef CONFIG_MM_LB
#include <linux/iommu/mm_iommu.h>
#include <linux/mm_lb/mm_lb.h>
#endif
#include <securec.h>
#include "hm_smmu_v3.h"

#ifdef CONFIG_MM_IOMMU_FAULT
#include "smmu_fault.h"

LIST_HEAD(smmu_dev_list);
#endif

LIST_HEAD(smmuv3_domain_list);

static struct iommu_ops arm_smmu_ops;
static struct platform_driver arm_smmu_driver;

struct arm_smmu_domain *to_smmu_domain(struct iommu_domain *dom)
{
	return container_of(dom, struct arm_smmu_domain, domain);
}

static u16 arm_smmu_get_ssid(struct device *dev)
{
	struct iommu_fwspec *fwspec = NULL;

	fwspec = dev->iommu->fwspec;

	if (fwspec->num_ids <= 1)
		return (u16)-1;

	return (u16)fwspec->ids[1];
}

static u32 arm_smmu_get_sid(struct iommu_fwspec *fwspec)
{
	if (fwspec->num_ids <= 0)
		return (u32)-1;

	return fwspec->ids[0];
}

static struct arm_smmu_device *arm_smmu_get_dev_smmu(struct device *dev)
{
	struct arm_smmu_master *master = NULL;

	if (!dev_iommu_fwspec_get(dev)) {
		dev_err(dev, "%s, iommu_fwspec is null\n", __func__);
		return NULL;
	}

	master = dev_iommu_priv_get(dev);
	if (!master) {
		dev_err(dev, "%s, iommu_priv is null\n", __func__);
		return NULL;
	}

	return master->smmu;
}

#ifdef CONFIG_MM_IOMMU_TEST
/* export for test, testcode call the func to get arm_smmu_device, declare in mm_smmu.h */
struct arm_smmu_device *test_arm_smmu_get_dev_smmu(struct device *dev)
{
	return arm_smmu_get_dev_smmu(dev);
}

/* export for test, testcode call the func to dump smmu pgtble, declare in mm_smmu.h */
void test_arm_smmu_dump_pgtbl(struct arm_smmu_device *smmu, pgd_t *base_pgd, unsigned long va,
				size_t pgtbl_size, u64 *dst, size_t buf_size)
{
	return;
}
#endif

void hm_mem_dump_pte(u64 ttbr, u64 va)
{
	hm_mem_dump_pte_with_pgd(0, ttbr, va);
}

static void arm_smmu_tlb_inv_context_dev(struct arm_smmu_domain *smmu_domain, u32 ssid)
{
	struct smmu_tlb_paras tlb_para = {};

	tlb_para.sid = smmu_domain->sid;
	tlb_para.ssid = ssid;
	liblinux_smmu_tlb_inv_context_dev(&tlb_para);
}

static void arm_smmu_tlb_inv_context(struct arm_smmu_domain *smmu_domain)
{
	struct smmu_tlb_paras tlb_para = {};

	tlb_para.sid = smmu_domain->sid;
	liblinux_smmu_tlb_inv_context(&tlb_para);
}

/*
 * DEV_FLUSH:
 * The master_dev's driver controls when to flush tlb and
 * which ssid should be flushed.
 *
 * WARN:
 * Since the param of the func arm_smmu_dev_flush_tlb is (domain, ssid)
 * and we can not get the base_ssid for the caller's dev from the param,
 * it is supposed that the s1_cfg.cd.ssid in this domain
 * is never changed once it has been created.
 * Thus, this flush type [DO NOT] support multi masters sharing one sid
 * (as this would cause the change of the domain's s1_cfg.cd.ssid).
 */
static int arm_smmu_dev_flush_tlb(struct iommu_domain *domain,
				  unsigned int ssid)
{
	struct arm_smmu_domain *smmu_domain = NULL;
	struct arm_smmu_device *smmu = NULL;
	struct smmu_tlb_paras tlb_para = {};

	smmu_domain = to_smmu_domain(domain);
	if (smmu_domain->tlb_flush != DEV_FLUSH) {
		pr_err("%s, tlb_flush %u is not DEV_FLUSH!\n",
			__func__, smmu_domain->tlb_flush);
		return -EPERM;
	}

	smmu = smmu_domain->smmu;
	if (!smmu) {
		pr_err("%s, smmu is null\n", __func__);
		return -ENODEV;
	}

	if (ssid >=  (1UL << SMMU_SSID_BITS)) {
		dev_err(smmu->dev, "%s ssid %u error\n", __func__, ssid);
		return -EINVAL;
	}

	tlb_para.sid = smmu_domain->sid;
	tlb_para.ssid = ssid;

	liblinux_smmu_tlb_inv_context_dev(&tlb_para);
	return 0;
}

/*
 * SSID_FLUSH:
 * When doing map/unmap, flush all ssid's tlb which belong to this dev.
 * Currently, each master that using SSID_FLUSH type has only one ssid attached to it.
 */
static void arm_smmu_flush_tlb(struct device *dev, struct iommu_domain *domain)
{
	struct arm_smmu_domain *smmu_domain = NULL;
	struct arm_smmu_master *master = NULL;
	struct smmu_tlb_paras tlb_para = {};

	smmu_domain = to_smmu_domain(domain);
	if (smmu_domain->tlb_flush != SSID_FLUSH)
		return;

	master = dev_iommu_priv_get(dev);
	if (!master) {
		dev_err(dev, "%s error, master is null\n", __func__);
		return;
	}

	tlb_para.sid = smmu_domain->sid;
	tlb_para.ssid = arm_smmu_get_ssid(dev);
	liblinux_smmu_tlb_inv_context_dev(&tlb_para);
}

#ifdef CONFIG_DMABUF_RECLAIM
static void arm_smmu_reclaim_flush_tlb(struct device *dev, struct iommu_domain *domain, unsigned long iova, size_t size)
{
	struct arm_smmu_domain *smmu_domain = NULL;

	if (!dev || !domain) {
		dev_err(dev, "%s, dev or domain is null\n", __func__);
		return;
	}

	smmu_domain = to_smmu_domain(domain);
	if (!smmu_domain) {
		dev_err(dev, "%s, smmu_domain is null\n", __func__);
		return;
	}

	arm_smmu_flush_tlb(dev, domain);
}
#endif

static void arm_smmu_flush_ttwc(struct iommu_domain *domain)
{
	struct arm_smmu_domain *smmu_domain = NULL;
	struct smmu_ttwc_paras para = {};

	smmu_domain = to_smmu_domain(domain);
	if (!smmu_domain->smmu) {
		pr_err("%s, smmu is null\n", __func__);
		return;
	}
	para.sid = smmu_domain->sid;

	liblinux_smmu_ttwc_inv_context(&para);
}

/* IOMMU API */
static bool arm_smmu_capable(enum iommu_cap cap)
{
	switch (cap) {
	case IOMMU_CAP_CACHE_COHERENCY:
		return true;
	case IOMMU_CAP_NOEXEC:
		return true;
	default:
		return false;
	}
}

static struct mm_smmu_domain *to_mm_smmu_domain(struct arm_smmu_domain *dom)
{
	return container_of(dom, struct mm_smmu_domain, smmu_domain);
}

static struct iommu_domain *arm_smmu_domain_alloc(unsigned int type)
{
	struct mm_smmu_domain *mm_domain = NULL;

	if (type != IOMMU_DOMAIN_UNMANAGED &&
	    type != IOMMU_DOMAIN_DMA &&
	    type != IOMMU_DOMAIN_IDENTITY)
		return NULL;

	/*
	 * Allocate the domain and initialise some of its data structures.
	 * We can't really do anything meaningful until we've added a
	 * master.
	 */
	mm_domain = kzalloc(sizeof(*mm_domain), GFP_KERNEL);
	if (!mm_domain)
		return NULL;

	mutex_init(&mm_domain->smmu_domain.init_mutex);

	/* dmabuf is not allocated through the svm */
	if (type != IOMMU_DOMAIN_UNMANAGED)
		list_add_tail(&mm_domain->list, &smmuv3_domain_list);
	return &mm_domain->smmu_domain.domain;
}

static int arm_smmu_alloc_asid_range(struct arm_smmu_domain *smmu_domain)
{
	u32 size;
	u32 asid;
	int i;
	int nr = smmu_domain->ssid_nums;
	struct arm_smmu_device *smmu = smmu_domain->smmu;

	size = 1 << ARM_SMMU_ID_SIZE_8;
	mutex_lock(&smmu->asid_mutex);
	asid = bitmap_find_next_zero_area_off(smmu->asid_map, size, 0, nr, 0, 0);
	if (asid >= size) {
		pr_err("%s: allocated asid failed!\n", __func__);
		mutex_unlock(&smmu->asid_mutex);
		return -ENOSPC;
	}

	for (i = 0; i < nr; i++)
		set_bit(asid + i, smmu->asid_map);

	mutex_unlock(&smmu->asid_mutex);
	pr_info("%s: allocated %u, nr %d, asid_bitmap 0x%lx\n",
		__func__, asid, nr, *((unsigned long *)smmu->asid_map));
	smmu_domain->asid = (u16)asid;
	return 0;
}

static void arm_smmu_release_asid_range(struct arm_smmu_domain *smmu_domain)
{
	int i;
	int nr = smmu_domain->ssid_nums;
	struct arm_smmu_device *smmu = smmu_domain->smmu;

	if (!smmu_domain->smmu) {
		return;
	}

	mutex_lock(&smmu->asid_mutex);
	for (i = 0; i < nr; i++)
		clear_bit(smmu_domain->asid + i, smmu->asid_map);
	mutex_unlock(&smmu->asid_mutex);
	pr_info("%s: free asid %u, nr %d, asid_bitmap 0x%lx\n",
		__func__, smmu_domain->asid, nr, *((unsigned long *)smmu->asid_map));
}

static int arm_smmu_bitmap_alloc(unsigned long *map, int span)
{
	int idx;
	int size = 1 << span;

	do {
		idx = find_first_zero_bit(map, size);
		if (idx == size)
			return -ENOSPC;
	} while (test_and_set_bit(idx, map));

	return idx;
}

static void arm_smmu_bitmap_free(unsigned long *map, int idx)
{
	clear_bit(idx, map);
}

static bool is_svm_bind_task_domain(struct iommu_domain *domain)
{
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);

	if ((smmu_domain->type == ARM_SMMU_DOMAIN_SVM) &&
		(domain->type == IOMMU_DOMAIN_UNMANAGED)) {
		return true;
	}
	return false;
}

static void arm_smmu_domain_free(struct iommu_domain *domain)
{
	int ret;
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);
	struct mm_smmu_domain *mm_domain = to_mm_smmu_domain(smmu_domain);
	struct domain_free_paras paras = {};

	if (!smmu_domain->smmu) {
		goto out_free;
	}

	iommu_put_dma_cookie(domain);

	if (is_svm_bind_task_domain(domain))
		goto out_free;

	arm_smmu_release_asid_range(smmu_domain);
	paras.sid = smmu_domain->sid;
	ret = liblinux_smmu_domain_free(&paras);
	if (ret)
		pr_err("into %s, free domain failed, ret:%d\n", __func__, ret);

out_free:
	if ((mm_domain->list.prev) && (mm_domain->list.next))
		list_del(&mm_domain->list);
	kfree(mm_domain);
}

static int arm_smmu_domain_set_pgd(struct arm_smmu_domain *smmu_domain,
				   void *data)
{
	u32 pid = (u32)(uintptr_t)data;
	u64 ttbr;

	ttbr = liblinux_pal_query_pgd(pid);
	if (!ttbr) {
		pr_err("%s get ttbr failed!\n", __func__);
		return -EINVAL;
	}

	smmu_domain->svm_ttbr = ttbr;

	pr_info("%s, pid:%d, ttbr:%lx\n", __func__, pid, ttbr);
	return 0;
}

void arm_smmu_svm_ssid_asign(struct iommu_domain *domain, u16 ssid)
{
	struct arm_smmu_domain *smmu_domain;

	if (!domain) {
		pr_err("%s, domain is null\n", __func__);
		return;
	}

	smmu_domain = to_smmu_domain(domain);

	if ((ssid >= SSID_ASSIGN_MIN) && (ssid < SSID_ASSIGN_MAX)) {
		smmu_domain->ssid_assign = 1;
		smmu_domain->svm_ssid = ssid;
	} else {
		pr_err("%s, ssid %u out-of-bounds\n", __func__, ssid);
		smmu_domain->ssid_assign = 0;
		smmu_domain->svm_ssid = 0;
	}
}

int arm_smmu_svm_get_ssid(struct iommu_domain *domain,
			  u16 *ssid, u64 *ttbr, u64 *tcr)
{
	struct arm_smmu_domain *smmu_domain = NULL;

	smmu_domain = to_smmu_domain(domain);
	if (!smmu_domain) {
		pr_err("%s, smmu_domain is null\n", __func__);
		return -EINVAL;
	}

	*ssid = smmu_domain->svm_ssid;
	*ttbr = smmu_domain->svm_ttbr;
	*tcr = 0;
	return 0;
}

static struct arm_smmu_domain *arm_smmu_get_dev_smmu_domain(struct device *dev)
{
	struct arm_smmu_master *master = NULL;

	if (!dev->iommu) {
		dev_err(dev, "%s, iommu is null!!\n", __func__);
		return NULL;
	}

	master = dev_iommu_priv_get(dev);
	if (!master) {
		dev_err(dev, "%s, master is null\n", __func__);
		return NULL;
	}

	return master->smmu_domain;
}

static int arm_smmu_svm_alloc_ssid(struct arm_smmu_domain *smmu_domain)
{
	struct arm_smmu_domain *dev_smmu_domain = NULL;

	dev_smmu_domain = arm_smmu_get_dev_smmu_domain(smmu_domain->dev);
	if (!dev_smmu_domain) {
		dev_err(smmu_domain->dev, "%s, dev_smmu_domain is null\n", __func__);
		return -EINVAL;
	}

	if (!smmu_domain->ssid_assign)
		smmu_domain->svm_ssid = arm_smmu_bitmap_alloc(dev_smmu_domain->ssid_map,
			CTXDESC_SURPPOT_SPLIT);

	return 0;
}

static void arm_smmu_svm_free_ssid(struct arm_smmu_domain *smmu_domain)
{
	struct arm_smmu_domain *dev_smmu_domain = NULL;

	dev_smmu_domain = arm_smmu_get_dev_smmu_domain(smmu_domain->dev);
	if (!dev_smmu_domain) {
		dev_err(smmu_domain->dev, "%s, dev_smmu_domain is null\n", __func__);
		return;
	}

	if (!smmu_domain->ssid_assign)
		arm_smmu_bitmap_free(dev_smmu_domain->ssid_map, smmu_domain->svm_ssid);
}

static u32 arm_smmu_get_domain_type(struct device *dev, struct iommu_domain *domain)
{
	int ret;
	u32 domain_type;

	if (of_property_read_bool(dev->of_node, "smmu_identity"))
		domain->type = IOMMU_DOMAIN_IDENTITY;

	ret = of_property_read_u32(dev->of_node, "domain_type", &domain_type);
	if (ret)
		return ARM_SMMU_DOMAIN_DMA;

	return domain_type;
}

static int arm_smmu_domain_finalise(struct device *dev, struct iommu_domain *domain,
				    struct arm_smmu_master *master)
{
	int ret;
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);
	bool is_svm = is_svm_bind_task_domain(domain);
	struct smmu_attach_paras paras = {};

	domain->pgsize_bitmap = PAGESIZE_BITMAP;
	domain->geometry.aperture_end = (1UL << CONFIG_ARM64_VA_BITS) - 1;
	domain->geometry.force_aperture = true;

	if (is_svm) {
		ret = arm_smmu_svm_alloc_ssid(smmu_domain);
		if (ret) {
			dev_err(dev, "%s, alloc ssid err!!\n", __func__);
			return ret;
		}
	}
	ret = arm_smmu_alloc_asid_range(smmu_domain);
	if (ret)
		goto free_ssid;
	debug_print("%s, alloc asid %u, ssid %u\n", __func__, smmu_domain->asid, smmu_domain->svm_ssid);

	paras.smmuid = smmu_domain->smmu->smmuid;
	paras.sid = smmu_domain->sid;
	paras.asid = smmu_domain->asid;
	paras.ssid = arm_smmu_get_ssid(dev);
	paras.ssid_nums = smmu_domain->ssid_nums;
	paras.tlb_flush = smmu_domain->tlb_flush;
	paras.svm_ssid = smmu_domain->svm_ssid;
	paras.svm_ttbr = smmu_domain->svm_ttbr;
	paras.domain_type = smmu_domain->type;
	paras.is_svm = is_svm;

	debug_print("smmu attach smmuid %u, sid %u, ssid %u, ssid_nums %u, tlb_flush %u, asid %u\n",
		paras.smmuid, paras.sid, paras.ssid, paras.ssid_nums, paras.tlb_flush, paras.asid);
	debug_print("svm_ssid %u, svm_ttbr 0x%lx, domain_type %u, is_svm %u\n",
		paras.svm_ssid, paras.svm_ttbr, paras.domain_type, paras.is_svm);
	ret = liblinux_smmu_attach_device(&paras);
	if (ret) {
		dev_err(dev, "liblinux smmu_attach_device failed\n");
		goto free_all;
	}

	return 0;
free_all:
	arm_smmu_release_asid_range(smmu_domain);
free_ssid:
	if (is_svm)
		arm_smmu_svm_free_ssid(smmu_domain);
	return ret;
}

void arm_smmu_svm_tlb_inv_context(struct mm_svm *svm)
{
	int ret;
	struct arm_smmu_domain *smmu_domain = NULL;
	struct smmu_tlb_paras paras = {};

	if (!svm) {
		pr_err("%s: svm is null\n", __func__);
		return;
	}

	smmu_domain = to_smmu_domain(svm->dom);
	if (!smmu_domain) {
		pr_err("%s: smmu_domain is null\n", __func__);
		return;
	}

	paras.sid = smmu_domain->sid;
	paras.is_svm = true;
	paras.ssid = smmu_domain->svm_ssid;

	ret = liblinux_smmu_tlb_inv_context(&paras);
	if (ret)
		pr_err("%s, tlb_inv_context failed\n", __func__);
}

static void arm_smmu_svm_detach_dev(struct iommu_domain *domain,
				    struct device *dev)
{
	int ret;
	struct arm_smmu_domain *smmu_domain = NULL;
	struct smmu_detach_paras paras = {};

	if (unlikely(!domain)) {
		dev_err(dev, "domain is null %s\n", __func__);
		return;
	}

	smmu_domain = to_smmu_domain(domain);
	if (unlikely(!smmu_domain)) {
		dev_err(dev, "smmu_domain is null %s\n", __func__);
		return;
	}

	paras.sid = smmu_domain->sid;
	paras.svm_ssid = smmu_domain->svm_ssid;

	debug_print("%s , detach sid %u, ssid %u\n", __func__, paras.sid, paras.svm_ssid);
	ret = liblinux_smmu_svm_detach_device(&paras);
	if (ret)
		dev_err(dev, "liblinux smmu_svm_detach_device failed\n");
	arm_smmu_svm_free_ssid(smmu_domain);
	arm_smmu_release_asid_range(smmu_domain);
}

static void arm_smmu_detach_dev(struct device *dev)
{
	struct arm_smmu_master *master = dev_iommu_priv_get(dev);
	struct arm_smmu_domain *smmu_domain = master->smmu_domain;

	arm_smmu_release_asid_range(smmu_domain);
}

static u32 of_get_ssid_nums(struct device_node *np)
{
	int ret;
	u32 ssid_nums;

	if (!np) {
		ssid_nums = 1;
		goto out;
	}

	ret = of_property_read_u32(np, "ssid_nums", &ssid_nums);
	if (ret)
		ssid_nums = 1;

out:
	pr_err("%s ssid nums %u\n", __func__, ssid_nums);
	return ssid_nums;
}

static int arm_smmu_domain_attch_dev(struct device *dev,
				     struct arm_smmu_domain *smmu_domain)
{
	int ret;
	struct mm_dom_cookie *cookie = NULL;
	struct arm_smmu_device *smmu = NULL;
	struct arm_smmu_master *master = dev_iommu_priv_get(dev);
	struct smmu_attach_paras paras = {};

	smmu = master->smmu;

	if (smmu_domain->tlb_flush != master->tlb_flush)
		dev_err(dev, "domain tlb_flush:%u, master tlb_flush:%u\n",
			smmu_domain->tlb_flush, master->tlb_flush);

	debug_print(" master tlb_flush %u, ssid_nums %u. smmu_domain  tlb_flush %u, ssid_nums %u\n",
		master->tlb_flush, master->ssid_nums, smmu_domain->tlb_flush, smmu_domain->ssid_nums);

	/*
	 * When tlb_flush type is not ASID_FLUSH and two masters share one sid,
	 * we should alloc a new asid for this new master.
	 * At the time they flush tlb by ssid, we have to find the corresponding
	 * asid to fill into the cmdbuf.
	 *
	 * As for the ASID_FLUSH type, all of the masters which share the same sid
	 * is asigned with the same asid. Thus, all tlbs of this group of masters
	 * will be flushed when any master in this group flushes its tlb.
	 */
	if (master->tlb_flush != ASID_FLUSH) {
		ret = arm_smmu_alloc_asid_range(smmu_domain);
		if (ret)
			return ret;
	}

	paras.smmuid = smmu->smmuid;
	paras.sid = smmu_domain->sid;
	paras.ssid = arm_smmu_get_ssid(dev);
	paras.asid = smmu_domain->asid;
	paras.ssid_nums = smmu_domain->ssid_nums;
	paras.tlb_flush = smmu_domain->tlb_flush;
	ret = liblinux_smmu_domain_attach_new_dev(&paras);
	if (ret) {
		dev_err(dev, "liblinux smmu_domain_attach_new_dev failed\n");
		goto free_asid;
	}

	cookie = smmu_domain->domain.iova_cookie;
	BUG_ON(cookie == NULL);

	if ((cookie->iova.iova_start != master->iova.iova_start) ||
		(cookie->iova.iova_size != master->iova.iova_size) ||
		(cookie->iova.iova_align != master->iova.iova_align)) {
		dev_err(dev, "%s iova mismatch!\n", __func__);
		dev_err(dev, "pool iova:0x%lx,0x%lx,0x%lx\n",
			cookie->iova.iova_start, cookie->iova.iova_size,
			cookie->iova.iova_align);
		dev_err(dev, "master iova:0x%lx,0x%lx,0x%lx\n",
			master->iova.iova_start, master->iova.iova_size,
			master->iova.iova_align);
	}

	return 0;
free_asid:
	arm_smmu_release_asid_range(smmu_domain);
	return ret;
}

static void arm_smmu_iova_lazy_free(struct mm_dom_cookie *cookie)
{
	unsigned int pages_num, i, size, pong;
	unsigned int pingpong;
	unsigned long iova_start;
	u32 *free_size = NULL;

	pingpong = 1UL - cookie->lazy_free->pingpong;
	pages_num = DIV_ROUND_UP(cookie->iova.iova_size, PAGE_SIZE);
	iova_start = cookie->iova.iova_start;
	free_size = cookie->lazy_free->free_size;
	mutex_lock(&cookie->lazy_free->mutex);
	for (i = 0; i < pages_num; i++) {
		size = free_size[i] & IOVA_SIZE_MASK;
		if (size == 0)
			continue;

		pong = (free_size[i] & PINGPONG_MASK) >> PINGPONG_SHIFT;
		if (pong != pingpong)
			continue;

		free_size[i] = 0;
		gen_pool_free(cookie->iova_pool,
			(iova_start + ((unsigned long)i << PAGE_SHIFT)), size);
	}
	mutex_unlock(&cookie->lazy_free->mutex);
	cookie->lazy_free->end = true;
}

static int arm_smmu_iova_lazy_free_thread(void *p)
{
	struct device *dev = (struct device *)p;
	struct iommu_domain *domain = NULL;
	struct mm_dom_cookie *cookie = NULL;
	DEFINE_WAIT(wait);

	domain = iommu_get_domain_for_dev(dev);
	if (!domain) {
		dev_err(dev, "%s, domain is null\n", __func__);
		return -ENOENT;
	}

	cookie = (struct mm_dom_cookie *)domain->iova_cookie;
	if (!cookie) {
		dev_err(dev, "%s, iova_cookie is null\n", __func__);
		return -ENOENT;
	}

	while (!kthread_should_stop()) {
		prepare_to_wait(&cookie->lazy_free->wait_q, &wait,
			TASK_UNINTERRUPTIBLE);
		schedule();
		finish_wait(&cookie->lazy_free->wait_q, &wait);
		arm_smmu_flush_tlb(dev, domain);
		arm_smmu_iova_lazy_free(cookie);
	}
	return 0;
}

static void arm_smmu_cookie_alloc_lazy_free(struct device *dev, struct mm_dom_cookie *cookie)
{
	int ret;
	unsigned int pages_num;
	struct cpumask sched_cpus;
	struct mm_iova_lazy_free *lazy_free = NULL;
	struct sched_param param;

	if (cookie->iova.iova_free == IMME_FREE)
		return;

	lazy_free = kzalloc(sizeof(*lazy_free), GFP_KERNEL);
	if (!lazy_free)
		goto out_err;

	pages_num = DIV_ROUND_UP(cookie->iova.iova_size, PAGE_SIZE);
	lazy_free->free_size = kcalloc(pages_num, sizeof(u32), GFP_KERNEL);
	if (!lazy_free->free_size)
		goto out_free;

	init_waitqueue_head(&lazy_free->wait_q);
	mutex_init(&lazy_free->mutex);
	spin_lock_init(&lazy_free->lock);
	lazy_free->end = true;
	lazy_free->waterline = pages_num / LAZY_FREE_WATERLINE;
	if (lazy_free->waterline < LAZY_FREE_WATERLINE) {
		dev_info(dev, "%s,iova_size %lu too small,disable lazy free\n",
			__func__, cookie->iova.iova_size);
		goto out_free;
	}
	cookie->lazy_free = lazy_free;

	lazy_free->task = kthread_run(arm_smmu_iova_lazy_free_thread, dev,
		"iova.%s", dev_name(dev));
	if (IS_ERR(lazy_free->task)) {
		dev_err(dev, "%s, create lazy free task err %d\n",
			__func__, IS_ERR(lazy_free->task));
		goto out_stop;
	}

	cpumask_setall(&sched_cpus);
	cpumask_clear_cpu(WATCHDOG_CPU, &sched_cpus);
	cpumask_clear_cpu(WIFI_INTX_CPU, &sched_cpus);
	set_cpus_allowed_ptr(lazy_free->task, &sched_cpus);

	/* set thread priority and schedule policy */
	param.sched_priority = LAZY_FREE_SCHED_PRI;
	ret = sched_setscheduler(lazy_free->task, SCHED_RR, &param);
	if (ret)
		dev_info(dev, "%s, task set priority error\n", __func__);

	dev_info(dev, "%s, iova lazy free waterline %lu\n",
		 __func__, lazy_free->waterline);

	return;

out_stop:
	kfree(lazy_free->free_size);
	cookie->lazy_free = NULL;
out_free:
	kfree(lazy_free);
out_err:
	cookie->iova.iova_free = IMME_FREE;
	dev_err(dev, "%s iova_free rollback imme free!\n", __func__);
}

static int arm_smmu_domain_alloc_cookie(struct device *dev,
	struct arm_smmu_domain *smmu_domain, struct iommu_domain *domain)
{
	struct mm_dom_cookie *cookie = NULL;
	struct arm_smmu_master *master = NULL;

	cookie = kzalloc(sizeof(*cookie), GFP_KERNEL);
	if (!cookie) {
		dev_err(dev, "%s, cookie alloc failed\n", __func__);
		return -ENOMEM;
	}

	master = dev_iommu_priv_get(dev);
	cookie->iova_pool = iova_pool_setup(master->iova.iova_start,
					    master->iova.iova_size,
					    master->iova.iova_align,
					    master->iova.iova_seg_grain);
	if (!cookie->iova_pool) {
		dev_err(dev, "%s, iova pool setup fail\n", __func__);
		kfree(cookie);
		return -ENOMEM;
	}
	spin_lock_init(&cookie->iova_lock);
	cookie->iova_root = RB_ROOT;
	cookie->domain = domain;
	cookie->iova.iova_start = master->iova.iova_start;
	cookie->iova.iova_size = master->iova.iova_size;
	cookie->iova.iova_align = master->iova.iova_align;
	cookie->iova.iova_free = master->iova.iova_free;
	smmu_domain->domain.iova_cookie = cookie;

	arm_smmu_cookie_alloc_lazy_free(dev, cookie);

	return 0;
}

static int arm_smmu_attach_dev(struct iommu_domain *domain, struct device *dev)
{
	int ret;
	struct arm_smmu_device *smmu = NULL;
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);
	struct arm_smmu_master *master =  dev_iommu_priv_get(dev);

	debug_print("%s, dev %s attach\n", __func__, dev_name(dev));
	smmu = master->smmu;
	mutex_lock(&smmu_domain->init_mutex);

	if (!smmu_domain->smmu) {
		smmu_domain->smmu = smmu;
		smmu_domain->tlb_flush = master->tlb_flush;
		smmu_domain->sid = arm_smmu_get_sid(dev->iommu->fwspec);
		smmu_domain->dev = dev;
		smmu_domain->type = arm_smmu_get_domain_type(dev, domain);
		/* ssid_nums needed to be set before domain_finalise for asid alloc */
		smmu_domain->ssid_nums = master->ssid_nums;
		ret = arm_smmu_domain_finalise(dev, domain, master);
		if (ret) {
			smmu_domain->smmu = NULL;
			goto out_unlock;
		}
		/* svm bind task */
		if (is_svm_bind_task_domain(domain))
			goto out_unlock;
	} else if (smmu_domain->smmu != smmu) {
		dev_err(dev, "cannot attach to SMMU %s (upstream of %s)\n",
			dev_name(smmu_domain->smmu->dev), dev_name(smmu->dev));
		ret = -ENXIO;
		goto out_unlock;
	} else {
		/* Only first attach device install ste and alloc cookie */
		ret = arm_smmu_domain_attch_dev(dev, smmu_domain);
		goto out_unlock;
	}

	master->smmu_domain = smmu_domain;

	ret = arm_smmu_domain_alloc_cookie(dev, smmu_domain, domain);
	if (ret) {
		dev_err(dev, "%s domain alloc cookie return %d error\n",
			dev_name(dev), ret);
		smmu_domain->smmu = NULL;
		goto out_unlock;
	}
out_unlock:
	mutex_unlock(&smmu_domain->init_mutex);
	dev_info(dev, "%s: [sid:%u] attach to smmuid %d, ret[%d]\n",
		 __func__, master->sids[0], smmu->smmuid, ret);
	return ret;
}

static u64 arm_lpae_prot_to_pte(int prot)
{
#ifdef CONFIG_MM_LB
	int pid = 0;
#endif
	u64 pte;

	pte = ARM_LPAE_PTE_nG;
	if (!(prot & IOMMU_WRITE) && (prot & IOMMU_READ))
		pte |= ARM_LPAE_PTE_AP_RDONLY;
	if (!(prot & IOMMU_PRIV))
		pte |= ARM_LPAE_PTE_AP_UNPRIV;

	/*
	 * Note that this logic is structured to accommodate Mali LPAE
	 * having stage-1-like attributes but stage-2-like permissions.
	 */
	if (prot & IOMMU_MMIO)
		pte |= (ARM_LPAE_MAIR_ATTR_IDX_DEV << ARM_LPAE_PTE_ATTRINDX_SHIFT);
#ifdef CONFIG_MM_LB_FULL_COHERENCY
	/* OSH(2b10) and IOMMU_NO_ALLOCATE is set equivalent to NON_SC */
	else if ((prot & IOMMU_CACHE) && (prot & IOMMU_OSH) && (prot & IOMMU_NO_ALLOCATE))
		pte |= (ARM_LPAE_MAIR_ATTR_IDX_ICACHE_OCACHE_NRANWA << ARM_LPAE_PTE_ATTRINDX_SHIFT);
#endif
	else if (prot & IOMMU_CACHE)
		pte |= (ARM_LPAE_MAIR_ATTR_IDX_CACHE << ARM_LPAE_PTE_ATTRINDX_SHIFT);

	/*
	 * Also Mali has its own notions of shareability wherein its Inner
	 * domain covers the cores within the GPU, and its Outer domain is
	 * "outside the GPU" (i.e. either the Inner or System domain in CPU
	 * terms, depending on coherency).
	 */
	if (prot & IOMMU_CACHE)
#ifdef CONFIG_MM_LB_FULL_COHERENCY
		pte |= prot & IOMMU_OSH ? ARM_LPAE_PTE_SH_OS : ARM_LPAE_PTE_SH_IS;
#else
		pte |= ARM_LPAE_PTE_SH_IS;
#endif
	else
		pte |= ARM_LPAE_PTE_SH_OS;

	if (prot & IOMMU_NOEXEC)
		pte |= ARM_LPAE_PTE_XN;

	pte |= ARM_LPAE_PTE_AF;

#ifdef CONFIG_MM_LB
	pid = (prot & IOMMU_PORT_MASK) >> IOMMU_PORT_SHIFT;
	pte |= !pid ? 0 : lb_pid_to_gidphys(pid);
#endif

#ifdef CONFIG_MM_IO_PGTABLE_EBIT
	pte |= (prot & IOMMU_PROTECT) ? MM_LPAE_PTE_EBIT : 0;
#endif

	return pte;
}

static int arm_smmu_map(struct iommu_domain *domain, unsigned long iova,
				phys_addr_t paddr, size_t size, int prot, gfp_t gfp)
{
	int ret;
	struct smmu_map_paras paras = {};
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);

	paras.paddr = paddr;
	paras.iova = iova;
	paras.prot = arm_lpae_prot_to_pte(prot);
	paras.sid = smmu_domain->sid;
	paras.size = size;

	debug_print("%s sid %u map: iova 0x%llx pa 0x%llx size 0x%llx, prot 0x%llx\n",
		__func__, paras.sid, iova, paddr, size, paras.prot);
	ret = liblinux_smmu_map(&paras);
	if (ret) {
		pr_err("%s map fail: sid %u iova 0x%llx pa 0x%llx size 0x%llx, prot 0x%llx\n",
			__func__, paras.sid, iova, paddr, size, paras.prot);
		return ret;
	}

	return 0;
}

#ifdef CONFIG_DMA_MAP_BATCH
static int arm_smmu_map_batch(void *para, int prot)
{
	struct smmu_map_batch_paras *p = (struct smmu_map_batch_paras *)para;

	p->prot = arm_lpae_prot_to_pte(prot);
	return liblinux_smmu_map_page_batch(para);
}

static int arm_smmu_unmap_batch(void *para)
{
	return liblinux_smmu_unmap_page_batch(para);
}
#endif

static size_t arm_smmu_unmap(struct iommu_domain *domain, unsigned long iova,
			     size_t size, struct iommu_iotlb_gather *gather)
{
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);
	struct smmu_unmap_paras paras = {};
	
	(void)gather;
	paras.sid = smmu_domain->sid;
	paras.iova = iova;
	paras.size = size;
	if (liblinux_smmu_unmap(&paras)) {
		pr_err("%s fail: sid %u iova 0x%lx size 0x%lx\n", __func__, smmu_domain->sid, iova, size);
		return 0;
	}

	return paras.size;
}

static void arm_smmu_flush_iotlb_all(struct iommu_domain *domain)
{
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);

	if (smmu_domain->smmu)
		arm_smmu_tlb_inv_context(smmu_domain);
}

static phys_addr_t arm_smmu_iova_to_phys(struct iommu_domain *domain,
					 dma_addr_t iova)
{
	int ret;
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);
	struct smmu_iova_paras paras = {};

	if (domain->type == IOMMU_DOMAIN_IDENTITY)
		return iova;

	paras.iova = (u64)iova;
	paras.sid = smmu_domain->sid;
	ret = liblinux_smmu_iova_to_phys(&paras);
	if (ret) {
		pr_err("%s fail: iova 0x%lx\n", __func__, iova);
		return 0;
	}
	return (phys_addr_t)paras.paddr;
}


static bool is_addr_in_whitelist(struct iommu_domain *domain, u64 paddr)
{
	struct arm_smmu_device *smmu = to_smmu_domain(domain)->smmu;

	for (int i = 0; i < smmu->whitelist_num; ++i) {
		if((paddr >= smmu->whitelist_range[i].addr) && (paddr < smmu->whitelist_range[i].end))
		   return true;
	}
	return false;
}

static int arm_smmu_map_sg(struct iommu_domain *domain, unsigned long iova,
			   struct scatterlist *sg, unsigned int nents, int prot,
			   gfp_t gfp, size_t *mapped)
{
	int ret = 0;
	struct smmu_map_sg_paras sg_paras = {};
	struct smmu_unmap_paras unmap_paras = {};
	struct scatterlist *s = NULL;
	int i;
	size_t tmp_size = 0;
	size_t mapped_size = 0;

	*mapped = 0;
	if (unlikely(!(domain->type & __IOMMU_DOMAIN_PAGING))) {
		pr_err("%s domain type %u is not __IOMMU_DOMAIN_PAGING\n", __func__, domain->type);
		return -EINVAL;
	}

	if (!(prot & (IOMMU_READ | IOMMU_WRITE))) {
		pr_err("%s no access prot(%d)\n", __func__, prot);
		return -ENOEXEC;
	}

#ifdef CONFIG_MM_IOMMU_TEST
	if ((PageLB(sg_page(sg))) &&
	    (page_private(sg_page(sg))) &&
	    (page_private(sg_page(sg)) & BIT(PAGE_PRIV_OSH_SHIFT))) {
		if (!(prot & IOMMU_CACHE))
			pr_info("[%s] IOMMU_CACHE is not available\n", __func__);
	}
#endif

	sg_paras.sid = to_smmu_domain(domain)->sid;
	sg_paras.count = 0;
	sg_paras.tlb_iova = iova;
	sg_paras.tlb_size = 0;
	sg_paras.map_page_batch = 0;

	for_each_sg(sg, s, nents, i) {
		struct page *pg = sg_page(s);
		phys_addr_t phys = page_to_phys(pg) + s->offset;

		if (!IS_ALIGNED(s->offset, PAGE_SIZE)) {
			pr_err("%s unaligned: offset 0x%lx\n", __func__, s->offset);
			ret = -EINVAL;
			goto out_err;
		}	

		int tmp_prot = prot;
#ifdef CONFIG_MM_LB_FULL_COHERENCY
		if (!(tmp_prot & IOMMU_MMIO) && !is_addr_in_whitelist(domain, (u64)phys)) {
			if (page_private(phys_to_page(phys)) & BIT(PAGE_PRIV_OSH_SHIFT)) {
				tmp_prot |= IOMMU_OSH;
#ifdef CONFIG_MM_IOMMU_TEST
				if (!(tmp_prot & IOMMU_OSH))
					pr_info("[%s] IOMMU_OSH is not available\n", __func__);
#endif	
			}
		}
#endif
		sg_paras.phys[sg_paras.count] = phys;
		sg_paras.length[sg_paras.count] = s->length;
		sg_paras.prot[sg_paras.count] =  arm_lpae_prot_to_pte(tmp_prot);

		tmp_size += s->length;
		sg_paras.count++;

		if (sg_paras.count == MAX_SMMU_MAP_PAGE_BATCH_NUMS) {
			sg_paras.iova = iova + mapped_size;
			if (i + 1 == nents) /* the last call hongmeng to map as well as flush tlb */
				sg_paras.tlb_size = tmp_size;
			debug_print("%s sid %u map: iova 0x%llx pa 0x%llx count %u, prot 0x%lx\n", __func__,
				sg_paras.sid, sg_paras.iova, sg_paras.phys[0], sg_paras.count, sg_paras.prot[0]);
			ret = liblinux_smmu_map_sg(&sg_paras);
			if (ret) {
				goto out_err;
			}
			mapped_size = tmp_size; /* map succ, upd mapped_size */
			sg_paras.count = 0;
		}
	}

	if (sg_paras.count > 0 && ret == 0) {
		sg_paras.iova = iova + mapped_size;
		sg_paras.tlb_size = tmp_size; /* the last call hongmeng to map as well as flush tlb */
		debug_print("%s sid %u map: iova 0x%llx pa 0x%llx count %u, prot 0x%lx\n", __func__,
			sg_paras.sid, sg_paras.iova, sg_paras.phys[0], sg_paras.count, sg_paras.prot[0]);
		ret = liblinux_smmu_map_sg(&sg_paras);
		if (ret) {
			goto out_err;
		}
		mapped_size = tmp_size; /* map succ, upd mapped_size */
	}
	*mapped = mapped_size;
	return 0;

out_err:
	/* undo mappings already done */
	if (mapped_size)
		(void)arm_smmu_unmap(domain, iova, mapped_size, NULL);
	return ret;
}

static int arm_smmu_match_node(struct device *dev, const void *data)
{
	return dev->fwnode == data;
}

static struct arm_smmu_device *arm_smmu_get_by_fwnode(struct fwnode_handle *fwnode)
{
	struct device *dev = driver_find_device(&arm_smmu_driver.driver, NULL,
						fwnode, arm_smmu_match_node);

	put_device(dev);
	return dev ? dev_get_drvdata(dev) : NULL;
}

static int of_get_iova_info_smmu(struct device_node *np,
				 struct iommu_domain_data *iova)
{
	struct device_node *node = NULL;
	int ret;

	iova->iova_start = IOVA_DEFAULT_ADDS;
	iova->iova_size = IOVA_DEFAULT_SIZE;
	iova->iova_align = PAGE_SIZE;

	if (!np)
		return -ENODEV;

	node = of_get_child_by_name(np, "iova_info");
	if (!node) {
		pr_err("no iova_info, default cfg(0x%lx, 0x%lx, 0x%lx, %u)\n",
		       iova->iova_start, iova->iova_size,
		       iova->iova_align, iova->iova_free);
		return 0;
	}
	ret = of_property_read_u64(node, "start-addr", (u64 *)&iova->iova_start);
	if (ret)
		pr_err("read iova start address error\n");

	ret = of_property_read_u64(node, "size", (u64 *)&iova->iova_size);
	if (ret)
		pr_err("read iova size error\n");

	ret = of_property_read_u64(node, "iova-align", (u64 *)&iova->iova_align);
	if (ret)
		pr_err("read iova align error\n");

	ret = of_property_read_u32(node, "iova-free", (u32 *)&iova->iova_free);
	if (ret)
		pr_info("iova free is default\n");

	ret = of_property_read_u64(node, "iova-segment-granularity", (u64 *)&iova->iova_seg_grain);
	if (ret)
		pr_info("iova segment is not needed\n");
	
	pr_info("%s:start_addr 0x%lx, size 0x%lx align 0x%lx, free %u, granularity 0x%lx\n",
		__func__, iova->iova_start, iova->iova_size,
		iova->iova_align, iova->iova_free, iova->iova_seg_grain);

	return 0;
}

static u32 of_get_tlb_flush(struct device_node *np)
{
	u32 tlb_flush = ASID_FLUSH;
	int ret;

	if (!np)
		return ASID_FLUSH;

	ret = of_property_read_u32(np, "tlb_flush", &tlb_flush);
	if (ret)
		return ASID_FLUSH;

	pr_info("%s:tlb_flush %u\n", __func__, tlb_flush);
	return tlb_flush;
}

#ifdef CONFIG_MM_IOMMU_FAULT
static void arm_smmu_remove_master(struct arm_smmu_master *master)
{
	struct arm_smmu_device *smmu = master->smmu;

	if (!smmu || !master->streams)
		return;

	mutex_lock(&smmu->streams_mutex);
	rb_erase(&master->streams->node, &smmu->streams);
	mutex_unlock(&smmu->streams_mutex);

	kfree(master->streams);
	master->streams = NULL;
}

static int arm_smmu_insert_master(struct arm_smmu_device *smmu,
				  struct arm_smmu_master *master)
{
	int ret = 0;
	u32 sid;
	struct arm_smmu_stream *new_stream, *cur_stream;
	struct rb_node **new_node, *parent_node = NULL;

	master->streams = kzalloc(sizeof(*master->streams), GFP_KERNEL);
	if (!master->streams)
		return -ENOMEM;

	mutex_lock(&smmu->streams_mutex);

	sid = master->sids[0];
	new_stream = master->streams;
	new_stream->id = sid;
	new_stream->master = master;

	/* Insert into SID tree */
	new_node = &(smmu->streams.rb_node);
	while (*new_node) {
		cur_stream = rb_entry(*new_node, struct arm_smmu_stream,
				      node);
		parent_node = *new_node;
		if (cur_stream->id > new_stream->id) {
			new_node = &((*new_node)->rb_left);
		} else if (cur_stream->id < new_stream->id) {
			new_node = &((*new_node)->rb_right);
		} else {
			dev_err(master->dev, "stream %u already in tree\n",
				cur_stream->id);
			ret = -EINVAL;
			break;
		}
	}

	if (ret) {
		kfree(master->streams);
		goto out_unlock;
	}
	rb_link_node(&new_stream->node, parent_node, new_node);
	rb_insert_color(&new_stream->node, &smmu->streams);

out_unlock:
	mutex_unlock(&smmu->streams_mutex);
	return ret;
}
#endif

static struct iommu_device *arm_smmu_probe_device(struct device *dev)
{
	int ret = 0;
	struct arm_smmu_device *smmu = NULL;
	struct arm_smmu_master *master = NULL;
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);

	if (!fwspec || fwspec->ops != &arm_smmu_ops)
		return ERR_PTR(-ENODEV);

	if (WARN_ON_ONCE(dev_iommu_priv_get(dev)))
		return ERR_PTR(-EBUSY);

	smmu = arm_smmu_get_by_fwnode(fwspec->iommu_fwnode);
	if (!smmu)
		return ERR_PTR(-ENODEV);

	master = kzalloc(sizeof(*master), GFP_KERNEL);
	if (!master)
		return ERR_PTR(-ENOMEM);

	master->dev = dev;
	master->smmu = smmu;
	master->sids = fwspec->ids;
	master->num_sids = fwspec->num_ids;
	master->tlb_flush = (u8)of_get_tlb_flush(dev->of_node);
	master->ssid_nums = of_get_ssid_nums(dev->of_node);
	ret = of_get_iova_info_smmu(dev->of_node, &master->iova);
	if (ret)
		pr_err("get dev(%s) iova info fail\n", dev_name(dev));

	dev_iommu_priv_set(dev, master);
#ifdef CONFIG_MM_IOMMU_FAULT
	ret = arm_smmu_insert_master(smmu, master);
	if (ret)
		goto err_free_master;
#endif
	pr_info("%s: dev[%s] smmuid %d done.\n", __func__, dev_name(dev), smmu->smmuid);
	mm_iommu_setup_dma_ops(dev);

	return &smmu->iommu;

err_free_master:
	kfree(master);
	dev_iommu_priv_set(dev, NULL);
	return ERR_PTR(ret);
}

static void arm_smmu_release_device(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);
	struct arm_smmu_master *master;

	if (!fwspec || fwspec->ops != &arm_smmu_ops) {
		dev_err(dev, "dev %s not a iommu device\n", dev_name(dev));
		return;
	}

	master = dev_iommu_priv_get(dev);
	arm_smmu_detach_dev(dev);
#ifdef CONFIG_MM_IOMMU_FAULT
	arm_smmu_remove_master(master);
#endif
	kfree(master);
	iommu_fwspec_free(dev);
}

static struct iommu_group *arm_smmu_get_iommu_group_by_sid(struct device *dev)
{
	u32 sid;
	struct arm_smmu_device *smmu = NULL;
	struct arm_smmu_iommu_group *arm_smmu_group = NULL;

	smmu = arm_smmu_get_dev_smmu(dev);
	if (!smmu) {
		dev_err(dev, "%s, dev smmu is null\n", __func__);
		return NULL;
	}

	sid = arm_smmu_get_sid(dev->iommu->fwspec);
	if (sid >= SMMU_MAX_STREAM_ID) {
		dev_err(dev, "%s: %u out of %s sid range\n", __func__, sid,
			dev_name(smmu->dev));
		return NULL;
	}

	list_for_each_entry(arm_smmu_group, &(smmu->iommu_groups), list)
		if (arm_smmu_group->sid == sid) {
			iommu_group_ref_get(arm_smmu_group->group);
			return  arm_smmu_group->group;
		}

	return NULL;
}

static void arm_smmu_iommu_group_register(struct device *dev, struct iommu_group *group)
{
	u32 sid;
	struct arm_smmu_device *smmu = NULL;
	struct arm_smmu_iommu_group *arm_smmu_group = NULL;

	if (IS_ERR_OR_NULL(group))
		return;

	smmu = arm_smmu_get_dev_smmu(dev);
	if (!smmu) {
		dev_err(dev, "%s, dev smmu is null\n", __func__);
		return;
	}

	sid = arm_smmu_get_sid(dev->iommu->fwspec);
	if (sid >= SMMU_MAX_STREAM_ID) {
		dev_err(dev, "%s: %u out of %s sid range\n", __func__, sid,
			dev_name(smmu->dev));
		return;
	}

	arm_smmu_group = kzalloc(sizeof(*arm_smmu_group), GFP_KERNEL);
	if (!arm_smmu_group)
		return;

	arm_smmu_group->group = group;
	BLOCKING_INIT_NOTIFIER_HEAD(&arm_smmu_group->notifier);
	arm_smmu_group->sid = sid;
	spin_lock(&smmu->iommu_groups_lock);
	list_add_tail(&arm_smmu_group->list, &(smmu->iommu_groups));
	spin_unlock(&smmu->iommu_groups_lock);
}

static struct iommu_group *arm_smmu_device_group(struct device *dev)
{
	struct iommu_group *group = NULL;

	/*
	 * We don't support devices sharing stream IDs other than PCI RID
	 * aliases, since the necessary ID-to-device lookup becomes rather
	 * impractical given a potential sparse 32-bit stream ID space.
	 */
	group = arm_smmu_get_iommu_group_by_sid(dev);
	if (group)
		return group;

	if (dev_is_pci(dev))
		group = pci_device_group(dev);
	else
		group = generic_device_group(dev);

	arm_smmu_iommu_group_register(dev, group);
	return group;
}

static int arm_smmu_domain_get_attr(struct iommu_domain *domain,
				enum iommu_attr attr, void *data)
{
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);
	struct smmu_ttbr_paras paras = {};
	int ret;

	switch (attr) {
	case DOMAIN_ATTR_NESTING:
		*(int *)data = DOMAIN_ATTR_NESTING_NOT_SUPPORTED;
		return 0;
	case DOMAIN_ATTR_TTBR:
		paras.sid = smmu_domain->sid;
		ret = liblinux_smmu_get_ttbr(&paras);
		if (ret) {
			pr_err("%s get ttbr fail\n", __func__);
			return -EINVAL;
		}

		*(phys_addr_t *)data = paras.ttbr;
		return 0;
	default:
		pr_err("%s: attr not support, attr = %d", __func__, (int)attr);
		return -ENODEV;
	}
}

static int arm_smmu_domain_set_attr(struct iommu_domain *domain,
				    enum iommu_attr attr, void *data)
{
	int ret = 0;
	struct arm_smmu_domain *smmu_domain = to_smmu_domain(domain);

	if (domain->type != IOMMU_DOMAIN_UNMANAGED)
		return -EINVAL;

	switch (attr) {
	case DOMAIN_ATTR_PGD:
		ret = arm_smmu_domain_set_pgd(smmu_domain, data);
		break;
	default:
		pr_err("%s: attr not support, attr = %d", __func__, (int)attr);
		ret = -ENODEV;
	}

	return ret;
}

static int arm_smmu_of_xlate(struct device *dev, struct of_phandle_args *args)
{
	dev_info(dev, "%s %s args->args_count %d\n", __func__, dev_name(dev), args->args_count);
	return iommu_fwspec_add_ids(dev, args->args, args->args_count);
}

static void arm_smmu_get_resv_regions(struct device *dev, struct list_head *head)
{
	struct iommu_resv_region *region;
	int prot = IOMMU_WRITE | IOMMU_NOEXEC | IOMMU_MMIO;

	region = iommu_alloc_resv_region(MSI_IOVA_BASE, MSI_IOVA_LENGTH,
					 prot, IOMMU_RESV_SW_MSI);
	if (!region) {
		dev_err(dev, "%s failed to allocate resv region\n", __func__);
		return;
	}

	list_add_tail(&region->list, head);

	iommu_dma_get_resv_regions(dev, head);
}

static void arm_smmu_put_resv_regions(struct device *dev, struct list_head *head)
{
	struct iommu_resv_region *entry = NULL;
	struct iommu_resv_region *next = NULL;

	list_for_each_entry_safe(entry, next, head, list)
		kfree(entry);
}

static struct iommu_ops arm_smmu_ops = {
	.capable		= arm_smmu_capable,
	.domain_alloc		= arm_smmu_domain_alloc,
	.domain_free		= arm_smmu_domain_free,
	.attach_dev		= arm_smmu_attach_dev,
	.detach_dev		= arm_smmu_svm_detach_dev,
	.map			= arm_smmu_map,
	.unmap			= arm_smmu_unmap,
	.map_sg			= arm_smmu_map_sg,
#ifdef CONFIG_DMA_MAP_BATCH
	.map_batch		= arm_smmu_map_batch,
	.unmap_batch		= arm_smmu_unmap_batch,
#endif
	.flush_iotlb_all	= arm_smmu_flush_iotlb_all,
	.iova_to_phys		= arm_smmu_iova_to_phys,
	.probe_device		= arm_smmu_probe_device,
	.release_device		= arm_smmu_release_device,
	.device_group		= arm_smmu_device_group,
	.domain_get_attr	= arm_smmu_domain_get_attr,
	.domain_set_attr	= arm_smmu_domain_set_attr,
	.of_xlate		= arm_smmu_of_xlate,
	.get_resv_regions	= arm_smmu_get_resv_regions,
	.put_resv_regions	= arm_smmu_put_resv_regions,
	.flush_tlb		= arm_smmu_flush_tlb,
	.dev_flush_tlb		= arm_smmu_dev_flush_tlb,
#ifdef CONFIG_DMABUF_RECLAIM
	.reclaim_flush_tlb	= arm_smmu_reclaim_flush_tlb,
#endif
	.flush_ttwc		= arm_smmu_flush_ttwc,
	.pgsize_bitmap		= PAGESIZE_BITMAP,
};

static inline bool is_media_normal_smmuid(int smmuid)
{
	return (smmuid == SMMU_MEDIA1 || smmuid == SMMU_MEDIA2);
}

#ifdef CONFIG_SMMU_SUPPORT_AOD
bool get_smmu_tcu_init_info(enum smmu_type_id smmuid, struct smmu_init_info *info)
{
	int ret;
	struct smmu_tcu_info_paras paras = {};

	if (!is_media_normal_smmuid(smmuid) || !info) {
		pr_err("%s false, smmuid-%d\n", __func__, smmuid);
		return false;
	}

	paras.smmuid = smmuid;

	ret = liblinux_smmu_tcu_info(&paras);
	if (ret) {
		pr_err("%s from kernel failed\n", __func__);
		return false;
	}
	info->strtab_base = paras.strtab_base;
	info->strtab_base_cfg = paras.strtab_base_cfg;
	info->cmdq_q_qbase = paras.cmdq_q_qbase;
	info->evtq_q_qbase = paras.evtq_q_qbase;

#ifdef CONFIG_MM_IOMMU_TEST
	pr_info("%s, smmuid: %d, stebase: 0x%llx, stecfg: 0x%llx, cmdq:0x%llx evtq:0x%llx\n",
		__func__, smmuid, info->strtab_base, info->strtab_base_cfg,
		info->cmdq_q_qbase, info->evtq_q_qbase);
#endif

	return true;
}
#endif

static int arm_smmu_set_bus_ops(struct iommu_ops *ops)
{
	int err;

#ifdef CONFIG_PCI
	if (pci_bus_type.iommu_ops != ops) {
		err = bus_set_iommu(&pci_bus_type, ops);
		if (err) {
			pr_err("%s, pci bus set failed!\n", __func__);
			return err;
		}
	}
#endif
#ifdef CONFIG_ARM_AMBA
	if (amba_bustype.iommu_ops != ops) {
		err = bus_set_iommu(&amba_bustype, ops);
		if (err) {
			pr_err("%s, amba bus set failed!\n", __func__);
			goto err_reset_pci_ops;
		}
	}
#endif
	if (platform_bus_type.iommu_ops != ops) {
		err = bus_set_iommu(&platform_bus_type, ops);
		if (err) {
			pr_err("%s, platform bus set failed!\n", __func__);
			goto err_reset_amba_ops;
		}
	}

	return 0;

err_reset_amba_ops:
#ifdef CONFIG_ARM_AMBA
	bus_set_iommu(&amba_bustype, NULL);
#endif
err_reset_pci_ops: __maybe_unused;
#ifdef CONFIG_PCI
	bus_set_iommu(&pci_bus_type, NULL);
#endif
	return err;
}

static int arm_smmu_set_iommu(struct arm_smmu_device *smmu,
			      struct device *dev, resource_size_t ioaddr)
{
	int ret;

	INIT_LIST_HEAD(&smmu->iommu_groups);
	spin_lock_init(&smmu->iommu_groups_lock);

	/* And we're up. Go go go! */
	ret = iommu_device_sysfs_add(&smmu->iommu, dev, NULL,
				     "smmu3.%pa", &ioaddr);
	if (ret) {
		pr_err("%s, sysfs add failed!\n", __func__);
		return ret;
	}

	iommu_device_set_ops(&smmu->iommu, &arm_smmu_ops);
	iommu_device_set_fwnode(&smmu->iommu, dev->fwnode);

	ret = iommu_device_register(&smmu->iommu);
	if (ret) {
		dev_err(dev, "Failed to register iommu\n");
		return ret;
	}
	return 0;
}

#ifdef CONFIG_SMMU_PMU_TCU
int pmu_tcu_sec_sid_init(int smmuid, u32 sid)
{
	int ret;
	struct smmu_pmu_tcu_config_paras paras = {};
	paras.cmd = SMMU_PMU_TCU_SEC_SID_INIT;
	paras.smmuid = smmuid;
	paras.sid = sid;
	ret = liblinux_smmu_pmu_tcu_config(&paras);
	if (ret) {
		pr_err("%s,SMMU TCU init failed!\n", __func__);
		return ret;
	}
 
	return 0;
}

int pmu_tcu_evt_cnt_set(int smmuid, u8 *evt_type, u32 evt_type_size)
{
	if (evt_type == NULL || evt_type_size > PMCG_EVENT_MAX_CNT_NUM) {
		return -EINVAL;
	}
	int ret;
	struct smmu_pmu_tcu_config_paras paras = {};
	paras.cmd = SMMU_PMU_TCU_EVT_CNT_SET;
	paras.smmuid = smmuid;
	ret = memcpy_s(paras.evt_type, sizeof(paras.evt_type), evt_type, evt_type_size * sizeof(u8));
	if (ret) {
		pr_err("%s,SMMU TCU event memcpy failed!\n", __func__);
		return ret;
	}
	paras.evt_type_size = evt_type_size;
	ret = liblinux_smmu_pmu_tcu_config(&paras);
	if (ret) {
		pr_err("%s,SMMU TCU event set failed!\n", __func__);
		return ret;
	}
 
	return 0;
}

int pmu_tcu_enable(int smmuid)
{
	int ret;
	struct smmu_pmu_tcu_config_paras paras = {};
	paras.cmd = SMMU_PMU_TCU_ENABLE;
	paras.smmuid = smmuid;
	ret = liblinux_smmu_pmu_tcu_config(&paras);
	if (ret) {
		pr_err("%s,SMMU TCU enable set failed!\n", __func__);
		return ret;
	}
 
	return 0;
}

int pmu_tcu_read_pmcg_serv(int smmuid, u32 *pmcg_serv, u32 pmcg_srv_size)
{
	if (pmcg_serv == NULL || pmcg_srv_size > PMCG_EVENT_MAX_CNT_NUM) {
		return -EINVAL;
	}
	int ret;
	struct smmu_pmu_tcu_config_paras paras = {};
	paras.cmd = SMMU_PMU_TCU_READ_PMCG_SERV;
	paras.smmuid = smmuid;
	paras.pmcg_srv_size = pmcg_srv_size;
	ret = liblinux_smmu_pmu_tcu_config(&paras);
	if (ret) {
		pr_err("%s,SMMU TCU read pmcg serv failed!\n", __func__);
		return ret;
	}
	ret = memcpy_s(pmcg_serv, pmcg_srv_size * sizeof(u32), paras.pmcg_serv, paras.pmcg_srv_size * sizeof(u32));
	if (ret) {
		pr_err("%s,SMMU TCU read pmcg serv memcpy failed!\n", __func__);
		return ret;
	}
	return 0;
}
#endif

int arm_smmu_tcu_poweron(struct arm_smmu_device *smmu)
{
	int ret;
	struct smmu_tcu_power_on paras = {};
	paras.smmuid = smmu->smmuid;

	ret = liblinux_smmu_tcu_power_on(&paras);
	if (ret) {
		pr_err("%s,TCU power on failed\n", __func__);
		return ret;
	}
 
	return 0;
}

int arm_smmu_tcu_poweroff(struct arm_smmu_device *smmu)
{
	int ret;
	struct smmu_tcu_power_off paras = {};
	
	paras.smmuid = smmu->smmuid;

	ret = liblinux_smmu_tcu_power_off(&paras);
	if (ret) {
		pr_err("%s,TCU power off failed\n", __func__);
		return ret;
	}

	return 0;
}

static int arm_smmu_tcu_pm_notifier(struct notifier_block *nb,
				    unsigned long event, void *data)
{
	int ret;
	struct arm_smmu_device *smmu = NULL;

	if (!nb) {
		pr_err("%s, nb is null\n", __func__);
		return NOTIFY_BAD;
	}

	smmu = container_of(nb, struct arm_smmu_device, tcu_pm_nb);
	if (event == REGULATOR_EVENT_ENABLE) {
		ret = arm_smmu_tcu_poweron(smmu);
		if (ret) {
			pr_err("%s, tcu poweron failed\n", __func__);
			return NOTIFY_BAD;
		}
	} else if (event == REGULATOR_EVENT_PRE_DISABLE) {
		ret = arm_smmu_tcu_poweroff(smmu);
		if (ret) {
			pr_err("%s, tcu poweroff failed\n", __func__);
			return NOTIFY_BAD;
		}
	}
	return NOTIFY_OK;
}

static int arm_smmu_register_pm_notifier(struct arm_smmu_device *smmu)
{
	int ret;
	struct regulator *subsys_supply = NULL;

	if (smmu->smmuid == SMMU_ERR_ID) {
		dev_err(smmu->dev, "%s, smmu error id\n", __func__);
		return -ENODEV;
	}

	if (!is_media_normal_smmuid(smmu->smmuid)) {
		dev_err(smmu->dev, "%s, smmu %u is not media normal smmu\n",
			__func__, smmu->smmuid);
		return 0;
	}

	subsys_supply = devm_regulator_get(smmu->dev, "smmu_tcu");
	if (IS_ERR_OR_NULL(subsys_supply)) {
		dev_err(smmu->dev, "%s, get smmu_tcu failed\n", __func__);
		return -ENXIO;
	}

	smmu->tcu_pm_nb.notifier_call = arm_smmu_tcu_pm_notifier;
	ret = devm_regulator_register_notifier(subsys_supply, &smmu->tcu_pm_nb);
	if (ret) {
		dev_err(smmu->dev, "%s, register regulator notifier err %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

int arm_smmu_poweron(struct device *dev)
{
	int ret;
	struct arm_smmu_device *smmu = NULL;

	smmu = arm_smmu_get_dev_smmu(dev);
	if (!smmu) {
		dev_err(dev, "%s, smmu is null\n", __func__);
		return -EINVAL;
	}

	ret = arm_smmu_tcu_poweron(smmu);
	if (ret)
		dev_err(dev, "%s, smmu power on err %d\n", __func__, ret);

	return ret;
}

int arm_smmu_poweroff(struct device *dev)
{
	int ret;
	struct arm_smmu_device *smmu = NULL;

	smmu = arm_smmu_get_dev_smmu(dev);
	if (!smmu) {
		dev_err(dev, "%s, smmu is null\n", __func__);
		return -EINVAL;
	}

	ret = arm_smmu_tcu_poweroff(smmu);
	if (ret)
		dev_err(dev, "%s, smmu power off err %d\n", __func__, ret);

	return ret;
}

int arm_smmu_get_reg_info(struct device *dev, char *buffer, int length)
{
	int ret;

	if (buffer == NULL) {
		dev_err(dev, "%s, buffer is null\n", __func__);
        return -1;
    }

	ret = snprintf(buffer, length, "do not support dump smmu reg info\n");
	if (ret < 0)
		return ret;

	return 0;
}

void arm_smmu_tbu_status_print(struct device *dev)
{
	pr_err("%s, do not support\n", __func__);
	(void)dev;
}

int arm_smmu_evt_register_notify(struct device *dev, struct notifier_block *n)
{
	if (!dev) {
		pr_err("%s, dev is null\n", __func__);
		return -EINVAL;
	}
	pr_err("%s, do not support smmu event notify\n", __func__);
	(void)n;
	return 0;
}

int arm_smmu_evt_unregister_notify(struct device *dev, struct notifier_block *n)
{
	if (!dev) {
		pr_err("%s, dev is null\n", __func__);
		return -EINVAL;
	}
	pr_err("%s, do not support smmu event notify\n", __func__);
	(void)n;
	return 0;
}

static int arm_smmu_device_dt_probe(struct platform_device *pdev,
				    struct arm_smmu_device *smmu)
{
	struct device *dev = &pdev->dev;

	if (of_property_read_u32(dev->of_node, "smmuid", &smmu->smmuid)) {
		dev_err(dev, "missing smmuid property\n");
		return -EINVAL;
	}
	if (smmu->smmuid > SMMU_PCIE1)
		return -EINVAL;	

	smmu->whitelist_num = 0;
	(void)of_property_read_u32(dev->of_node, "whitelist-num", &smmu->whitelist_num);

	if (smmu->whitelist_num > MAX_WHITELIST_NUM) {
		dev_err(dev, "smmuid %d whitelist-num %u too long\n", smmu->smmuid, smmu->whitelist_num);
		return -EINVAL;	
	} else if (smmu->whitelist_num != 0) {
		if(of_property_read_u32_array(dev->of_node, "whitelist-range",
			(u32 *)&(smmu->whitelist_range), smmu->whitelist_num * 2)) {
			dev_err(dev, "smmuid %d with whitelist_num.%d\n", smmu->smmuid, smmu->whitelist_num);
			return -EINVAL;
		}
		for(int i = 0; i < smmu->whitelist_num; i++) {
			smmu->whitelist_range[i].end = smmu->whitelist_range[i].addr + smmu->whitelist_range[i].end;
		}
	}

	dev_err(dev, "smmuid %u probe wlnum %u\n", smmu->smmuid, smmu->whitelist_num);
	return 0;
}

u32 svm_get_smmuid(struct device *dev)
{
	struct arm_smmu_device *smmu = NULL;
	struct arm_smmu_master *master;

	if (!dev) {
		pr_err("%s dev invalid!\n", __func__);
		return SMMU_ERR_ID;
	}

	master = dev_iommu_priv_get(dev);
	if (!master) {
		pr_err("%s master invalid!\n", __func__);
		return SMMU_ERR_ID;
	}
	
	smmu = master->smmu;
	if (!smmu) {
		pr_err("%s smmu invalid!\n", __func__);
		return SMMU_ERR_ID;
	}
	return smmu->smmuid;
}

static int arm_smmu_device_probe(struct platform_device *pdev)
{
	int ret;
	struct resource *res = NULL;
	struct arm_smmu_device *smmu = NULL;
	struct device *dev = &pdev->dev;

	smmu = devm_kzalloc(dev, sizeof(*smmu), GFP_KERNEL);
	if (!smmu) {
		dev_err(dev, "failed to allocate arm_smmu_device\n");
		return -ENOMEM;
	}
	smmu->dev = dev;

	ret = arm_smmu_device_dt_probe(pdev, smmu);
	if (ret) {
		dev_err(dev, "%s, device dt probe err %d\n", __func__, ret);
		return ret;
	}

	/* Base address */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		pr_err("%s:platform_get_resource failed!\n", __func__);
		return -ENODEV;
	}

	ret = arm_smmu_register_pm_notifier(smmu);
	if (ret) {
		dev_err(dev, "%s, register pm notifier err %d\n", __func__, ret);
		return ret;
	}

	/* Record our private device structure */
	platform_set_drvdata(pdev, smmu);

	ret = arm_smmu_set_iommu(smmu, dev, res->start);
	if (ret) {
		dev_err(dev, "Failed to set iommu\n");
		return ret;
	}
	ret = arm_smmu_set_bus_ops(&arm_smmu_ops);
	if (ret) {
		dev_err(dev, "Failed to set iommu bus\n");
		return ret;
	}

#ifdef CONFIG_MM_IOMMU_FAULT
	mutex_init(&smmu->streams_mutex);
	smmu->streams = RB_ROOT;
	list_add(&smmu->smmu_list, &smmu_dev_list);
#endif
	mutex_init(&smmu->asid_mutex);
	/* Set the INVAL_TTWC_ASID (0xFF) unavailable for users */
	set_bit(INVAL_TTWC_ASID, smmu->asid_map);
	return 0;
}

void arm_smmu_dmabuf_release_iommu(struct dma_buf *dmabuf)
{
	struct mm_smmu_domain *dom = NULL;
	struct mm_smmu_domain *tmp = NULL;

	list_for_each_entry_safe(dom, tmp, &smmuv3_domain_list, list) {
		__dmabuf_release_iommu(dmabuf, dom->smmu_domain.domain);
	}
}

static int arm_smmu_device_remove(struct platform_device *pdev)
{
	struct arm_smmu_device *smmu = platform_get_drvdata(pdev);
	struct smmu_remove_paras paras = {};

	if (!smmu) {
		pr_err("%s, smmu is null\n", __func__);
		return 0;
	}
	paras.smmuid = smmu->smmuid;
	liblinux_smmu_device_remove(&paras);

	pr_info("out %s\n", __func__);
	return 0;
}

/* An SMMUv3 instance */
static const struct of_device_id arm_smmu_of_match[] = {
	{ .compatible = "mm,smmu-v3", },
	{ },
};
MODULE_DEVICE_TABLE(of, arm_smmu_of_match);

static struct platform_driver arm_smmu_driver = {
	.driver	= {
		.name		= "smmu-v3",
		.of_match_table	= of_match_ptr(arm_smmu_of_match),
	},
	.probe	= arm_smmu_device_probe,
	.remove	= arm_smmu_device_remove,
};

static int __init mm_smmuv3_group_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&arm_smmu_driver);
	if (ret) {
		pr_err("%s failed, ret:%d\n", __func__, ret);
		return -EINVAL;
	}
	return 0;
}
fs_initcall_sync(mm_smmuv3_group_init);

MODULE_DESCRIPTION("IOMMU API for ARM architected SMMUv3 implementations");
MODULE_LICENSE("GPL v2");