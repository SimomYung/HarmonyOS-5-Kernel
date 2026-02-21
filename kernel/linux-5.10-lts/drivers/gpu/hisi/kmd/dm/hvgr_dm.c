/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/kref.h>
#include <linux/mman.h>
#include <linux/io.h>
#include <linux/rcupdate.h>

#include "hvgr_version.h"
#include "hvgr_gpu_id.h"

#include "hvgr_ioctl.h"
#include "apicmd/hvgr_ioctl_dev.h"
#include "external/hvgr_external_base.h"

#include "hvgr_dm_api.h"
#include "hvgr_mem_api.h"
#include "hvgr_sch_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_debugfs_api.h"
#include "hvgr_datan_api.h"
#include "hvgr_mem_api.h"
#include "hvgr_dm_ctx.h"
#include "hvgr_dm_driver_base.h"
#include "hvgr_dm_dfx.h"
#include "hvgr_platform_api.h"
#include "hvgr_hts_api.h"

#include "hvgr_fcp.h"
#include "hvgr_assert.h"
#include "hvgr_msync.h"
#if defined(HVGR_SIMULATE) && HVGR_SIMULATE
#include "hvgr_cq_simulate.h"
#endif
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common.h"
#endif


static struct hvgr_device *g_gpu_device;
static uint32_t g_gpu_counter;

#define HVGR_PCIE_REG_VIR_MODE      0x300010
#ifndef HVGR_PCIE_REG_JM_TOP
#define HVGR_PCIE_REG_JM_TOP        0x100000
#endif

#define HVGR_DEFAULT_DEV_MODE       0666u

struct hvgr_device *hvgr_get_device(void)
{
	return g_gpu_device;
}

static int hvgr_open(struct inode *knode, struct file *kfile)
{
	struct hvgr_ctx *ctx = NULL;
	struct hvgr_device *gdev = g_gpu_device;

	if (gdev == NULL) {
		pr_err("%s gdev is NULL", __func__);
		return -ENODEV;
	}

	ctx = hvgr_open_context(gdev, kfile);
	if (ctx == NULL) {
		hvgr_err(gdev, HVGR_DM, "%s open ctx failed.", __func__);
		return -EINVAL;
	}

	kfile->private_data = ctx;
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	kfile->liblinux_private = liblinux_pal_sync_open(current_thread_info()->kobj_ctx);
	kfile->f_mode |= FMODE_MAP_DOWNGRADE;
#endif
	return 0;
}

#define HVGR_DM_WAIT_IOCTL_MAX_TIMES 6000

void hvgr_ctx_kref_release(struct kref *ctx_kref)
{
	struct hvgr_ctx * const ctx = container_of(ctx_kref, struct hvgr_ctx, ctx_kref);

	hvgr_close_context(ctx);
}

static void hvgr_ctx_kfile_release(struct hvgr_ctx * const ctx)
{
#ifdef CONFIG_LIBLINUX_HVGR_POLL_OPT
	unsigned long flags;

	spin_lock_irqsave(&ctx->ctx_cq.event_lock, flags);
	ctx->kfile = NULL;
	spin_unlock_irqrestore(&ctx->ctx_cq.event_lock, flags);
#endif
}

static int hvgr_release(struct inode *knode, struct file *kfile)
{
	struct hvgr_ctx * const ctx = (struct hvgr_ctx * const)kfile->private_data;

	if (ctx == NULL)
		return -EINVAL;

	rcu_assign_pointer(kfile->private_data, NULL);
	hvgr_ctx_flag_set(ctx, HVGR_CTX_FLAG_DYING);
	synchronize_rcu();
	hvgr_ctx_kfile_release(ctx);
	(void)kref_put(&ctx->ctx_kref, hvgr_ctx_kref_release);
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	liblinux_pal_sync_close(kfile->liblinux_private);
#endif
	return 0;
}

HVGR_STATIC int HVGR_WEAK hvgr_test_ioctl_cfg(struct hvgr_ctx * const ctx)
{
	return 0;
}

