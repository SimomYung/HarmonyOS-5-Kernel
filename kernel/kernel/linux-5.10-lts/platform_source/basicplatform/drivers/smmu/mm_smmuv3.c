/*
 * Copyright(C) 2019-2020 Hisilicon Technologies Co., Ltd. All rights reserved.
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

#define pr_fmt(fmt) "smmuv3_svm:" fmt

#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <linux/iommu.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <asm/pgtable.h>
#include <linux/arm-smccc.h>
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(5, 4, 0))
#include <asm/compiler.h>
#endif

#ifdef CONFIG_SMMU_SUPPORT_FFA
#include <platform_include/see/ffa/ffa_plat_drv.h>
#endif

#include "platform_include/see/bl31_smc.h"
#include <linux/iommu/mm_svm.h>
#include "platform_smmu_v3.h"

#ifndef VA_BITS
#define VA_BITS CONFIG_ARM64_VA_BITS
#endif

enum mm_svm_lvl {
	SVM_ERROR = 0,
	SVM_WARN = 1,
	SVM_DEBUG = 2,
	SVM_INFO = 3,
	SVM_TRACE = 4,
};

static int svm_debug_level = SVM_INFO;
module_param_named(level, svm_debug_level, int, 0444);

#define mm_svm_print(level, x...)                                        \
	do { if (svm_debug_level >= (level)) pr_err(x); } while (0)

DEFINE_MUTEX(npu_mm_svm_mutex);
DEFINE_MUTEX(aae_mm_svm_mutex);
LIST_HEAD(npu_mm_svm_list);
LIST_HEAD(aae_mm_svm_list);

static struct list_head *get_svm_list(u32 smmuid)
{	
	if (smmuid != SMMU_NPU && smmuid != SMMU_MEDIA1) {
		mm_svm_print(SVM_ERROR, "%s smmuid error, smmid:0x%llx\n", __func__, smmuid);
		return NULL;
	}

	if (smmuid == SMMU_NPU)
		return &npu_mm_svm_list;
	else
		return &aae_mm_svm_list;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
#define MMF_SVM_NPU	32 /* start from high 32bit */
#define MMF_SVM_AAE	33
#endif

static unsigned long smc_ops_list[MM_TCU_OPS_MAX] = {
	MM_SMMU_FID_TCUCTL_UNSEC, /* MM_TCU_CTL_UNSEC */
	MM_SMMU_FID_NODE_STATUS, /* MM_TCU_NODE_STATUS */
	MM_SMMU_FID_TCU_CLK_ON, /* MM_TCU_CLK_ON */
	MM_SMMU_FID_TCU_CLK_OFF, /* MM_TCU_CLK_OFF */
	MM_SMMU_FID_TCU_TTWC_BYPASS_ON, /* TTWC_BYPASS_ON */
	MM_SMMU_FID_TCU_DBG_DIU, /* MM_TCU_DBG_DIU */
	MM_SMMU_FID_TBU_DBG_INF0, /* MM_TBU_DBG_INF0 */
	MM_SMMU_FID_TBU_DBG_INF1, /* MM_TBU_DBG_INF1 */
};

#ifdef CONFIG_SMMU_PMU_TCU
int mm_smmu_pmu_tcu_set_sid(int smmuid, u32 sid)
{
	int ret = 0;
	if ((smmuid > SMMU_MAX_ID) || (smmuid < 0)) {
		pr_err("%s, smmuid is invalid\n", __func__);
		return -ENODEV;
	}
	ret = pmu_tcu_sec_sid_init(smmuid, sid);
	if (ret)
		pr_err("%s, smmu pmu set sid failed\n", __func__);
	return ret;
}

int mm_smmu_pmu_tcu_evt_reg_set(int smmuid, u8 *evt_type, u32 evt_type_size)
{
	int ret = 0;
	if ((smmuid > SMMU_MAX_ID) || (smmuid < 0)) {
		pr_err("%s, smmuid is invalid\n", __func__);
		return -ENODEV;
	}
	ret = pmu_tcu_evt_cnt_set(smmuid, evt_type, evt_type_size);
	if (ret)
		pr_err("%s, smmu pmu evt reg set failed\n", __func__);
	return ret;
}

int mm_smmu_pmu_tcu_enable(int smmuid)
{
	int ret = 0;
	if ((smmuid > SMMU_MAX_ID) || (smmuid < 0)) {
		pr_err("%s, smmuid is invalid\n", __func__);
		return -ENODEV;
	}
	ret = pmu_tcu_enable(smmuid);
	if (ret)
		pr_err("%s, smmu pmu enable failed\n", __func__);
	return ret;
}

int mm_smmu_pmu_tcu_read_pmcg_sevr(int smmuid, u32 *pmcg_srv, u32 pmcg_srv_size)
{
	int ret = 0;
	if ((smmuid > SMMU_MAX_ID) || (smmuid < 0)) {
		pr_err("%s, smmuid is invalid\n", __func__);
		return -ENODEV;
	}
	ret = pmu_tcu_read_pmcg_serv(smmuid, pmcg_srv, pmcg_srv_size);
	if (ret)
	    pr_err("%s, smmu read pmcg svr failed\n", __func__);

	return ret;
}
#endif

#ifdef CONFIG_SMMU_PMU_TBU
int mm_smmu_pmu_tbu_evt_set(void *tbu_base, u8 *evt_type, u32 evt_type_size)
{
	if (!tbu_base) {
		pr_err("%s, tbu_base is invalid\n", __func__);
		return -EINVAL;
	}
	return pmu_tbu_evt_set(tbu_base, evt_type, evt_type_size);
}

int mm_smmu_pmu_tbu_enable(void *tbu_base)
{
	int ret = 0;
	if (!tbu_base) {
		pr_err("%s, tbu_base is invalid\n", __func__);
		return -EINVAL;
	}

	pmu_tbu_enable(tbu_base);
	return ret;
}

int mm_smmu_pmu_tbu_read_cnt(void *tbu_base, u32 *evt_cnt, u32 evt_cnt_size)
{
	if (!tbu_base) {
		pr_err("%s, tbu_base is invalid\n", __func__);
		return -EINVAL;
	}

	return pmu_tbu_read_cnt(tbu_base, evt_cnt, evt_cnt_size);
}
#endif

