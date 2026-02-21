/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: sec_region_mntn.c
 * Create : 2024-07-10
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/see/sec_region_mntn.h>
#include <mntn_subtype_exception.h>
#include "sec_region_inner.h"

static struct semaphore interrupt_happen_sem = {};
static struct task_struct *g_task = NULL;
static u32 g_interrupt_sem_init_flag = 0;

void sec_region_mntn_handler(void)
{
	if (g_interrupt_sem_init_flag != 1) {
		pr_err("[ddr_sec]: g_interrupt_sem_init_flag is not init \n");
		return;
	}
	pr_err("[ddr_sec]: open sec region mntn semaphore \n");
	up(&interrupt_happen_sem);
}

static void print_intr_ns(u32 ns)
{
	if (ns == IBQE_NON_SECURE_OP0)
		pr_err("[ddr_sec]: non-secure access \n");
	else if (ns == IBQE_SECURE_OP0)
		pr_err("[ddr_sec]: secure access \n");
	else if (ns == IBQE_PROTECT_OP0)
		pr_err("[ddr_sec]: protected access\n");
	else
		pr_err("[ddr_sec]: unknown access, ns is 0x%x\n", ns);
}

static void print_intr_opc_sc(u32 access_type)
{
	if (access_type == SC_READ_NO_SNP || access_type == SC_READ_NO_SNP_SEP  || \
	    access_type == SC_READ_SHARED)
		pr_err("[ddr_sec]: read access \n");
	else if (access_type == SC_WRITE_NO_SNP_PTL)
		pr_err("[ddr_sec]: write access \n");
	else
		pr_err("[ddr_sec]: unknown access, opc is 0x%x\n", access_type);
}

static void print_intr_mid(u32 mid)
{
	pr_err("[ddr_sec]: master_id: 0x%x\n", mid);
}

static void print_intr_opc_tbed(u32 access_type)
{
	if (access_type == TBED_ACCESS_WRITE)
		pr_err("[ddr_sec]: write access \n");
	else if (access_type == TBED_ACCESS_READ)
		pr_err("[ddr_sec]: read access \n");
	else
		pr_err("[ddr_sec]: unknown access, opc is 0x%x\n", access_type);
}

static void print_intr_opc_ibqe(u32 access_type)
{
	if (access_type == IBQE_READ_OP0 || access_type == IBQE_READ_OP1)
		pr_err("[ddr_sec]: read access \n");
	else if (access_type == IBQE_WRITE_OP0 || access_type == IBQE_WRITE_OP1)
		pr_err("[ddr_sec]: write access \n");
	else
		pr_err("[ddr_sec]: unknown access, opc is 0x%x\n", access_type);
}

static void print_intr_info_sc(struct qice_intr_mem *data)
{
	print_intr_ns(data->ns);
	print_intr_opc_sc(data->access_type);
	print_intr_mid(data->mid);
}

static void print_intr_info_tbed(struct qice_intr_mem *data)
{
	print_intr_ns(data->ns);
	print_intr_opc_tbed(data->access_type);
	print_intr_mid(data->mid);
}

static void print_intr_info_ibqe(struct qice_intr_mem *data)
{
	print_intr_ns(data->ns);
	print_intr_opc_ibqe(data->access_type);
	print_intr_mid(data->mid);
}

static void print_intr_addr(u64 phy_addr)
{
	pr_err("[ddr_sec]: err_addr: 0x%lx\n", phy_addr);
}

static int sec_region_mntn_process(void *arg)
{
	struct qice_intr_mem *intr_info = NULL;
	u64 *gic_flag_addr = NULL;
	u64 *intr_info_addr = NULL;
	u64 *err_addr = NULL;
	u64 intr_id, addr;

	pr_err("[ddr_sec]: %s start\n", __func__);
	down(&interrupt_happen_sem);

	pr_err("[ddr_sec]: qice & sc interrupt happen \n");

	gic_flag_addr = ioremap(QICE_FIQ_GIC_NUMBER_ADDR, QICE_FIQ_GIC_NUMBER_SIZE);
	if(gic_flag_addr == NULL) {
		pr_err("[ddr_sec]: gic_flag_addr ioremap fail \n");
		goto err;
	}
	intr_id = (u64)readq(gic_flag_addr);
	pr_err("[ddr_sec]: intr_id = 0x%x \n", intr_id);
	iounmap(gic_flag_addr);
	intr_info_addr = ioremap(QICE_INTR_INFO_ADDR, QICE_INTR_INFO_SIZE);
	if(intr_info_addr == NULL) {
		pr_err("[ddr_sec]: intr_info_addr ioremap fail \n");
		goto err;
	}
	intr_info = (struct qice_intr_mem *)(uintptr_t)intr_info_addr;
	if(intr_info->init_flag != DDR_SEC_INIT_END) {
		pr_err("[ddr_sec]: qice_intr_mem flag is not set \n");
		iounmap(intr_info_addr);
		goto err;
	}

	if(intr_info->err_reason == ERR_REASON_SC) {
		pr_err("[ddr_sec]: sec region interrupt in system cache \n");
		print_intr_info_sc(intr_info);
	} else if(intr_info->err_reason == ERR_REASON_MPU) {
		pr_err("[ddr_sec]: mpu interrupt in tbed \n");
		print_intr_info_tbed(intr_info);
	} else if(intr_info->err_reason == ERR_REASON_QICE) {
		pr_err("[ddr_sec]: sec region interrupt in qice \n");
		print_intr_info_ibqe(intr_info);
	} else {
		pr_err("[ddr_sec]: err_reason is %d, wrong \n", intr_info->err_reason);
	}
	iounmap(intr_info_addr);

	err_addr = ioremap(QICE_SEC_ERR_ADDR, QICE_SEC_ERR_SIZE);
	if(err_addr == NULL) {
		pr_err("[ddr_sec]: err_addr ioremap fail \n");
		goto err;
	}
	addr = (u64)readq(err_addr);
	print_intr_addr(addr);
	iounmap(err_addr);
err:
	pr_err("[ddr_sec]: rdr_syserr_process_for_ap begin \n");
	rdr_syserr_process_for_ap(MODID_AP_S_DDRC_SEC, 0, 0);
	pr_err("[ddr_sec]: rdr_syserr_process_for_ap end \n");
	return 0;
}

static int __init sec_region_mntn_init(void)
{
	sema_init(&interrupt_happen_sem, 0);
	g_task = kthread_run(sec_region_mntn_process, NULL, "sec_region_mntn_process");
	if (!g_task) {
		pr_err("[ddr_sec]: create thread sec_region_mntn_process faild\n");
		return -1;
	}
	g_interrupt_sem_init_flag = 1;
	pr_err("[ddr_sec]: create thread sec_region_mntn_process success\n");
	return 0;
}

static void __exit sec_region_mntn_exit(void)
{
	if (g_task)
		kthread_stop(g_task);
}

module_init(sec_region_mntn_init);
module_exit(sec_region_mntn_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("sec region mntn module");