static inline bool hvgr_is_illegal_process(struct hvgr_ctx *ctx)
{
#ifndef CONFIG_LIBLINUX
	if (likely(ctx->leader == current->group_leader))
#else
	if (likely(ctx->tgid == current->tgid))
#endif
		return false;

	hvgr_err(ctx->gdev, HVGR_DM, "ctx_%u in wrong process, ctx->tgid = %u, current->tgid = %u",
		ctx->id, ctx->tgid, current->tgid);
	return true;
}

static struct hvgr_ctx* hvgr_get_ctx(struct file *kfile)
{
	struct hvgr_ctx *ctx = NULL;

	rcu_read_lock();
	ctx = (struct hvgr_ctx *)rcu_dereference(kfile->private_data);
	if (ctx == NULL) {
		pr_err("[hvgr] get ctx is null\n");
		goto rcu_exit;
	}

	if (hvgr_is_illegal_process(ctx))
		goto rcu_exit;

	kref_get(&ctx->ctx_kref);
	rcu_read_unlock();
	return ctx;

rcu_exit:
	rcu_read_unlock();
	return NULL;
}

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
static bool hvgr_ctx_status_is_background(unsigned int cmd, struct hvgr_ctx *ctx)
{
	if (!ctx->ctx_sw.is_background)
		return false;

	if (cmd != HVGR_IOCTL_CONTEXT_SWITCH)
		return true;

	return false;
}
#endif

static long hvgr_ioctl(struct file *kfile, unsigned int cmd, unsigned long arg)
{
	struct hvgr_ctx * const ctx = hvgr_get_ctx(kfile);
	long ret = -EINVAL;

	if (ctx == NULL)
		return -EINVAL;

	if (hvgr_test_ioctl_cfg(ctx) != 0)
		hvgr_err(ctx->gdev, HVGR_DM, "[test]ctx_%u Driver NO configure.", ctx->id);

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	if (hvgr_ctx_status_is_background(cmd, ctx)) {
		hvgr_debug(ctx->gdev, HVGR_DM, "ctx_%u is background,cmd:%x.", ctx->id, cmd);
		goto exit;
	}
#endif

	switch (cmd) {
	case HVGR_IOCTL_VERSION:
		hvgr_ioctl_handle_r(ctx, HVGR_IOCTL_VERSION,
			hvgr_ioctl_get_driver_info, arg, hvgr_ioctl_para_version);
		break;
	case HVGR_IOCTL_QUERY_RSV:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_QUERY_RSV,
			hvgr_mem_ioctl_query_rsv, arg, hvgr_ioctl_para_rsv);
		break;
	case HVGR_IOCTL_CONFIGURE:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_CONFIGURE,
			hvgr_ioctl_cfg_driver, arg, hvgr_ioctl_para_drv_cfg);
		break;
	case HVGR_IOCTL_DECONFIGURE:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_DECONFIGURE,
			hvgr_ioctl_decfg_driver, arg, hvgr_ioctl_para_drv_cfg);
		break;
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	case HVGR_IOCTL_SET_UNI_RENDER:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_SET_UNI_RENDER,
			hvgr_dev_ioctl_set_uni_render, arg, hvgr_ioctl_para_set_uni_render);
		break;
#endif
	default:
		break;
	}

	if (!hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_CFGED)) {
		hvgr_err(ctx->gdev, HVGR_DM, "ctx_%u Driver NO configure.", ctx->id);
		ret = -EPERM;
		goto exit;
	}

	switch (_IOC_TYPE(cmd)) {
	case HVGR_IOCTL_MODULE_DATAN:
		ret = hvgr_ioctl_datan(ctx, cmd, arg);
		break;
	case HVGR_IOCTL_MODULE_DEV:
		ret = hvgr_ioctl_dm(ctx, cmd, arg);
		break;
	case HVGR_IOCTL_MODULE_PM:
		ret = -EINVAL;
		break;
	case HVGR_IOCTL_MODULE_MEM:
		ret = hvgr_ioctl_mem(ctx, cmd, arg);
		break;
	case HVGR_IOCTL_MODULE_SCH:
		ret = hvgr_sch_cmd_dispatch(ctx, cmd, arg);
		break;
	default:
		hvgr_err(ctx->gdev, HVGR_DM, "Ioctl cmd 0x%x is error.", cmd);
		ret = -EINVAL;
		break;
	}