int mm_smmu_poweron(struct device *dev)
{
	int ret;

	if (!dev) {
		pr_err("%s, dev is null\n", __func__);
		return -ENODEV;
	}

	ret = arm_smmu_poweron(dev);
	if (ret)
		dev_err(dev, "%s, smmu power on failed\n", __func__);
	return ret;
}
EXPORT_SYMBOL(mm_smmu_poweron);

int mm_smmu_poweroff(struct device *dev)
{
	int ret;

	if (!dev) {
		pr_err("%s, dev is null\n", __func__);
		return -ENODEV;
	}

	ret = arm_smmu_poweroff(dev);
	if (ret)
		dev_err(dev, "%s, smmu power off failed\n", __func__);
	return ret;
}
EXPORT_SYMBOL(mm_smmu_poweroff);

int mm_svm_get_ssid(struct mm_svm *svm, u16 *ssid, u64 *ttbr, u64 *tcr)
{
	int ret;

	if (!svm || !ssid || !ttbr || !tcr)
		return -EINVAL;

	if (!svm->dom)
		return -EINVAL;
	ret = arm_smmu_svm_get_ssid(svm->dom, ssid, ttbr, tcr);
	if (ret)
		mm_svm_print(SVM_ERROR, "%s get ssid error!\n", __func__);

	return ret;
}
EXPORT_SYMBOL(mm_svm_get_ssid);
#ifdef CONFIG_LDK_SVM
void mm_svm_dump_pagetable(pgd_t *base_pgd, unsigned long addr, size_t size)
{
	pr_err("%s, evtq has happened, for HM do not dump\n", __func__);
}

void mm_svm_show_pte(struct mm_svm *svm, unsigned long addr, size_t size)
{
	pr_err("%s, evtq has happened, for HM do not show\n", __func__);
}
#else
void mm_svm_dump_pagetable(pgd_t *base_pgd, unsigned long addr, size_t size)
{
	size_t step;
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;

	do {
		step = SZ_4K;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		pgd = pgd_offset_raw(base_pgd, addr);
#else
		pgd = pgd_offset_pgd(base_pgd, addr);
#endif
		pr_err("[0x%08lx], *pgd=0x%016llx", addr, pgd_val(*pgd));
		if (pgd_none(*pgd) || pgd_bad(*pgd)) {
			step = SZ_1G;
			continue;
		}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		pud = pud_offset(pgd, addr);
#else
		pud = pud_offset((p4d_t *)pgd, addr);
#endif
		pr_err(", *pud=0x%016llx", pud_val(*pud));
		if (pud_none(*pud) || pud_bad(*pud)) {
			step = SZ_1G;
			continue;
		}

		pmd = pmd_offset(pud, addr);
		pr_err(", *pmd=0x%016llx", pmd_val(*pmd));
		if (pmd_none(*pmd) || pmd_bad(*pmd)) {
			step = SZ_2M;
			continue;
		}

		pte = pte_offset_map(pmd, addr);
		pr_err(", *pte=0x%016llx", pte_val(*pte));
		pte_unmap(pte);
		pr_err("\n");
	} while (size >= step && (addr += step, size -= step));
}

void mm_svm_show_pte(struct mm_svm *svm, unsigned long addr, size_t size)
{
	struct mm_struct *mm = NULL;
	struct task_struct *task = NULL;
	struct pid *bindpid = NULL;

	if (!svm || !svm->mm || !size)
		return;

	bindpid = find_get_pid(svm->pid);
	if (!bindpid) {
		pr_err("task exit,%d", svm->pid);
		return;
	}

	task = get_pid_task(bindpid, PIDTYPE_PID);
	if (!task) {
		pr_err("task exit,%d", svm->pid);
		put_pid(bindpid);
		return;
	}

	if (task->mm != svm->mm) {
		pr_err("task exit,%d, %pK, %pK", svm->pid, task->mm, svm->mm);
		put_pid(bindpid);
		put_task_struct(task);
		return;
	}

	put_pid(bindpid);
	put_task_struct(task);

	spin_lock(&svm->mm->page_table_lock);

	mm = svm->mm;
	pr_alert("cached pgd = %pK\n", mm->pgd);

	mm_svm_dump_pagetable(mm->pgd, addr, size);

	spin_unlock(&svm->mm->page_table_lock);
}
#endif

static void mm_svm_mutex_unlock(u32 smmuid)
{
	if (smmuid == SMMU_NPU)
		mutex_unlock(&npu_mm_svm_mutex);
	else if (smmuid == SMMU_MEDIA1)
		mutex_unlock(&aae_mm_svm_mutex);
}

static void mm_svm_mutex_lock(u32 smmuid)
{
	if (smmuid == SMMU_NPU)
		mutex_lock(&npu_mm_svm_mutex);
	else if (smmuid == SMMU_MEDIA1)
		mutex_lock(&aae_mm_svm_mutex);
}

#ifdef CONFIG_LDK_SVM
static DEFINE_MUTEX(npu_svm_pid_list_lock);
LIST_HEAD(npu_svm_pid_list);
static DEFINE_MUTEX(aae_svm_pid_list_lock);
LIST_HEAD(aae_svm_pid_list);
struct svm_pid {
	u32 pid; // task->tgid;
	struct list_head list;
};

static void mm_svm_pid_list_mutex_unlock(u32 smmuid)
{
	if (smmuid == SMMU_NPU)
		mutex_unlock(&npu_svm_pid_list_lock);
	else if (smmuid == SMMU_MEDIA1)
		mutex_unlock(&aae_svm_pid_list_lock);
}

static void mm_svm_pid_list_mutex_lock(u32 smmuid)
{
	if (smmuid == SMMU_NPU)
		mutex_lock(&npu_svm_pid_list_lock);
	else if (smmuid == SMMU_MEDIA1)
		mutex_lock(&aae_svm_pid_list_lock);
}

bool is_svm_task(u32 pid)
{
	struct list_head *pos = NULL;
	struct svm_pid *tmp = NULL;

	mutex_lock(&npu_svm_pid_list_lock);
	list_for_each(pos, &npu_svm_pid_list) {
		tmp = list_entry(pos, struct svm_pid, list);
		if (tmp->pid == pid) {
			mutex_unlock(&npu_svm_pid_list_lock);
			return true;
		}
	}
	mutex_unlock(&npu_svm_pid_list_lock);

	mutex_lock(&aae_svm_pid_list_lock);
	list_for_each(pos, &aae_svm_pid_list) {
		tmp = list_entry(pos, struct svm_pid, list);
		if (tmp->pid == pid) {
			mutex_unlock(&aae_svm_pid_list_lock);
			return true;
		}
	}
	mutex_unlock(&aae_svm_pid_list_lock);

	return false;
}

