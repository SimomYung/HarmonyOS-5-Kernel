/*
 * bootdevice.c
 *
 * bootdevice init
 *
 * Copyright (c) 2018-2020 Huawei Technologies Co., Ltd.
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

#include <linux/bootdevice/bootdevice.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <securec.h>

#define SCINNERSTAT 0x3A0
#define EMMC_UFS_SEL BIT(15)
#define BOOT_EMMC 0
#define BOOT_UFS 1
#define BOOT_NAME_LEN 32

static const char device_name[DEVICE_TYPE_MAX][BOOT_NAME_LEN] = {
	"ufs",
	"emmc",
	"ssd",
	"norflash"
};

static int get_bootdevice_type_from_dts(u32 *type)
{
	struct device_node *boot_np = NULL;
	char node_name[BOOT_NAME_LEN];
	u32 i, temp_type;
	int ret;

	boot_np = of_find_compatible_node(NULL, NULL, "hisilicon,bootdevice");
	if (!boot_np) {
		pr_err("can't find bootdevice dts node\n");
		return -ENODEV;
	}

	/* BIT0:ufs, BIT1:EMMC, BIT2:SSD, BIT3:Norflash，other:reserved. */
	temp_type = 0;
	for (i = DEVICE_TYPE_UFS; i < DEVICE_TYPE_MAX; i++) {
		(void)memset_s(node_name, BOOT_NAME_LEN, 0, BOOT_NAME_LEN);
		ret = snprintf_s(node_name, BOOT_NAME_LEN, BOOT_NAME_LEN - 1, "boot-from-%s", device_name[i]);
		if (ret == -1)
			continue;
		
		if (of_find_property(boot_np, node_name, NULL)) 
			temp_type |= (0x1 << i);
	}

	if (temp_type) {
		*type = temp_type;
		ret = 0;
	} else {
		ret = -EFAULT;
	}
	return ret;
}

static int get_bootdevice_type_from_reg(u32 *type)
{
	void __iomem *sysctrl = NULL;
	struct device_node *sys_np = NULL;

	sys_np = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
	if (!sys_np) {
		pr_err("can't find sysctrl, get bootdevice failed\n");
		return -ENXIO;
	}

	sysctrl = of_iomap(sys_np, 0);
	if (!sysctrl) {
		pr_err("sysctrl of_iomap failed, can not get bootdevice type\n");
		return -ENOMEM;
	}

	if ((readl(sysctrl + SCINNERSTAT) & EMMC_UFS_SEL) == BOOT_EMMC)
		*type = (0x1 << DEVICE_TYPE_EMMC);
	else
		*type = (0x1 << DEVICE_TYPE_UFS);

	iounmap(sysctrl);
	return 0;
}

static void set_bootdevice_type_to_node(u32 type)
{
	/* to ensure compatibility, the eMMC and UFS processes are not modified here. */
	if (type & (0x1 << DEVICE_TYPE_UFS))
		set_bootdevice_type(BOOT_DEVICE_UFS);
	else if (type & (0x1 << DEVICE_TYPE_EMMC))
		set_bootdevice_type(BOOT_DEVICE_EMMC);
	else
		set_bootdevice_type(type);
}

static void show_bootdevice_type(u32 type)
{
	u32 i;

	if (type == 0) {
		pr_info("no storage bootdevice.\n");
		return;
	}

	for (i = DEVICE_TYPE_UFS; i < DEVICE_TYPE_MAX; i++) {
		if (type & (0x1 << i))
			pr_info("storage bootdevice: %s\n", device_name[i]);
	}
}

static int __init bootdevice_init(void)
{
	int err;
	u32 type = 0;

	err = get_bootdevice_type_from_dts(&type);
	if (err)
		err = get_bootdevice_type_from_reg(&type);

	if (err){
		pr_err("can't find boot type.\n");
		return err;
	}
	set_bootdevice_type_to_node(type);
	show_bootdevice_type(type);
	return err;
}
arch_initcall(bootdevice_init);
