/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
/*
 * Copyright(C) 2023 Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/plist.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/random.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <linux/miscdevice.h>
#include "../rasbase/rasbase.h"
#include "../rasbase/rasprobe.h"
#include "../rasbase/rasproc.h"

#define FAULT_PERCENT_SEED 0x64

static int rest_count = 1;
static int timeout_count = 1;
static int rest_fault_count = 1;
static int gpufault_count = 1;
static int fence_timeout_count = 1;
static int job_timeout_count = 1;
static int job_count = 1;
static int gpu_page_count = 1;

/* for fence timeout worker memory */
enum fault_type {
    FAULT_NONE = 0,
    FAULT_FENCE_TIMEOUT,
    FAULT_GPU_FAULT,
    FAULT_SOFT_RESET,
    FAULT_CHIPSET_HUNG,
    FAULT_GPU_JOB_TIMEOUT,
    FAULT_GPU_PAGE_FAULT,
    FAULT_FREQUENT_POWER_ON_AND_OFF
};

struct fault_ops {
    const char *name;
    enum fault_type fault;
};

static const struct fault_ops fault_ops_list[] = {
  {.name = "soft_reset", .fault = FAULT_SOFT_RESET},
  {.name = "fence_timeout", .fault = FAULT_FENCE_TIMEOUT},
  {.name = "gpu_fault", .fault = FAULT_GPU_FAULT},
  {.name = "chipset_hung", .fault = FAULT_CHIPSET_HUNG},
  {.name = "gpu_job_timeout", .fault = FAULT_GPU_JOB_TIMEOUT},
  {.name = "gpu_page_fault", .fault = FAULT_GPU_PAGE_FAULT},
  {.name = "power_on_off", .fault = FAULT_FREQUENT_POWER_ON_AND_OFF},
};

#define NAME_LEN 128
struct fault_impl {
    pid_t pid;
    enum fault_type fault;
    /* fault input para */
    char fault_name[NAME_LEN];
    unsigned long jiff;
    int times;
    int delay;
    unsigned int space;
    int fault_count;
    int fault_percent;
};

#define FAULT_MAX 32
struct fault_list {
    rwlock_t rwk;
    struct fault_impl impl[FAULT_MAX];
};

/* record the faults which was injected. */
static struct fault_list fault_injected;
static struct fault_ops *get_fault_ops(enum fault_type type)
{
    int i = 0;

    for (i = 0; i < ARRAY_SIZE(fault_ops_list); i++) {
        if (fault_ops_list[i].fault == type) {
            return (struct fault_ops *)&fault_ops_list[i];
}
    }
    return NULL;
}

static const char *type2name(enum fault_type type)
{
    struct fault_ops *ops = get_fault_ops(type);

    return (ops) ? ops->name : "fault_unknown";
}

static enum fault_type name2type(const char *name)
{
    int i = 0;

    for (i = 0; i < ARRAY_SIZE(fault_ops_list); i++) {
        if (strcmp(name, fault_ops_list[i].name) == 0) {
            return fault_ops_list[i].fault;
}
    }
    return FAULT_NONE;
}

struct fault_impl *fault_include(enum fault_type type)
{
    int i = 0;
    struct fault_impl *fault = NULL;

    for (i = 0; i < ARRAY_SIZE(fault_injected.impl); i++) {
        if (fault_injected.impl[i].fault == type) {
            fault = &fault_injected.impl[i];
            break;
        }
    }
    return fault;
}

int is_fault(struct fault_impl *fault)
{
    unsigned long jiff = jiffies;

    /* 1.check time to fault */
    if (fault->space > 0) {
        if (fault->jiff == 0 || (jiff - fault->jiff) > msecs_to_jiffies(fault->space)) {
            fault->jiff = jiff;
        } else {
            return 0;
}
    }
    /* 2.check times is match */
    if (fault->times == -1) {
        return 1;
}
    if (fault->times > 0) {
        return 1;
}
    return 0;
}

int set_args(struct fault_impl *fault, char *prm)
{
    if (!prm) {
        return 0;
}
    rasbase_set_func(fault, fault, prm, name2type);
    rasbase_set(fault, times, prm);
    rasbase_set(fault, space, prm);
    rasbase_set(fault, delay, prm);
    rasbase_set(fault, fault_count, prm);
    rasbase_set(fault, fault_percent, prm);
    rasbase_cset(fault, fault_name, prm);
    return 0;
}

