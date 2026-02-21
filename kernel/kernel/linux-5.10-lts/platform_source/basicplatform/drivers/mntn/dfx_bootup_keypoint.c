/*
 *
 *
 * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#if defined(CONFIG_DFX_MNTN_PC) || defined(CONFIG_LIBLINUX_CDC_RDA)
#include <linux/uaccess.h>
#endif
#include <securec.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/io.h>
#include <linux/of.h>
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/basicplatform/linux/util.h>
#include <platform_include/basicplatform/linux/pr_log.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <platform_include/basicplatform/linux/dfx_bootup_keypoint.h>
#include <mntn_public_interface.h>
#ifdef CONFIG_SYS_PM_ULSR
#include <linux/syscore_ops.h>
#endif
#define PR_LOG_TAG BOOTUP_KEYPOINT_TAG
#include "blackbox/rdr_print.h"

static u32 g_last_bootup_keypoint;
#ifndef CONFIG_LIBLINUX_CDC_RDA
static struct semaphore g_clear_dfx_sem;
#endif
static u64 g_bootup_keypoint_addr;
static u32 fpga_flag;
#ifdef CONFIG_SYS_PM_ULSR
static struct syscore_ops g_boot_keypoint_syscore_ops;
#endif
#if defined(CONFIG_DFX_MNTN_PC) || defined(CONFIG_LIBLINUX_CDC_RDA)
/* maximum length of keypoint string, including trailing null */
#define STR_KEYPOINT_MAXLEN   8
#endif
/*
 * Description:    set bootup_keypoint, record last_bootup_keypoint,
 * Input:          value:the value that need to set
 * Output:         NA
 * Return:         NA
 */
