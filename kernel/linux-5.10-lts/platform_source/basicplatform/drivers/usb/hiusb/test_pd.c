/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: test pd driver
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/platform_drivers/usb/tca.h>
#include <linux/platform_drivers/usb/hiusb_client.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/usb.h>
#include <linux/seq_file.h>
#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_TEST_PD]%s: " fmt, __func__

#define MAX_PD_TEST_CMD_LEN   32
#define MAX_PD_CMD_LEN        32

struct test_pd {
	struct device 	*dev;
	struct hiusb_port_handle *handle;
	struct dentry *debugfs_root;
};

static int test_pd_show(struct seq_file *s, void *unused)
{
	struct test_pd *pd = s->private;
	if (pd == NULL)
		return -ENOENT;

	seq_printf(s, "Usage: echo <deviceon/deviceoff/hoston/hostoff>  >/d/usb/test_pd@/test_mode\n");
	seq_printf(s, "current usb state %s\n", chip_usb_state_string(hiusb_get_state(pd->handle)));
	return 0;
}

static int test_pd_open(struct inode *inode, struct file *file)
{
	return single_open(file, test_pd_show, inode->i_private);
}

static ssize_t test_pd_write(struct file *file,
		const char __user *ubuf, size_t count, loff_t *ppos)
{
	size_t len;
	char buf[MAX_PD_CMD_LEN] = {0};
	struct seq_file *s = file->private_data;
	struct test_pd *pd = s->private;

	if (pd == NULL)
		return -ENOENT;

	if (ubuf == NULL)
		return -EINVAL;

	len = min_t(size_t, sizeof(buf) - 1, count);
	if (copy_from_user(&buf, ubuf, len))
		return -EFAULT;
	buf[len] = '\0';

	if (!strncmp(buf, "deviceon", sizeof("deviceon") - 1)) {
		seq_printf(s, "deviceon\n");
		hiusb_device_start(pd->handle->data);
	} else if (!strncmp(buf, "deviceoff", sizeof("deviceoff") - 1)) {
		seq_printf(s, "deviceoff");
		hiusb_device_stop(pd->handle->data);
	} else if (!strncmp(buf, "hoston", sizeof("hoston") - 1)) {
		seq_printf(s, "hoston\n");
		hiusb_host_start(pd->handle->data);
	} else if (!strncmp(buf, "hostoff", sizeof("hostoff") - 1)) {
		seq_printf(s, "hostoff\n");
		hiusb_host_stop(pd->handle->data);
	} else {
		seq_printf(s, "inval para\n");
		return -EINVAL;
	}

	return count;
}

static const struct file_operations pd_test_mode_fops = {
	.open			= test_pd_open,
	.write			= test_pd_write,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

int test_pd_debugfs_init(struct test_pd *pd, struct device *dev)
{
	struct dentry *root = NULL;
	struct dentry *file = NULL;

	root = debugfs_create_dir(dev_name(dev), usb_debug_root);
	if (IS_ERR_OR_NULL(root))
		return -ENOMEM;

	file = debugfs_create_file("test_mode", S_IRUGO | S_IWUSR, root,
		pd, &pd_test_mode_fops);
	if (IS_ERR_OR_NULL(file))
		goto file_create_err;

	pd->debugfs_root = root;
	return 0;

file_create_err:
	debugfs_remove_recursive(root);
	return -ENODEV;
}

void test_pd_debugfs_exit(struct test_pd *pd)
{
	if (pd->debugfs_root == NULL)
		return;

	debugfs_remove_recursive(pd->debugfs_root);
}

static int test_pd_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct hiusb_port_handle *handle = NULL;
	struct test_pd *pd = NULL;
	struct device *dev = &pdev->dev;

	pr_err("+\n");

	handle = hiusb_port_handle_create_by_dev(dev);
	if (IS_ERR_OR_NULL(handle)) {
		ret = PTR_ERR(handle);
		pr_err("ret %d\n", ret);
		return ret;
	}

	pd = devm_kzalloc(dev, sizeof(*pd), GFP_KERNEL);
	if (!pd) {
		ret = -ENOMEM;
		goto destroy_handle;
	}

	pd->dev= dev;
	pd->handle = handle;
	ret = test_pd_debugfs_init(pd, dev);
	if (ret) {
		goto destroy_handle;
	}
	platform_set_drvdata(pdev, pd);

	pr_err("-\n");
	return ret;

destroy_handle:
	hiusb_port_handle_destroy(handle);
	return ret;
}

static int test_pd_remove(struct platform_device *pdev)
{
	struct test_pd *pd = platform_get_drvdata(pdev);

	hiusb_port_handle_destroy(pd->handle);
	test_pd_debugfs_exit(pd);
	return 0;
}

static const struct of_device_id of_test_pd_match[] = {
	{ .compatible = "hisilicon,test_pd" },
	{ }
};
MODULE_DEVICE_TABLE(of, of_test_pd_match);

static struct platform_driver test_pd_driver = {
	.probe		= test_pd_probe,
	.remove		= test_pd_remove,
	.driver		= {
		.name	= "test_pd",
		.of_match_table = of_test_pd_match,
	},
};

module_platform_driver(test_pd_driver);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Test Pd");