exit:
	kref_put(&ctx->ctx_kref, hvgr_ctx_kref_release);
	return ret;
}

static ssize_t hvgr_read(struct file *kfile, char __user *buf, size_t count, loff_t *f_pos)
{
	struct hvgr_ctx * const ctx = hvgr_get_ctx(kfile);
	uint32_t nums;
	ssize_t ret = -EINVAL;

	if (ctx == NULL)
		return -EINVAL;

	nums = hvgr_msync_get_event_read_num(ctx, (uint32_t)count);
	if (unlikely(nums == 0))
		goto fail_exit;

	ret = (ssize_t)hvgr_token_dequeue(ctx, buf, nums);
fail_exit:
	(void)kref_put(&ctx->ctx_kref, hvgr_ctx_kref_release);
	return ret;
}


static unsigned int hvgr_poll(struct file *kfile, poll_table *wait)
{
	struct hvgr_ctx * const ctx = hvgr_get_ctx(kfile);
	bool datan_jobtrace_softq = false;
	uint32_t ret = 0;

	if (ctx == NULL)
		return -EINVAL;

	if (hvgr_msync_event_checkdata(ctx) || (atomic_read(&ctx->softq_closed) != 0)) {
		ret = POLLIN | POLLRDNORM;
		goto fail_exit;
	}

	poll_wait(kfile, &ctx->softq_wait, wait);

	if (hvgr_msync_event_checkdata(ctx) || (atomic_read(&ctx->softq_closed) != 0)) {
		ret = POLLIN | POLLRDNORM;
		goto fail_exit;
	}

	/* check if the softq used by Datan JobTrace is not empty */
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_LOCAL, KDATAN_TL_LOCAL_CHK_SOFTQ,
		&datan_jobtrace_softq);
	if (datan_jobtrace_softq)
		ret = POLLIN | POLLRDNORM;

fail_exit:
	(void)kref_put(&ctx->ctx_kref, hvgr_ctx_kref_release);
	return ret;
}

static int hvgr_mmap(struct file *kfile, struct vm_area_struct *vma)
{
	struct hvgr_ctx * const ctx = hvgr_get_ctx(kfile);
	int ret;

	if (ctx == NULL)
		return -EINVAL;

	hvgr_systrace_begin("%s vma 0x%lx~0x%lx", __func__, vma->vm_start, vma->vm_end);
	ret = hvgr_mem_mmap(ctx, vma);
	(void)kref_put(&ctx->ctx_kref, hvgr_ctx_kref_release);
	hvgr_systrace_end();
	return ret;
}

static int hvgr_check_flags(int flags)
{
	if (((uint64_t)flags & O_CLOEXEC) == 0)
		return -EINVAL;

	return 0;
}

static bool hvgr_get_unmapped_area_para_check(struct hvgr_ctx *ctx, unsigned long addr,
	unsigned long len, unsigned long pgoff, unsigned long flags)
{
	if ((flags & MAP_FIXED) != 0 || addr != 0 || len > TASK_SIZE)
		return false;

	if (pgoff == PFN_DOWN(HVGR_MEM_4G_RSV_OFFSET)) {
		if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT))
			return false;

		if (len != (HVGR_MEM_4G_PAGES << PAGE_SHIFT))
			return false;
	}

	return true;
}

static void hvgr_del_mmap_fail_area(struct hvgr_ctx *ctx, unsigned long pgoff)
{
	uint32_t index;
	struct hvgr_mem_area *area = NULL;

	if (pgoff < PFN_DOWN(HVGR_MEM_BITMAP_START_OFFSET) ||
		pgoff >= PFN_DOWN(HVGR_MEM_BITMAP_END_OFFSET))
		return;

	index = (uint32_t)(pgoff - PFN_DOWN(HVGR_MEM_BITMAP_START_OFFSET));
	area = hvgr_mem_pend_area_del(&ctx->mem_ctx, index);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u no pend area for %u", ctx->id, index);
		return;
	}

	if(area->property == HVGR_MEM_PROPERTY_SAME_VA)
		hvgr_mem_area_ref_put(area);
}