int fit_set_args(struct fault_impl *fault, int args, char *argv[])
{
    int i;

    /* init fault args */
    fault->times = -1;
    fault->fault_count = 0;
    fault->fault_percent = 0;
    for (i = 0; i < args; i++) {
        ras_retn_iferr(set_args(fault, argv[i]));
}
    ras_retn_if((fault->fault == FAULT_NONE), -EINVAL);
    /* fence timeout 10s */
    if (fault->fault == FAULT_FENCE_TIMEOUT && fault->delay == 0) {
        fault->delay = 10000; /* 10000ms = 10s */
}
    return 0;
}

/* Convert arguments to faults, then inject and inject them. */
static struct fault_impl *args2fault(int args, char *argv[])
{
    int i = 0;
    struct fault_impl *fault = NULL;
    struct fault_impl fault_tmp;

    /* 1. check the commands */
    (void)memset_s(&fault_tmp, sizeof(struct fault_impl), 0, sizeof(struct fault_impl));
    if (fit_set_args(&fault_tmp, args, argv))
        return NULL;

    /* 2. manage the faults */
    for (i = 0; i < ARRAY_SIZE(fault_injected.impl); i++) {
        if (fault == NULL && fault_injected.impl[i].fault == FAULT_NONE) {
            fault = &fault_injected.impl[i];
}
        /* fault already exist */
        if (fault_injected.impl[i].fault == fault_tmp.fault) {
            fault = &fault_injected.impl[i];
            break;
        }
    }
    /* 3. inject */
    if (fault == NULL)
        return NULL;
    (void)memcpy_s(fault, sizeof(struct fault_impl), &fault_tmp, sizeof(struct fault_impl));
    return fault;
}

int isErrorCondition(int probability)
{
    unsigned char random_byte;
    /* 获取一个随机字节 */
    get_random_bytes(&random_byte, sizeof(random_byte));
    pr_err("rgpu isErrorCondition  random_byte:%s remain 0x64:%d", random_byte, (random_byte % FAULT_PERCENT_SEED)) ;
    return (random_byte % FAULT_PERCENT_SEED) < probability;
}

static int rasprobe_handler_entry(hvgr_ksync_trigger_fence)
    (struct rasprobe_instance *ri, struct pt_regs *regs)
{
    struct fault_impl *fault = fault_include(FAULT_FENCE_TIMEOUT);
    if (!fault || !is_fault(fault)) {
        return 0;
    }
    if ((timeout_count % 10) == 1) {
        pr_err("rgpu fault_fence_timeout count:%d\n", fence_timeout_count);
        if (fault->times != -1) {
            msleep(fault->times);
        }else {
            msleep(5000);
        }
        fence_timeout_count++;
    }
    timeout_count++;
    return 0;
}

static int rasprobe_handler(hvgr_pm_driver_reset_cores)
    (struct rasprobe_instance *ri, struct pt_regs *regs)
{
    struct fault_impl *fault_rest = fault_include(FAULT_SOFT_RESET);
    struct fault_impl *fault_fault = fault_include(FAULT_GPU_FAULT);
    if (fault_rest && is_fault(fault_rest)) {
        if ((rest_count % 5) == 1 && fault_rest->fault_count > 0) {
            bool ret = false;
            pr_err("rgpu fault_soft_reset count:%d\n", rest_fault_count);
            rasprobe_seturn(regs, ret);
            rest_fault_count++;
            fault_rest->fault_count--;
        }
        rest_count++;
    }
    if (fault_fault && is_fault(fault_fault)) {
        bool ret = false;
        pr_err("rgpu fault_gpu_fault count:%d\n", gpufault_count);
        rasprobe_seturn(regs, ret);
        gpufault_count++;
    }
    return 0;
}

static int rasprobe_handler_entry(powerkey_down_hdl)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rr = NULL;
	struct fault_impl *fault = fault_include(FAULT_CHIPSET_HUNG);
    pr_err("rgpu rasprobe_handler_entry down\n");
	rasprobe_entry(ri, regs);
	rr = (struct RasRegs *)ri->data;
	if (!fault) {
		return 0;
}
	if (!is_fault(fault)) {
		return 0;
}
	pr_err("rgpu down: input=%ld will set null\n", (long)rr->args[0]);
	/* chipeset irq */
	rasprobe_setargptr(regs, 1, NULL); /* set NULL to ignore irq */
	return 0;
}