static int mm_svm_flag_set(u32 pid, u32 smmuid)
{
	struct svm_pid *svm_pid_ent = NULL;

	mm_svm_pid_list_mutex_lock(smmuid);
	svm_pid_ent = kzalloc(sizeof(*svm_pid_ent), GFP_KERNEL);
	if (!svm_pid_ent) {
		mm_svm_print(SVM_ERROR, "%s kzalloc failed!\n", __func__);
		mm_svm_pid_list_mutex_unlock(smmuid);
		return -ENOMEM;
	}
	svm_pid_ent->pid = pid;

	if (smmuid == SMMU_NPU)
		list_add_tail(&svm_pid_ent->list, &npu_svm_pid_list);
	else if (smmuid == SMMU_MEDIA1) 
		list_add_tail(&svm_pid_ent->list, &aae_svm_pid_list);
		
	mm_svm_pid_list_mutex_unlock(smmuid);
	return 0;
}

static int mm_svm_flag_unset(struct mm_svm *svm, u32 smmuid)
{
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;
	struct svm_pid *svm_pid_ent = NULL;
	struct list_head *svm_pid_list = NULL;

	if (!svm) {
		mm_svm_print(SVM_ERROR, "%s invalid svm!\n", __func__);
		return -EINVAL;
	}

	mm_svm_pid_list_mutex_lock(smmuid);
	if (smmuid == SMMU_NPU) 
		svm_pid_list = &npu_svm_pid_list;
	else if (smmuid == SMMU_MEDIA1) 
		svm_pid_list = &aae_svm_pid_list;

	list_for_each_safe(pos, tmp, svm_pid_list) {
		svm_pid_ent = list_entry(pos, struct svm_pid, list);
		if (svm_pid_ent->pid == svm->pid) {
			list_del(&svm_pid_ent->list);
			kfree(svm_pid_ent);
			svm_pid_ent = NULL;
			break;
		}
	}

	mm_svm_pid_list_mutex_unlock(smmuid);
	mm_svm_print(SVM_INFO, "%s success, pid %d smmuid %u\n",__func__, svm->pid, smmuid);
	return 0;
}

static u32 mm_iommu_get_sid(struct iommu_fwspec *fwspec)
{
	if (fwspec->num_ids <= 0)
		return (u32)-1;

	return fwspec->ids[0];
}

int get_svm_sid_pid(u32 *sid, pid_t *pid, u32 smmuid)
{
	struct mm_svm *svm = NULL;
	struct mutex *npu_mm_svm_mutex = NULL;
	struct arm_smmu_domain *svm_domain = NULL;
	struct list_head *svm_list = get_svm_list(smmuid);

	if (!svm_list) {
		pr_err("%s, svm_list is NULL\n", __func__);
		return -1;
	}

	mm_svm_mutex_lock(smmuid);
	if (list_empty(svm_list)) {
		pr_err("%s, svm list is empty\n", __func__);
		mm_svm_mutex_unlock(smmuid);
		return -1;
	}

	svm = list_entry(svm_list->next, struct mm_svm, list);
	if (!svm) {
		pr_err("%s, get mm svm fail\n", __func__);
		mm_svm_mutex_unlock(smmuid);
		return -1;
	}
	mm_svm_mutex_unlock(smmuid);
	svm_domain = to_smmu_domain(svm->dom);
	if (!svm_domain) {
		pr_err("%s, get svm_domain fail\n", __func__);
		return -1;
	}

	*sid = mm_iommu_get_sid(svm_domain->dev->iommu->fwspec);
	*pid = svm->pid;
	return 0;
}
#else
static int mm_svm_flag_unset(struct mm_svm *svm, u32 smmuid)
{
	struct task_struct *task = NULL;
	struct pid *bindpid = NULL;
	struct mm_struct *mm = NULL;

	if (smmuid != SMMU_NPU && smmuid != SMMU_MEDIA1) {
		mm_svm_print(SVM_ERROR, "%s smmuid error, smmid:0x%llx\n", __func__, smmuid);
		return -EINVAL;
	}

	if (!svm) {
		mm_svm_print(SVM_ERROR, "%s invalid svm!\n", __func__);
		return -EINVAL;
	}

	bindpid = find_get_pid(svm->pid);
	if (!bindpid) {
		mm_svm_print(SVM_ERROR, "%s invalid bindpid:%d!\n", __func__, svm->pid);
		return -EINVAL;
	}

	task = get_pid_task(bindpid, PIDTYPE_PID);
	if (!task) {
		mm_svm_print(SVM_ERROR, "%s invalid task,pid:%d!\n", __func__, svm->pid);
		put_pid(bindpid);
		return -EINVAL;
	}

	mm = get_task_mm(task);
	if (!mm) {
		mm_svm_print(SVM_ERROR, "%s get_task_mm failed!\n", __func__);
		put_pid(bindpid);
		put_task_struct(task);
		return -EINVAL;
	}

	put_pid(bindpid);
	put_task_struct(task);

	if (smmuid == SMMU_NPU) {
		clear_bit(MMF_SVM_NPU, &mm->flags);
	} else if (smmuid == SMMU_MEDIA1) {
		clear_bit(MMF_SVM_AAE, &mm->flags);
	}
	mmput(mm);

	return 0;
}
#endif

#ifdef CONFIG_LDK_SVM
static int mm_svm_set_task_info(struct mm_svm *svm, u32 pid)
{
	svm->pid = pid;
	return 0;
}

static void mm_svm_flush_tlb_by_pid(struct mm_svm *svm)
{
	if (!svm) {
		mm_svm_print(SVM_ERROR, "%s svm NULL!\n", __func__);
		return;
	}

	/* flush tlb by asid */
	arm_smmu_svm_tlb_inv_context(svm);
}
#else
static int mm_svm_set_task_info(struct mm_svm *svm,
				struct task_struct *task)
{
	struct mm_struct *mm = NULL;