#if defined(CONFIG_LIBLINUX)
static unsigned long hvgr_mem_get_unmapped_area_compat(struct hvgr_ctx *ctx, unsigned long len,
	unsigned long pgoff, unsigned long flags)
{
	struct vm_unmapped_area_info area_info = {0};

	area_info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	area_info.length = len;
	area_info.low_limit = 0U;
	area_info.high_limit = (u64)1 << 32U;

	return vm_unmapped_area(&area_info);
}
#endif

static unsigned long hvgr_get_unmapped_area(struct file *kfile, unsigned long addr,
	unsigned long len, unsigned long pgoff, unsigned long flags)
{
	unsigned long ret;
	struct hvgr_ctx *ctx = hvgr_get_ctx(kfile);

	if (unlikely(ctx == NULL))
		return -ENODEV;

	hvgr_systrace_begin("%s 0x%lx:0x%lx:0x%lx", __func__, len, pgoff, flags);
	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_DYING)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u dying addr 0x%lx len 0x%lx pgoff 0x%lx flags 0x%lx error",
			ctx->id, addr, len, pgoff, flags);
		ret = -EINVAL;
		goto exit;
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u, addr 0x%lx len 0x%lx pgoff 0x%lx flags 0x%lx",
		__func__, __LINE__, ctx->id, addr, len, pgoff, flags);

	if (!hvgr_get_unmapped_area_para_check(ctx, addr, len, pgoff, flags)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u, addr 0x%lx len 0x%lx pgoff 0x%lx flags 0x%lx error",
			__func__, __LINE__, ctx->id, addr, len, pgoff, flags);
		ret = -EINVAL;
		goto exit;
	}

	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT))
#if defined(CONFIG_LIBLINUX)
		ret = hvgr_mem_get_unmapped_area_compat(ctx, len, pgoff, flags);
#else
		ret = current->mm->get_unmapped_area(kfile, addr, len, pgoff, flags);
#endif
	else
		ret = hvgr_mem_get_unmapped_area(ctx, len, pgoff, flags);

	if (IS_ERR_VALUE(ret)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u (%d:%d:%s) get va 0x%lx:0x%lx fail %ld",
			ctx->id, ctx->tgid, ctx->pid,
			(hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT) ? "32" : "64"), len, pgoff, ret);
		hvgr_del_mmap_fail_area(ctx, pgoff);
	}

exit:
	(void)kref_put(&ctx->ctx_kref, hvgr_ctx_kref_release);
	hvgr_systrace_end();
	return ret;
}

static const struct file_operations hvgr_kmd_fops = {
	.owner = THIS_MODULE,
	.open = hvgr_open,
	.release = hvgr_release,
	.unlocked_ioctl = hvgr_ioctl,
	.compat_ioctl = hvgr_ioctl,
	.read = hvgr_read,
	.poll = hvgr_poll,
	.mmap = hvgr_mmap,
	.check_flags = hvgr_check_flags,
	.get_unmapped_area = hvgr_get_unmapped_area,
};

void hvgr_free_irqs(struct hvgr_device *gdev)
{
	int i;
	struct hvgr_irq *p = NULL;

	for (i = 0; i < GPU_IRQ_CNT; i++) {
		p = &gdev->dm_dev.irqs[i];
		if (p->reg_done == IRQ_REGISTER_DONE) {
			(void)free_irq(p->irq, gdev);
			p->reg_done = 0;
		}
	}
}

void hvgr_synchronize_irqs(struct hvgr_device *gdev)
{
	int i;

	for (i = 0; i < GPU_IRQ_CNT; i++) {
		if (gdev->dm_dev.irqs[i].irq != 0)
			synchronize_irq(gdev->dm_dev.irqs[i].irq);
	}
}

int hvgr_register_irq(struct hvgr_device * const gdev,
	enum hvgr_irqs_type type, irq_handler_t handler)
{
	int val;
	struct hvgr_irq *p = NULL;

	if (gdev == NULL)
		return -ENODEV;
	if ((type >= GPU_IRQ_CNT) || (handler == NULL)) {
		hvgr_err(gdev, HVGR_DM, "register irq failed. type is %d", type);
		goto irq_failed;
	}
	p = &gdev->dm_dev.irqs[type];