static int rasprobe_handler_entry(powerkey_up_hdl)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rr = NULL;
	struct fault_impl *fault = fault_include(FAULT_CHIPSET_HUNG);
    pr_err("rgpu rasprobe_handler_entry up\n");
	rasprobe_entry(ri, regs);
	rr = (struct RasRegs *)ri->data;
	if (!fault) {
		return 0;
}
	if (!is_fault(fault)) {
		return 0;
}
	pr_err("rgpu up: input=%ld will set null\n", (long)rr->args[0]);
	/* chipeset irq */
	rasprobe_setargptr(regs, 1, NULL); /* set NULL to ignore irq */
	return 0;
}

static int rasprobe_handler_entry(powerkey_1s_hdl)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rr = NULL;
	struct fault_impl *fault = fault_include(FAULT_CHIPSET_HUNG);
    pr_err("rgpu rasprobe_handler_entry 1s\n");
	rasprobe_entry(ri, regs);
	rr = (struct RasRegs *)ri->data;
	if (!fault) {
		return 0;
}
	if (!is_fault(fault)) {
		return 0;
}
	pr_err("rgpu 1s input=%ld will set null\n", (long)rr->args[0]);
	/* chipeset irq */
	rasprobe_setargptr(regs, 1, NULL); /* set NULL to ignore irq */
	return 0;
}

static int rasprobe_handler_entry(powerkey_6s_hdl)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rr = NULL;
	struct fault_impl *fault = fault_include(FAULT_CHIPSET_HUNG);
    pr_err("rgpu rasprobe_handler_entry 6s\n");
	rasprobe_entry(ri, regs);
	rr = (struct RasRegs *)ri->data;
	if (!fault) {
		return 0;
}
	if (!is_fault(fault)) {
		return 0;
}
	pr_err("rgpu 6s input=%ld will set null\n", (long)rr->args[0]);
	/* chipeset irq */
	rasprobe_setargptr(regs, 1, NULL); /* set NULL to ignore irq */
	return 0;
}

static int rasprobe_handler_entry(powerkey_8s_hdl)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rr = NULL;
	struct fault_impl *fault = fault_include(FAULT_CHIPSET_HUNG);
    pr_err("rgpu rasprobe_handler_entry 8s\n");
	rasprobe_entry(ri, regs);
	rr = (struct RasRegs *)ri->data;
	if (!fault) {
		return 0;
}
	if (!is_fault(fault)) {
		return 0;
}
	pr_err("rgpu 8s input=%ld will set null\n", (long)rr->args[0]);
	/* chipeset irq */
	rasprobe_setargptr(regs, 1, NULL); /* set NULL to ignore irq */
	return 0;
}

static int rasprobe_handler_entry(powerkey_10s_hdl)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct RasRegs *rr = NULL;
	struct fault_impl *fault = fault_include(FAULT_CHIPSET_HUNG);
    pr_err("rgpu rasprobe_handler_entry 10s\n");
	rasprobe_entry(ri, regs);
	rr = (struct RasRegs *)ri->data;
	if (!fault) {
		return 0;
}
	if (!is_fault(fault)) {
		return 0;
}
	pr_err("rgpu 10s input=%ld will set one\n", (long)rr->args[0]);
	/* chipeset irq */
	rasprobe_setargptr(regs, 1, NULL); /* set NULL to ignore irq */
	return 0;
}

static int rasprobe_handler_entry(hvgr_token_work)
	(struct rasprobe_instance *ri, struct pt_regs *regs)
{
	struct fault_impl *fault = fault_include(FAULT_GPU_JOB_TIMEOUT);
    if (!fault || !is_fault(fault)) {
        return 0;
    }
    if ((job_count % 10) == 1) {
        pr_err("rgpu fault_gpu_job_timeout count:%d\n", job_timeout_count);
        if (fault->times != -1) {
            msleep(fault->times);
        }else {
            msleep(5000);
        }
        job_timeout_count++;
    }
    job_count++;
    return 0;
}

static int rasprobe_handler(hvgr_mmu_get_flags)
    (struct rasprobe_instance *ri, struct pt_regs *regs)
{
    struct fault_impl *fault = fault_include(FAULT_GPU_PAGE_FAULT);
    if (fault && is_fault(fault)) {
        if (fault->fault_count > 0) {
            if (fault->fault_percent != 0 && isErrorCondition(fault->fault_percent)) {
                bool ret = false;
                pr_err("rgpu fault_gpu_page_fault count:%d\n", gpu_page_count);
                rasprobe_seturn(regs, ret);
                gpu_page_count++;
                fault->fault_count--;
            }
        }
    }
    return 0;
}