	svm->name = task->comm;
	mm = get_task_mm(task);
	if (!mm) {
		mm_svm_print(SVM_ERROR, "%s get_task_mm err!\n", __func__);
		return -EINVAL;
	}
	svm->mm = mm;
	svm->pid = task->pid;
	/*
	 * Drop the reference to the mm_struct here. We rely on the
	 * mmu_notifier release call-back to inform us when the mm
	 * is going away.
	 */
	mmput(mm);

	return 0;
}
#endif

static int mm_svm_domain_set_pgd(struct device *dev,
				struct iommu_domain *dom,
				struct task_struct *task)
{
	int ret;
	struct iommu_fwspec *fwspec = NULL;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	fwspec = dev->iommu_fwspec;
#else
	fwspec = dev_iommu_fwspec_get(dev);
#endif
	if (!fwspec) {
		mm_svm_print(SVM_ERROR, "%s fwspec is null\n", __func__);
		return -ENODEV;
	}

	if (!fwspec->ops) {
		mm_svm_print(SVM_ERROR, "%s fwspec ops is null\n", __func__);
		return -ENODEV;
	}

	if (!fwspec->ops->domain_set_attr) {
		mm_svm_print(SVM_ERROR, "%s domain_set_attr is null\n",
			__func__);
		return -ENODEV;
	}

	ret = fwspec->ops->domain_set_attr(dom, DOMAIN_ATTR_PGD, task);
	if (ret)
		mm_svm_print(SVM_ERROR, "%s domain_set_attr pgd err %d\n",
			__func__, ret);

	return ret;
}


bool mm_svm_bind_lookup_list(u32 pid, struct mm_struct *mm,
				u16 ssid, u32 smmuid)
{
	struct mm_svm *tmp_svm = NULL;
	struct arm_smmu_domain *smmu_domain = NULL;
	struct arm_smmu_s1_cfg *cfg = NULL;
	struct list_head *mm_svm_list = NULL;
	struct list_head *pos = NULL;

	if (smmuid == SMMU_NPU) {
		mm_svm_list = &npu_mm_svm_list;
	} else if (smmuid == SMMU_MEDIA1) {
		mm_svm_list = &aae_mm_svm_list;
	}

	if (mm_svm_list == NULL) {
		mm_svm_print(SVM_ERROR, "%s: mm_svm_list is empty\n", __func__);
		return false;
	}

	list_for_each(pos, mm_svm_list) {
		tmp_svm = list_entry(pos, struct mm_svm, list);
		if (!tmp_svm) {
			mm_svm_print(SVM_ERROR, "%s: no mm_svm\n", __func__);
			return false;
		}
		smmu_domain = to_smmu_domain(tmp_svm->dom);
		if (!smmu_domain) {
			mm_svm_print(SVM_ERROR, "%s: no smmu_domain\n", __func__);
			return false;
		}
		cfg = &smmu_domain->s1_cfg;
#ifdef CONFIG_LDK_SVM
		if ((u32)tmp_svm->pid == pid || cfg->cd.ssid == ssid) {
			mm_svm_print(SVM_ERROR, "%s: pre svm also bind this mm struct! pid : %d, ssid: %d smmuid: %d\n",
				     __func__, tmp_svm->pid, cfg->cd.ssid, smmuid);
			return false;
		}
		unused(mm);
#else
		if (tmp_svm->mm == mm || cfg->cd.ssid == ssid) {
			mm_svm_print(SVM_ERROR, "%s:pre svm also bind this mm struct! smmuid : %d, ssid: %d\n",
					__func__, smmuid, cfg->cd.ssid);
			return false;
		}
		unused(pid);
#endif		
	}

	return true;
}

static void mm_svm_add_list_tail(struct list_head *svm_list, u32 smmuid)
{
	if (smmuid == SMMU_NPU) {
		list_add_tail(svm_list, &npu_mm_svm_list);
	} else if (smmuid == SMMU_MEDIA1) {
		list_add_tail(svm_list, &aae_mm_svm_list);
	}
}

#ifdef CONFIG_LDK_SVM
struct mm_svm *mm_svm_bind_task(struct device *dev,
				u32 pid, u16 ssid)
{
	struct mm_svm *svm = NULL;
	struct iommu_domain *dom = NULL;
	struct iommu_group *group = NULL;
	struct list_head *pos = NULL;
	struct mm_svm *tmp_svm = NULL;
	bool ret = false;
	u32 smmuid = 0;

	pr_info("into %s, pid %d, ssid %d\n", __func__, pid, ssid);
	if (!dev || !pid) {
		mm_svm_print(SVM_ERROR, "%s param invalid!\n", __func__);
		return NULL;
	}

	smmuid = svm_get_smmuid(dev);
	if (smmuid != SMMU_NPU && smmuid != SMMU_MEDIA1) {
		mm_svm_print(SVM_ERROR, "%s smmuid error, smmid:0x%llx\n", __func__, smmuid);
		return NULL;
	}

	mm_svm_mutex_lock(smmuid);
	ret = mm_svm_bind_lookup_list(pid, NULL, ssid, smmuid);
	if (!ret) {
		mm_svm_print(SVM_ERROR, "%s lookup list failed!\n", __func__);
		goto out;
	}

	if (mm_svm_flag_set(pid, smmuid) != 0) {
		mm_svm_print(SVM_ERROR, "%s svm flag set failed\n", __func__);
		return NULL;
	}

	svm = kzalloc(sizeof(*svm), GFP_KERNEL);
	if (!svm) {
		mm_svm_print(SVM_ERROR, "%s kzalloc failed!\n", __func__);
		goto out;
	}

	if (!dev->iommu) {
		mm_svm_print(SVM_ERROR, "%s iommu not exist!\n", __func__);
		goto out_free;
	}

	dom = iommu_fwspec_domain_alloc(dev->iommu->fwspec, IOMMU_DOMAIN_UNMANAGED);
	if (!dom) {
		mm_svm_print(SVM_ERROR, "%s domain alloc err!\n", __func__);
		goto out_free;
	}
	svm->dom = dom;

	group = dev->iommu_group;
	if (mm_svm_domain_set_pgd(dev, dom, pid)) {
		mm_svm_print(SVM_ERROR, "%s set task info err!\n", __func__);
		goto dom_free;
	}

	arm_smmu_svm_ssid_asign(dom, ssid);

	dev->iommu_group = NULL;
	if (iommu_attach_device(dom, dev)) {
		mm_svm_print(SVM_ERROR, "%s attach device err!\n", __func__);
		goto dom_free;
	}
	svm->dev = dev;

	if (mm_svm_set_task_info(svm, pid)) {
		mm_svm_print(SVM_ERROR, "%s set task info err!\n", __func__);
		goto iommu_attach;
	}

	dev->iommu_group = group;

	mm_svm_add_list_tail(&svm->list, smmuid);
	mm_svm_mutex_unlock(smmuid);

	/* ensure tlb clean before access data */
	liblinux_pal_flush_pgtbl_cache((current->tgid == pid ? 0 : pid), 0, 1UL << VA_BITS);
	(void)mm_svm_flush_tlb_by_pid(svm);

	return svm;

iommu_attach:
	iommu_detach_device(svm->dom, svm->dev);

dom_free:
	dev->iommu_group = group;
	iommu_domain_free(dom);

out_free:
	kfree(svm);
	svm = NULL;

out:
	mm_svm_mutex_unlock(smmuid);
	return NULL;
}
#else
static void mm_svm_set_bit(struct mm_struct *mm, u32 smmuid)
{
	if (smmuid == SMMU_NPU) {
		set_bit(MMF_SVM_NPU, &mm->flags);
	} else {
		set_bit(MMF_SVM_AAE, &mm->flags);
	}
}