void set_boot_keypoint(u32 value)
{
#ifndef CONFIG_LIBLINUX_CDC_RDA
	if (value < STAGE_KERNEL_EARLY_INITCALL || value > STAGE_END) {
#else
	if (value < STAGE_CDC_ACE_RDA_START || value > STAGE_CDC_ACE_RDA_END) {
#endif
		BB_PRINT_ERR("value[%u] is invalid\n", value);
		return;
	}
#ifndef CONFIG_LIBLINUX_CDC_RDA
	if (value == STAGE_BOOTUP_END)
		up(&g_clear_dfx_sem);
#endif
	if (fpga_flag == FPGA) {
		if (!g_bootup_keypoint_addr)
			return;
		writel(value, (void *)(uintptr_t)g_bootup_keypoint_addr);
	} else {
		pmic_write_reg(BOOTUP_KEYPOINT_OFFSET, (int)value);
	}

	pr_info("%s: write success, keypoint=%d.\n", __func__, value);
}

/*
 * Description:    get bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         the current bootup_keypoint
 */
u32 get_boot_keypoint(void)
{
	u32 value;

	if (fpga_flag == FPGA) {
		if (!g_bootup_keypoint_addr)
			return 0;
		value = readl((void *)(uintptr_t)g_bootup_keypoint_addr);
	} else {
		value = pmic_read_reg(BOOTUP_KEYPOINT_OFFSET);
	}

	pr_info("%s: read success, keypoint=%d.\n", __func__, value);

	return value;
}

/*
 * Description:    get bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         the last bootup_keypoint
 */
u32 get_last_boot_keypoint(void)
{
	return g_last_bootup_keypoint;
}
#ifdef CONFIG_DFX_DIAGINFO_BASEINFO
#define KEYPOINT_STR_LEN	8
int get_last_boot_keypoint_str(const void *rdr_memory, size_t rdr_size, char *out_buf, size_t buf_size)
{
	u32 keypoint;
	char keypoint_str[KEYPOINT_STR_LEN] = {0};
	size_t len;

	(void)(rdr_memory);
	(void)(rdr_size);

	if (out_buf == NULL) {
		pr_err("%s, invalid param\n", __func__);
		return -1;
	}
	keypoint = get_last_boot_keypoint();
	(void)sprintf_s(keypoint_str, KEYPOINT_STR_LEN, "%u", keypoint);
	len = strlen(keypoint_str);
	if (buf_size < len + 1) {
		pr_err("%s, out_buf size %lu is too small for string len %lu\n", __func__, buf_size, len);
		return -1;
	}
	if (strcpy_s(out_buf, buf_size, keypoint_str) != EOK) {
		pr_err("%s, strcpy failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}
#endif

/*
 * Description:    get last bootup_keypoint from cmdline
 * Input:          NA
 * Output:         NA
 * Return:         NA
 */
static int __init early_last_bootup_keypoint_cmdline(char *last_bootup_keypoint_cmdline)
{
	long keypoint = 0;

	if (last_bootup_keypoint_cmdline == NULL) {
		pr_debug("last_bootup_keypoint_cmdline is null\n");
		return -1;
	}

	if (kstrtol(last_bootup_keypoint_cmdline, 10, &keypoint) == 0)
	    g_last_bootup_keypoint = (u32)keypoint;
	pr_debug("g_last_bootup_keypoint is [%u]\n", g_last_bootup_keypoint);
	return 0;
}

early_param("last_bootup_keypoint", early_last_bootup_keypoint_cmdline);

#ifndef CONFIG_LIBLINUX_CDC_RDA
/*
 * Description:    clear dfx tempbuffer
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int clear_dfx_happen(void *arg)
{
	pr_debug("%s start\n", __func__);
	down(&g_clear_dfx_sem);
	clear_dfx_tempbuffer();

	return 0;
}
#endif

#if defined(CONFIG_DFX_MNTN_PC) || defined(CONFIG_LIBLINUX_CDC_RDA)
static ssize_t keypoint_proc_read(struct file *file, char __user *buf, size_t bytes, loff_t *off)
{
	int len;
	u32 keypoint;
	char temp_buf[STR_KEYPOINT_MAXLEN] = {0};

	if (buf == NULL || off == NULL) {
		BB_PRINT_ERR("%s: invalid parameter.\n", __func__);
		return -EINVAL;
	}

	if (*off > 0) {
		return 0;
	}

	keypoint = get_boot_keypoint();

	len = snprintf_s(temp_buf, STR_KEYPOINT_MAXLEN, STR_KEYPOINT_MAXLEN - 1, "%u\n", keypoint);
	if (unlikely(len <= 0)) {
		BB_PRINT_ERR("%s: snprintf_s error, len=%d\n", __func__, len);
		return -EFAULT;
	}

	if (unlikely(len + 1 > (int)bytes)) {
		BB_PRINT_ERR("%s: user buffer is too small.\n", __func__);
		return -EFAULT;
	}
	if (copy_to_user(buf, temp_buf, len + 1)) {
		BB_PRINT_ERR("%s: copy_to_user error.\n", __func__);
		return -EFAULT;
	}
	*off += len + 1;

	BB_PRINT_PN("%s: read success, keypoint=%d.\n", __func__, keypoint);

	return (ssize_t)(len + 1);
}

static ssize_t keypoint_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *off)
{
	long keypoint = 0;
	char temp_buf[STR_KEYPOINT_MAXLEN] = {0};

	if (count == 0 || buf == NULL) {
		BB_PRINT_ERR("%s: invalid parameter.\n", __func__);
		return -EINVAL;
	}

	if (count >= STR_KEYPOINT_MAXLEN) {
		BB_PRINT_ERR("%s: invalid count: %lu.\n", __func__, count);
		return -EINVAL;
	}

	if (copy_from_user(temp_buf, buf, count)) {
		BB_PRINT_ERR("%s: copy_from_user fail.\n", __func__);
		return -EFAULT;
	}
	temp_buf[count] = '\0';

	if (kstrtol((const char *)temp_buf, 10, &keypoint) != 0) {
		BB_PRINT_ERR("%s: kstrtol error\n", __func__);
		return -EINVAL;
	}

#ifndef CONFIG_LIBLINUX_CDC_RDA
	if (keypoint < STAGE_OS_ZYGOTE_START || keypoint >= STAGE_END) {
#else
	if (keypoint < STAGE_CDC_ACE_RDA_START || keypoint > STAGE_CDC_ACE_RDA_END) {
#endif
		BB_PRINT_ERR("%s: invalid keypoint value:%ld.\n", __func__, keypoint);
		return -EINVAL;
	}

	set_boot_keypoint((u32)keypoint);

	BB_PRINT_PN("%s: write success, keypoint=%ld\n", __func__, keypoint);

	return (ssize_t)count;
}
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0))
static const struct proc_ops keypoint_proc_fops = {
	.proc_write = keypoint_proc_write,
	.proc_read = keypoint_proc_read,
};
#else
static const struct file_operations keypoint_proc_fops = {
	.write = keypoint_proc_write,
	.read = keypoint_proc_read,
};
#endif
#endif

#ifdef CONFIG_SYS_PM_ULSR
static int boot_keypoint_suspend(void)
{
#ifndef CONFIG_LIBLINUX_CDC_RDA
	set_boot_keypoint(STAGE_KERNEL_ULSR_SUSPEND);
#else
	set_boot_keypoint(STAGE_CDC_ACE_RDA_SUSPEND);
#endif
	return 0;
}

static void boot_keypoint_resume(void)
{
#ifndef CONFIG_LIBLINUX_CDC_RDA
	set_boot_keypoint(STAGE_KERNEL_ULSR_RESUME);
#else
	set_boot_keypoint(STAGE_CDC_ACE_RDA_RESUME);
#endif
}

static struct syscore_ops g_boot_keypoint_syscore_ops = {
	.suspend = boot_keypoint_suspend,
	.resume = boot_keypoint_resume
};
#endif
/*
 * Description:    sema_init clear_dfx_sem and kthread_run clear_dfx task
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init dfx_bootup_keypoint_init(void)
{
#ifdef CONFIG_LIBLINUX_CDC_RDA
	struct proc_dir_entry *entry = NULL;
#endif

#ifndef CONFIG_LIBLINUX_CDC_RDA
	sema_init(&g_clear_dfx_sem, 0);
	if (!kthread_run(clear_dfx_happen, NULL, "clear_dfx_happen"))
		BB_PRINT_ERR("create thread clear_dfx_happen faild\n");
#else
	pr_info("%s: init start\n", __func__);
	entry = proc_mkdir("bootup_keypoint", NULL);
	if (!entry) {
		pr_info("%s: creat /proc/devhost/root/bootup_keypoint/ fail\n", __func__);
	}
	entry = proc_create_data("keypoint", (S_IWUSR | S_IRUSR), entry, &keypoint_proc_fops, NULL);
	if (!entry) {
		pr_info("%s: creat /proc/devhost/root/bootup_keypoint/keypoint fail\n", __func__);
	}

	set_boot_keypoint(STAGE_CDC_ACE_RDA_LDK_INIT);
#endif

#ifdef CONFIG_DFX_MNTN_PC
	dfx_create_stats_proc_entry("keypoint", (S_IWUSR | S_IRUSR), &keypoint_proc_fops, NULL);
#endif
#ifdef CONFIG_SYS_PM_ULSR
	register_syscore_ops(&g_boot_keypoint_syscore_ops);
#endif
	pr_info("%s: init end\n", __func__);
	return 0;
}
module_init(dfx_bootup_keypoint_init)

/*
 * Description:    init bootup_keypoint_addr
 * Input:          NA
 * Output:         NA
 * Return:         NA
 */
static void bootup_keypoint_addr_init(void)
{
	int ret;
	struct device_node *np = NULL;
	u64 bootup_keypoint_addr;

	np = of_find_compatible_node(NULL, NULL, "hisilicon,dpufb");
	if (np == NULL)
		np = of_find_compatible_node(NULL, NULL, "hisilicon,env_flag");

	if (np == NULL) {
		BB_PRINT_ERR("NOT FOUND device node 'fb' or env_flag!\n");
		return;
	}
	ret = of_property_read_u32(np, "fpga_flag", &fpga_flag);
	if (ret) {
		BB_PRINT_ERR("failed to get fpga_flag resource\n");
		return;
	}
	if (fpga_flag == FPGA) {
		bootup_keypoint_addr = FPGA_BOOTUP_KEYPOINT_ADDR;
		g_bootup_keypoint_addr = (uintptr_t)
			ioremap_wc(bootup_keypoint_addr, sizeof(int));
		if (!g_bootup_keypoint_addr) {
			BB_PRINT_ERR("get bootup_keypoint_addr error\n");
			return;
		}
	}
}

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init early_stage_init(void)
{
	bootup_keypoint_addr_init();
#ifndef CONFIG_LIBLINUX_CDC_RDA
	set_boot_keypoint(STAGE_KERNEL_EARLY_INITCALL);
#endif
	return 0;
}
early_initcall(early_stage_init);

#ifndef CONFIG_LIBLINUX_CDC_RDA
/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init pure_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_PURE_INITCALL);
	return 0;
}
pure_initcall(pure_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init core_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_CORE_INITCALL);
	return 0;
}
core_initcall(core_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init core_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_CORE_INITCALL_SYNC);
	return 0;
}
core_initcall_sync(core_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init postcore_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_POSTCORE_INITCALL);
	return 0;
}
postcore_initcall(postcore_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init postcore_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_POSTCORE_INITCALL_SYNC);
	return 0;
}
postcore_initcall_sync(postcore_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init arch_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_ARCH_INITCALL);
	return 0;
}
arch_initcall(arch_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init arch_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_ARCH_INITCALLC);
	return 0;
}
arch_initcall_sync(arch_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init subsys_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_SUBSYS_INITCALL);
	return 0;
}
subsys_initcall(subsys_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init subsys_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_SUBSYS_INITCALL_SYNC);
	return 0;
}
subsys_initcall_sync(subsys_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init fs_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_FS_INITCALL);
	return 0;
}
fs_initcall(fs_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init fs_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_FS_INITCALL_SYNC);
	return 0;
}
fs_initcall_sync(fs_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init rootfs_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_ROOTFS_INITCALL);
	return 0;
}
rootfs_initcall(rootfs_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init device_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_DEVICE_INITCALL);
	return 0;
}
device_initcall(device_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init device_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_DEVICE_INITCALL_SYNC);
	return 0;
}
device_initcall_sync(device_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init late_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_LATE_INITCALL);
	return 0;
}
late_initcall(late_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init late_sync_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_LATE_INITCALL_SYNC);
	return 0;
}
late_initcall_sync(late_sync_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
static int __init console_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_CONSOLE_INITCALL);
	return 0;
}
console_initcall(console_stage_init);

/*
 * Description:    set bootup_keypoint
 * Input:          NA
 * Output:         NA
 * Return:         OK:success
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
static int __init security_stage_init(void)
{
	set_boot_keypoint(STAGE_KERNEL_SECURITY_INITCALL);
	return 0;
}
security_initcall(security_stage_init);
#endif
#endif