	if (p->reg_done == IRQ_REGISTER_DONE) {
		hvgr_err(gdev, HVGR_DM,
			"register irq failed. %s is registered.", dev_name(gdev->dev));
		goto irq_failed;
	}

	val = request_irq(p->irq, handler, p->flags | IRQF_SHARED, dev_name(gdev->dev), gdev);
	if (val) {
		hvgr_err(gdev, HVGR_DM, "interrupt %d request failed\n", p->irq);
		goto irq_failed;
	}
	p->reg_done = IRQ_REGISTER_DONE;
	return 0;

irq_failed:
	hvgr_free_irqs(gdev);
	return -1;
}


int hvgr_dm_init(struct hvgr_device * const gdev)
{
	int ret;

	ret = hvgr_fcp_loader_init(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "hvgr_fcp_loader_init failed fcp %d", ret);
		return ret;
	}

	hvgr_get_info_from_reg(gdev);


	atomic_set(&gdev->ctx_num, 0);
	mutex_init(&gdev->dm_dev.ctx_list_mutex);
	INIT_LIST_HEAD(&gdev->dm_dev.ctx_list);
	mutex_init(&gdev->dm_dev.swapped_out_list_mutex);
	INIT_LIST_HEAD(&gdev->dm_dev.swapped_out_list);
	mutex_init(&gdev->dm_dev.switch_fs_mutex);
	INIT_LIST_HEAD(&gdev->dm_dev.switch_fs_list);
	(void)hvgr_platform_tbu_init(gdev);
	hvgr_dfx_init(gdev);
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	mutex_init(&gdev->dm_dev.mcu_2d_proc_mutex);
#endif

#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_lowpa_worker_init(gdev);
	hvgr_mcu_shrinker_init(gdev);
#endif


	ret = hvgr_register_irq(gdev, GPU_IRQ, hvgr_gpu_irq);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "GPU_IRQ register fail.");
		return ret;
	}

	ret = hvgr_fcp_init(gdev);
	if (ret != 0)
		hvgr_err(gdev, HVGR_DM, "FCP_IRQ register fail.");

	return ret;
}

void hvgr_dm_term(struct hvgr_device * const gdev)
{
	hvgr_free_irqs(gdev);
	hvgr_dfx_term(gdev);


#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	hvgr_lowpa_worker_term(gdev);
	hvgr_mcu_shrinker_term(gdev);
#endif
}

/* Register GPU device to kernel. */
int hvgr_misc_register(struct hvgr_device *gdev)
{
	gdev->misc_dev.name = gdev->devname;
	gdev->misc_dev.minor = MISC_DYNAMIC_MINOR;
	gdev->misc_dev.fops = &hvgr_kmd_fops;
	gdev->misc_dev.parent = get_device(gdev->dev);
	gdev->misc_dev.mode = HVGR_DEFAULT_DEV_MODE;
	return misc_register(&gdev->misc_dev);
}
struct hvgr_callback {
	int (*init_func)(struct hvgr_device * const gdev);
	void (*term_func)(struct hvgr_device * const gdev);
};

struct hvgr_callback hvgr_callback_entry[] = {
	{hvgr_dm_init, hvgr_dm_term},
	{hvgr_pm_init, hvgr_pm_term},
	{hvgr_mem_init, hvgr_mem_term},
	{hvgr_sch_init, hvgr_sch_term},
	{hvgr_datan_init, hvgr_datan_term},
	{hvgr_wq_init, NULL},
	{hvgr_dmd_msg_init, hvgr_dmd_msg_term},
#if defined(HVGR_SIMULATE) && HVGR_SIMULATE
	{hvgr_cq_simu_init, hvgr_cq_simu_term},
#endif
#if hvgr_version_ge(HVGR_V350)
	{hvgr_ctx_common_ctx_init, hvgr_ctx_common_ctx_term},
#endif
	{hvgr_hts_init, hvgr_hts_term},
};

static int hvgr_module_init(struct hvgr_device *gdev, unsigned int *step)
{
	int ret;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(hvgr_callback_entry); i++) {
		if (hvgr_callback_entry[i].init_func == NULL)
			continue;

		ret = hvgr_callback_entry[i].init_func(gdev);
		if (ret != 0) {
			*step = i;
			hvgr_err(gdev, HVGR_DM, "%s module %d in init failed!", __func__, i);
			return ret;
		}
	}

	*step = i;
	return 0;
}