struct mm_svm *mm_svm_bind_task(struct device *dev,
				struct task_struct *task, u16 ssid)
{
	struct mm_svm *svm = NULL;
	struct iommu_domain *dom = NULL;
	struct iommu_group *group = NULL;
	struct mm_struct *mm;
	bool ret = false;
	u32 smmuid = 0;

	mm_svm_print(SVM_INFO, "into %s, ssid %u\n", __func__, ssid);
	if (!dev || !task) {
		mm_svm_print(SVM_ERROR, "%s param invalid!\n", __func__);
		return NULL;
	}

	mm = get_task_mm(task);
	if (!mm) {
		mm_svm_print(SVM_ERROR, "%s get_task_mm failed!\n", __func__);
		return NULL;
	}

	smmuid = svm_get_smmuid(dev);
	if(smmuid != SMMU_NPU && smmuid != SMMU_MEDIA1) {
		mm_svm_print(SVM_ERROR, "%s smmuid error, smmid:0x%llx\n", __func__, smmuid);
		return NULL;
	}

	mm_svm_mutex_lock(smmuid);
	ret = mm_svm_bind_lookup_list(0, mm, ssid, smmuid);
	if (!ret) {
		mm_svm_print(SVM_ERROR, "%s smmuid:%d lookup list failed!\n", __func__, smmuid);
		goto out;
	}
	
	/* set svm flag */
	mm_svm_set_bit(mm, smmuid);

	svm = kzalloc(sizeof(*svm), GFP_KERNEL);
	if (!svm) {
		mm_svm_print(SVM_ERROR, "%s kzalloc failed!\n", __func__);
		goto out;
	}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	dom = iommu_domain_alloc(dev->bus);
#else
	if (!dev->iommu) {
		mm_svm_print(SVM_ERROR, "%s iommu not exist!\n", __func__);
		goto out_free;
	}
	dom = iommu_fwspec_domain_alloc(dev->iommu->fwspec, IOMMU_DOMAIN_UNMANAGED);
#endif
	if (!dom) {
		mm_svm_print(SVM_ERROR, "%s domain alloc err!\n", __func__);
		goto out_free;
	}
	svm->dom = dom;

	group = dev->iommu_group;
	if (mm_svm_domain_set_pgd(dev, dom, task)) {
		mm_svm_print(SVM_ERROR, "%s set task info err!\n", __func__);
		goto dom_free;
	}
	arm_smmu_svm_ssid_asign(dom, ssid);

	dev->iommu_group = NULL;
	if (iommu_attach_device(dom, dev)) {
		mm_svm_print(SVM_ERROR, "%s attach device err!\n", __func__);
		goto dom_free;
	}
	svm->dev = dev;

	if (mm_svm_set_task_info(svm, task)) {
		mm_svm_print(SVM_ERROR, "%s set task info err!\n", __func__);
		goto iommu_attach;
	}

	dev->iommu_group = group;
	mm_svm_print(SVM_INFO, "out %s\n", __func__);
	mm_svm_add_list_tail(&svm->list, smmuid);
	mm_svm_mutex_unlock(smmuid);

	/* ensure tlb clean before access data */
	(void)mm_flush_pgtbl_cache(mm->pgd, 0, 1UL << VA_BITS);
	mmput(mm);
	arm_smmu_svm_tlb_inv_context(svm);
	return svm;

iommu_attach:
	iommu_detach_device(svm->dom, svm->dev);

dom_free:
	dev->iommu_group = group;
	iommu_domain_free(dom);

out_free:
	kfree(svm);
	svm = NULL;

out:
	mm_svm_mutex_unlock(smmuid);
	mmput(mm);
	return NULL;
}
EXPORT_SYMBOL(mm_svm_bind_task);
#endif

void mm_svm_unbind_task(struct mm_svm *svm)
{
	int ret;
	struct iommu_group *group = NULL;
	u32 smmuid = 0;

	mm_svm_print(SVM_INFO, "in %s, pid:%d\n", __func__, svm->pid);
	if (!svm || !svm->dom || !svm->dev) {
		mm_svm_print(SVM_ERROR, "%s:svm NULL\n", __func__);
		return;
	}

	smmuid = svm_get_smmuid(svm->dev);
	if (smmuid != SMMU_NPU && smmuid != SMMU_MEDIA1) {
		mm_svm_print(SVM_ERROR, "%s smmuid error, smmid:0x%llx\n", __func__, smmuid);
		return;
	}

	mm_svm_mutex_lock(smmuid);
	(void)mm_svm_flag_unset(svm, smmuid);

	list_del(&svm->list);

	group = svm->dev->iommu_group;
	svm->dev->iommu_group = NULL;
	iommu_detach_device(svm->dom, svm->dev);
	svm->dev->iommu_group = group;

	iommu_domain_free(svm->dom);
	kfree(svm);
	svm = NULL;

	mm_svm_mutex_unlock(smmuid);
	mm_svm_print(SVM_INFO, "out %s, smmuid:%d\n", __func__, smmuid);
}
EXPORT_SYMBOL(mm_svm_unbind_task);