rasprobe_define(hvgr_pm_driver_reset_cores);
rasprobe_entry_define(hvgr_ksync_trigger_fence);
rasprobe_entry_define(powerkey_down_hdl); /* chipset_hung */
rasprobe_entry_define(powerkey_up_hdl); /* chipset_hung */
rasprobe_entry_define(powerkey_1s_hdl); /* chipset_hung */
rasprobe_entry_define(powerkey_6s_hdl); /* chipset_hung */
rasprobe_entry_define(powerkey_8s_hdl); /* chipset_hung */
rasprobe_entry_define(powerkey_10s_hdl); /* chipset_hung */
rasprobe_entry_define(hvgr_token_work); /* gpu_job_timeout */
rasprobe_define(hvgr_mmu_get_flags); /* gpu_page_fault */

static struct rasprobe *probes[] = {
  &rasprobe_name(hvgr_pm_driver_reset_cores),
  &rasprobe_name(hvgr_ksync_trigger_fence),
  &rasprobe_name(powerkey_down_hdl),
  &rasprobe_name(powerkey_up_hdl),
  &rasprobe_name(powerkey_1s_hdl),
  &rasprobe_name(powerkey_6s_hdl),
  &rasprobe_name(powerkey_8s_hdl),
  &rasprobe_name(powerkey_10s_hdl),
  &rasprobe_name(hvgr_token_work),
  &rasprobe_name(hvgr_mmu_get_flags)
};

static int cmd_main(void *data, int argc, char *args[])
{
    ras_retn_if(argc == 0, -EINVAL);
    ras_retn_if(args2fault(argc, args) == NULL, -EINVAL);
    return 0;
}

static int proc_ops_show(rGPU)(struct seq_file *m, void *v)
{
    int i = 0;
    struct fault_impl *impl = NULL;

    for (i = 0; i < ARRAY_SIZE(fault_injected.impl); i++) {
        impl = &fault_injected.impl[i];
        if (impl->fault == FAULT_NONE) {
            continue;
}
        seq_printf(m, "%2d\t%s\t time=%d\t count=%d\t percent=%d\n", i, type2name(impl->fault),
        impl->times, impl->fault_count, impl->fault_percent);
    }
    return 0;
}

static int proc_ops_open(rGPU)(struct inode *inode, struct file *file)
{
    return single_open(file, proc_ops_show(rGPU), NULL);
}

static ssize_t proc_ops_write(rGPU)(struct file *filp, const char __user *bff, size_t count, loff_t *data)
{
    char buf_cmd[256];

    if (unlikely(count >= sizeof(buf_cmd)))
        return -ENOMEM;
    (void)memset_s(buf_cmd, sizeof(buf_cmd), 0, sizeof(buf_cmd));
    ras_retn_iferr(copy_from_user(buf_cmd, bff, count));
    ras_retn_iferr(ras_args(buf_cmd, count, cmd_main, NULL));
    return count;
}

#define MODULE_NAME "rGPU"
proc_ops_define(rGPU);

static int tool_init(void)
{
    /* 1. initialize memory */
    ras_debugset(0);
    ras_retn_iferr(ras_check());
    (void)memset_s(&fault_injected, sizeof(struct fault_list), 0, sizeof(struct fault_list));
    /* 3. initialize probes and interface */
    ras_retn_iferr(register_rasprobes(probes, ARRAY_SIZE(probes)));
    ras_retn_iferr(proc_init(MODULE_NAME, &proc_ops_name(rGPU), &fault_injected));
    return 0;
}

static void tool_exit(void)
{
    /* 1.destroy interface and probes */
    proc_exit(MODULE_NAME);
    unregister_rasprobes(probes, ARRAY_SIZE(probes));
    /* 2.destroy the workqueue and clean memory */
    (void)memset_s(&fault_injected, sizeof(struct fault_list), 0, sizeof(struct fault_list));
}
module_init(tool_init);
module_exit(tool_exit);
MODULE_DESCRIPTION("GPU faults inject.");
MODULE_LICENSE("GPL");
MODULE_VERSION("V001R001C151-1.0");