static void hvgr_module_term(struct hvgr_device *gdev, unsigned int step)
{
	while (step != 0) {
		step--;
		if (hvgr_callback_entry[step].term_func == NULL)
			continue;

		hvgr_callback_entry[step].term_func(gdev);
	}
}

#if hvgr_version_lt(HVGR_V350)
static void hvgr_power_on_gpu(struct hvgr_device *gdev)
{
	if (!hvgr_is_always_on(gdev))
		return;

	/*
	 * When the FPGA/ASIC device is powered on, it is unstable.
	 * If the job is submitted immediately, There will be GPU hang.
	 * Therefore, we need to power on the GPU in the device probe
	 * phase.
	 */
	(void)hvgr_pm_request_gpu(gdev, false);
	/*
	 * After hvgr_pm_request_gpu is executed, user_ref is incremented
	 * by one. We need to call hvgr_pm_release_gpu to reduce
	 * user_ref by one.
	 * The FPGA/ASIC is in always_on mode. Therefore, when
	 * hvgr_pm_release_gpu is executed, only user_ref is
	 * decreased by one and the GPU is not powered off.
	 */
	hvgr_pm_release_gpu(gdev);
}
#endif

static void hvgr_change_mode(struct hvgr_device * const gdev)
{
}

int hvgr_device_probe(struct hvgr_device *gdev)
{
	int ret = 0;
	unsigned int step = 0;

	g_gpu_device = gdev;

	(void)scnprintf(gdev->devname, HVGR_DEV_NAME_NUM, "%s%d", HVGR_DIRVER_NAME, g_gpu_counter);
	/* debugfs & log module init frist */
	(void)hvgr_debugfs_init(gdev);
	hvgr_log_init(gdev);

	/* get external operation object */
	gdev->subsys_ops = hvgr_subsys_ops_get();
	if (unlikely(gdev->subsys_ops == NULL)) {
		hvgr_err(gdev, HVGR_DM, "%s get external operation object failed.", __func__);
		goto probe_fail;
	}


	if (unlikely(hvgr_platform_smmu_set_sid(gdev) != 0)) {
		hvgr_err(gdev, HVGR_DM, "%s hvgr platform smmu set sid failed", __func__);
		goto probe_fail;
	}

	/* GPU Buck on */
	ret = hvgr_pm_gpu_access_enable(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "%s hvgr_pm_gpu_access_enable failed %d", __func__, ret);
		goto buck_on_fail;
	}

	hvgr_change_mode(gdev);

	/* All module init. */
	ret = hvgr_module_init(gdev, &step);
	if (ret != 0)
		goto probe_fail;

	/* Register GPU device to kernel. */
	ret = hvgr_misc_register(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "%s hvgr_misc_register failed %d", __func__, ret);
		goto probe_fail;
	}

	/* GPU Buck off */
	ret = hvgr_pm_gpu_access_disable(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_DM, "%s hvgr_pm_gpu_access_disable failed %d", __func__, ret);
		goto buck_off_fail;
	}

	g_gpu_counter++;
#if hvgr_version_lt(HVGR_V350)
	hvgr_power_on_gpu(gdev);
#endif
	return 0;

buck_off_fail:
	misc_deregister(&gdev->misc_dev);
probe_fail:
	hvgr_fcp_loader_term(gdev);
	hvgr_module_term(gdev, step);
	hvgr_pm_gpu_access_disable(gdev);
	hvgr_pm_gpu_access_term(gdev);
buck_on_fail:
	hvgr_debugfs_term(gdev);
	g_gpu_device = NULL;
	return -1;
}

int hvgr_device_remove(struct hvgr_device *gdev)
{
	hvgr_hts_free_events_memory(gdev);
	hvgr_fcp_loader_term(gdev);
	misc_deregister(&gdev->misc_dev);
	hvgr_module_term(gdev, ARRAY_SIZE(hvgr_callback_entry));
	hvgr_pm_gpu_access_term(gdev);
	hvgr_debugfs_term(gdev);

	g_gpu_device = NULL;
	return 0;
}