static void pte_flush_range(pmd_t *pmd, unsigned long addr, unsigned long end)
{
	pte_t *pte = NULL;
	pte_t *pte4k = NULL;

	pte = pte_offset_map(pmd, addr);
	pte4k = (pte_t *)round_down((u64)pte, PAGE_SIZE);
	__flush_dcache_area(pte4k, PAGE_SIZE);

	pte_unmap(pte);
}

static void pmd_flush_range(pud_t *pud, unsigned long addr, unsigned long end)
{
	pmd_t *pmd = NULL;
	pmd_t *pmd4k = NULL;
	unsigned long next;

	pmd = pmd_offset(pud, addr);
	pmd4k = (pmd_t *)round_down((u64)pmd, PAGE_SIZE);
	do {
		next = pmd_addr_end(addr, end);
		if (!pmd_table(*pmd))
			continue;

		pte_flush_range(pmd, addr, next);
	} while (pmd++, addr = next, addr != end);

	__flush_dcache_area(pmd4k, PAGE_SIZE);
}

static void pud_flush_range(pgd_t *pgd, unsigned long addr, unsigned long end)
{
	pud_t *pud = NULL;
#if CONFIG_PGTABLE_LEVELS > 3
	pud_t *pud4k = NULL;
#endif
	unsigned long next;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	pud = pud_offset(pgd, addr);
#else
	pud = pud_offset((p4d_t *)pgd, addr);
#endif
#if CONFIG_PGTABLE_LEVELS > 3
	pud4k = (pud_t *)round_down((u64)pud, PAGE_SIZE);
#endif
	do {
		next = pud_addr_end(addr, end);
		if (!pud_table(*pud))
			continue;

		pmd_flush_range(pud, addr, next);
	} while (pud++, addr = next, addr != end);

#if CONFIG_PGTABLE_LEVELS > 3
	__flush_dcache_area(pud4k, PAGE_SIZE);
#endif
}

int mm_flush_pgtbl_cache(pgd_t *raw_pgd, unsigned long addr, size_t size)
{
	pgd_t *pgd = NULL;
	pgd_t *pgd4k = NULL;
	unsigned long next;
	unsigned long end = addr + PAGE_ALIGN(size);

	if (!raw_pgd) {
		pr_err("%s, bad pgd, addr:0x%lx, size:0x%zx\n",
			__func__, addr, size);
		return -EINVAL;
	}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	pgd = pgd_offset_raw(raw_pgd, addr);
#else
	pgd = pgd_offset_pgd(raw_pgd, addr);
#endif
	pgd4k = (pgd_t *)round_down((u64)pgd, PAGE_SIZE);
	do {
		next = pgd_addr_end(addr, end);
		pud_flush_range(pgd, addr, next);
		pgd++;
		addr = next;
	} while (addr != end);

	__flush_dcache_area(pgd4k, PAGE_SIZE);
	return 0;
}

static int mm_flush_pgtbl_cache_with_lock(struct mm_struct *mm, unsigned long addr, size_t size)
{
	pgd_t *pgd = NULL;
	pgd_t *pgd4k = NULL;
	pgd_t *raw_pgd = mm->pgd;
	unsigned long next;
	unsigned long end = addr + PAGE_ALIGN(size);

	if (!raw_pgd) {
		pr_err("%s, bad pgd, addr:0x%lx, size:0x%zx\n",
			__func__, addr, size);
		return -EINVAL;
	}
	spin_lock(&mm->page_table_lock);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	pgd = pgd_offset_raw(raw_pgd, addr);
#else
	pgd = pgd_offset_pgd(raw_pgd, addr);
#endif
	pgd4k = (pgd_t *)round_down((u64)pgd, PAGE_SIZE);
	do {
		next = pgd_addr_end(addr, end);
		pud_flush_range(pgd, addr, next);
		pgd++;
		addr = next;
	} while (addr != end);
	spin_unlock(&mm->page_table_lock);

	__flush_dcache_area(pgd4k, PAGE_SIZE);
	return 0;
}

#ifdef CONFIG_LDK_SVM
struct mm_svm *mm_svm_flush_lookup(u32 pid, struct list_head *mm_svm_list)
{
	struct mm_svm *svm = NULL;
	struct list_head *pos = NULL;

	list_for_each(pos, mm_svm_list) {
		svm = list_entry(pos, struct mm_svm, list);
		if ((u32)svm->pid == pid) {
			return svm;
		}
	}
	return NULL;
}

int mm_svm_flush_cache(u32 pid, unsigned long addr, size_t size)
{
	struct mm_svm *npu_svm = NULL;
	struct mm_svm *aae_svm = NULL;

	if (!pid || !size || (addr + size > BIT(VA_BITS))) {
		pr_err("%s, inval param, pid:0x%x, addr:0x%lx, size:0x%zx\n",
			__func__, pid, addr, size);
		return -EINVAL;
	}

#ifdef CONFIG_SVM_PRE_FLUSH_CACHE
	liblinux_pal_flush_pgtbl_cache((current->tgid == pid ? 0 : pid), addr, (addr + size));
#endif

	mutex_lock(&npu_mm_svm_mutex);
	npu_svm = mm_svm_flush_lookup(pid, &npu_mm_svm_list);
	if (npu_svm) {
		arm_smmu_svm_tlb_inv_context(npu_svm); /* flush tlb when mmap */
	}
	mutex_unlock(&npu_mm_svm_mutex);

	mutex_lock(&aae_mm_svm_mutex);
	aae_svm = mm_svm_flush_lookup(pid, &aae_mm_svm_list);
	if (aae_svm) {
		arm_smmu_svm_tlb_inv_context(aae_svm); /* flush tlb when mmap */
	}
	mutex_unlock(&aae_mm_svm_mutex);

	if (WARN(!npu_svm && !aae_svm, "do not find svm device"))
		return -ENXIO;

#ifndef CONFIG_SVM_PRE_FLUSH_CACHE
	liblinux_pal_flush_pgtbl_cache((current->tgid == pid ? 0 : pid), addr, (addr + size));
#endif
	return 0;
}
#else

struct mm_svm *mm_svm_flush_lookup(struct list_head *mm_svm_list, struct mm_struct *mm)
{
	struct list_head *pos = NULL;
	struct mm_svm *svm = NULL;
	list_for_each(pos, mm_svm_list) {
		svm = list_entry(pos, struct mm_svm, list);
		if (svm->mm == mm)
			return svm;
	}
	return NULL;
}

int mm_svm_flush_cache(struct mm_struct *mm, unsigned long addr, size_t size)
{
	struct mm_svm *npu_svm = NULL;
	struct mm_svm *aae_svm = NULL;

	if (!mm || !size || (addr + size > BIT(VA_BITS))) {
		pr_err("%s, bad mm, addr:0x%lx, size:0x%zx\n",
			__func__, addr, size);
		return -EINVAL;
	}
	
	mutex_lock(&npu_mm_svm_mutex);
	npu_svm = mm_svm_flush_lookup(&npu_mm_svm_list, mm);
	if ((npu_svm) && (npu_svm->mm) && test_bit(MMF_SVM_NPU, &npu_svm->mm->flags) ) {
		arm_smmu_svm_tlb_inv_context(npu_svm); /* flush tlb when mmap */
	}
	mutex_unlock(&npu_mm_svm_mutex);

	mutex_lock(&aae_mm_svm_mutex);
	aae_svm = mm_svm_flush_lookup(&aae_mm_svm_list, mm);
	if ((aae_svm) && (aae_svm->mm) && test_bit(MMF_SVM_AAE, &aae_svm->mm->flags)) {
		arm_smmu_svm_tlb_inv_context(aae_svm); /* flush tlb when mmap */
	}
	mutex_unlock(&aae_mm_svm_mutex);

	if (WARN(!npu_svm && !aae_svm, "do not find svm device"))
		return -ENXIO;

	(void)mm_flush_pgtbl_cache_with_lock(mm, addr, size);

	return 0;
}
EXPORT_SYMBOL(mm_svm_flush_cache);
#endif

int mm_smmu_evt_register_notify(struct device *dev, struct notifier_block *n)
{
	return arm_smmu_evt_register_notify(dev, n);
}
EXPORT_SYMBOL(mm_smmu_evt_register_notify);

int mm_smmu_evt_unregister_notify(struct device *dev,
				struct notifier_block *n)
{
	return arm_smmu_evt_unregister_notify(dev, n);
}
EXPORT_SYMBOL(mm_smmu_evt_unregister_notify);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
static noinline u64 smc_smmu_sec_config(u64 fid, int a1, int a2, int a3)
{
#ifdef CONFIG_SMMU_SUPPORT_FFA
	struct ffa_send_direct_data res = {0};
	res.data0 = fid;
	res.data1 = a1;
	res.data2 = a2;
	res.data3 = a3;
	ffa_platdrv_send_msg(&res);
	return res.data0;
#else
	struct arm_smccc_res res;

	arm_smccc_smc(fid, a1, a2, a3, 0, 0, 0, 0, &res);

	return res.a0;
#endif
}
#else
static noinline u64 smc_smmu_sec_config(u64 fid, int a1, int a2, int a3)
{
	register u64 x0 asm("x0") = fid;
	register u64 x1 asm("x1") = a1;
	register u64 x2 asm("x2") = a2;
	register u64 x3 asm("x3") = a3;

	asm volatile (
		__asmeq("%0", "x0")
		__asmeq("%1", "x1")
		__asmeq("%2", "x2")
		__asmeq("%3", "x3")

		"smc    #0\n"
		: "+r" (x0)
		: "r" (x1), "r" (x2), "r"(x3));

	return x0;
}
#endif

static bool smc_opcode_check(enum mm_smmu_smc_ops opcode)
{
	return (opcode != MM_TCU_NODE_STATUS && opcode != MM_TCU_DBG_DIU &&
		opcode != MM_TBU_DBG_INF0 && opcode != MM_TBU_DBG_INF1);
}

u64 sec_smmu_ops(int id, enum mm_smmu_smc_ops opcode, u32 cmd, u32 smmuid)
{
	u64 ret;

	if (opcode >= MM_TCU_OPS_MAX) {
		pr_err("%s opcode error: id = %d, opcode = %d\n", __func__, id, opcode);
		return (u64)-EINVAL;
	}

	ret = smc_smmu_sec_config(smc_ops_list[opcode], id, cmd, smmuid);
	if (smc_opcode_check(opcode)) {
		if (ret)
			pr_err("%s fail: id = %d, opcode = %d, ret = %llu\n", __func__, id, opcode, ret);
	}
	return ret;
}

int mm_smmu_nonsec_tcuctl(int smmuid)
{
	u64 ret;

	ret = sec_smmu_ops(smmuid, MM_TCU_CTL_UNSEC, SMC_DEFAULT_CMD, 0);
	pr_info("%s, smmuid:%d, ret:0x%llx\n", __func__, smmuid, ret);
	return 0;
}

#ifndef CONFIG_HM_SMMU_V3
int mm_smmu_tcu_node_status(int smmuid)
{
	u64 ret = 0;

	if (arm_smmu_tcu_is_disable(smmuid)) {
		pr_err("%s, smmu[%d] not poweron\n", __func__, smmuid);
		return ret;
	}

	ret = sec_smmu_ops(smmuid, MM_TCU_NODE_STATUS, SMC_DEFAULT_CMD, 0);
	pr_info("%s, smmuid:%d, ret:0x%llx\n", __func__, smmuid, ret);
	return ret;
}
#endif

void mm_smmu_tcu_dbg_diu(int smmuid)
{
	u64 ret;
	u32 inv_brc_dest;
	u32 sync_brc_dest;

	ret = sec_smmu_ops(smmuid, MM_TCU_DBG_DIU, SMC_DEFAULT_CMD, 0);
	inv_brc_dest = ret >> TCU_DBG_INV_BRC_DEST_SHIFT;
	sync_brc_dest = ret & TCU_DBG_SYNC_BRC_DEST_MASK;
	pr_err("%s, smmuid:%d, inv_brc_ack:0x%x, inv_brc_req:0x%x\n", __func__, smmuid,
			(inv_brc_dest >> TCU_DBG_INV_BRC_ACK_SHIFT) & TCU_DBG_INV_SYNC_BRC_REQ_ACK_MASK,
			(inv_brc_dest >> TCU_DBG_INV_BRC_REQ_SHIFT) & TCU_DBG_INV_SYNC_BRC_REQ_ACK_MASK);
	pr_err("%s, smmuid:%d, sync_brc_ack:0x%x, sync_brc_req:0x%x\n", __func__, smmuid,
			(sync_brc_dest >> TCU_DBG_SYNC_BRC_ACK_SHIFT) & TCU_DBG_INV_SYNC_BRC_REQ_ACK_MASK,
			(sync_brc_dest >> TCU_DBG_SYNC_BRC_REQ_SHIFT) & TCU_DBG_INV_SYNC_BRC_REQ_ACK_MASK);
}

static u32 mm_smmu_tbu_dbg_print(u32 tbuid, u32 cmd, u32 smmuid)
{
	u64 ret;
	u32 dbg_out_0;
	u32 dbg_out_1;
	u32 dbg_out_2;
	u32 dbg_out_3;

	pr_err("%s: tbuid:%d, cmd:%d tbu_dbg_info:\n", __func__, tbuid, cmd);
	pr_err("%s: dbg_cfg_addr bit[8:9]:%d, bit[7:0]:%d\n", __func__,
			(cmd >> TBU_DBG_CFG_ADDR_BIT8_SHIFT) & TBU_DBG_CFG_ADDR_BIT8_MASK,
			(cmd >> TBU_DBG_CFG_ADDR_BIT0_SHIFT) & TBU_DBG_CFG_ADDR_BIT0_MASK);

	/*  Attention!!! MM_TBU_DBG_INF0 must be obtained before obtaining MM_TBU_DBG_INF1. */
	ret = sec_smmu_ops(tbuid, MM_TBU_DBG_INF0, cmd, smmuid);
	dbg_out_0 = ret >> TBU_DBG_OUT_DATA_SHIFT;
	dbg_out_1 = ret & TBU_DBG_OUT_DATA_MASK;
	pr_err("%s: tbu_dbg_out0:%d\n", __func__, dbg_out_0);
	pr_err("%s: tbu_dbg_out1:%d\n", __func__, dbg_out_1);

	ret = sec_smmu_ops(tbuid, MM_TBU_DBG_INF1, SMC_DEFAULT_CMD, smmuid);
	dbg_out_2 = ret >> TBU_DBG_OUT_DATA_SHIFT;
	dbg_out_3 = ret & TBU_DBG_OUT_DATA_MASK;
	pr_err("%s: tbu_dbg_out2:%d\n", __func__, dbg_out_2);
	pr_err("%s: tbu_dbg_out3:%d\n", __func__, dbg_out_3);

	return dbg_out_0;
}

static void mm_tbu_residual_info(int tbuid, int smmuid)
{
	u32 cmd;
	int i;

	cmd = TBU_DBG_CFG_RD_ENABLE << TBU_DBG_CFG_RD_SHIFT | TBU_DBG_CFG_ADDR_BIT8_CMD0 << TBU_DBG_CFG_ADDR_BIT8_SHIFT;
	for (i = 0; i < TBU_DBG_CFG_CMD_MAX; i++) {
		cmd |= i << TBU_DBG_CFG_ADDR_BIT0_SHIFT;
		(void)mm_smmu_tbu_dbg_print(tbuid, cmd, smmuid);
	}

	cmd = TBU_DBG_CFG_RD_ENABLE << TBU_DBG_CFG_RD_SHIFT | TBU_DBG_CFG_ADDR_BIT8_CMD1 << TBU_DBG_CFG_ADDR_BIT8_SHIFT;
	for (i = 0; i < TBU_DBG_CFG_CMD_MAX; i++) {
		cmd |= i << TBU_DBG_CFG_ADDR_BIT0_SHIFT;
		(void)mm_smmu_tbu_dbg_print(tbuid, cmd, smmuid);
	}

	cmd = TBU_DBG_CFG_RD_ENABLE << TBU_DBG_CFG_RD_SHIFT | TBU_DBG_CFG_ADDR_BIT8_CMD2 << TBU_DBG_CFG_ADDR_BIT8_SHIFT;
	for (i = 0; i < TBU_DBG_CFG_CMD_MAX; i++) {
		cmd |= i << TBU_DBG_CFG_ADDR_BIT0_SHIFT;
		(void)mm_smmu_tbu_dbg_print(tbuid, cmd, smmuid);
	}
}

void mm_smmu_get_tbu_dbg_status(u32 tbuid, u32 smmuid)
{
	u32 cmd;
	u32 dbg_out_0;

	cmd = TBU_DBG_CFG_RD_ENABLE << TBU_DBG_CFG_RD_SHIFT |
			TBU_DBG_CFG_ADDR_BIT8_CMD3 << TBU_DBG_CFG_ADDR_BIT8_SHIFT |
			TBU_DBG_CFG_ADDR_BIT0_CMD1 << TBU_DBG_CFG_ADDR_BIT0_SHIFT;
	(void)mm_smmu_tbu_dbg_print(tbuid, cmd, smmuid);

	cmd = TBU_DBG_CFG_RD_ENABLE << TBU_DBG_CFG_RD_SHIFT |
			TBU_DBG_CFG_ADDR_BIT8_CMD3 << TBU_DBG_CFG_ADDR_BIT8_SHIFT |
			TBU_DBG_CFG_ADDR_BIT0_CMD0 << TBU_DBG_CFG_ADDR_BIT0_SHIFT;

	dbg_out_0 = mm_smmu_tbu_dbg_print(tbuid, cmd, smmuid);
	if (dbg_out_0)
		mm_tbu_residual_info(tbuid, smmuid);
}

void mm_smmu_dump_tbu_status(struct device *dev)
{
	if (!dev) {
		pr_err("%s input invalid\n", __func__);
		return;
	}
	arm_smmu_tbu_status_print(dev);
}

bool mm_get_smmu_info(struct device *dev, char *buffer, int length)
{
	int ret;

	if (!dev || !buffer || (length <= 0)) {
		pr_err("%s input invalid\n", __func__);
		return false;
	}

	ret = arm_smmu_get_reg_info(dev, buffer, length);
	if (ret)
		return false;
	else
		return true;
}
EXPORT_SYMBOL(mm_get_smmu_info);
#ifdef CONFIG_SMMU_SUPPORT_AOD
bool mm_get_smmu_init_info(enum smmu_type_id smmuid, struct smmu_init_info *info)
{
	return get_smmu_tcu_init_info(smmuid, info);
}
#endif

MODULE_DESCRIPTION(
	"SVM Base on ARM architected SMMUv3 implementations");
MODULE_LICENSE("GPL v2");
