/**
 * @file cmdlist_dev.c
 * @brief Provide equipment related interfaces
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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
#include "ukmd_log.h"

#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include "chash.h"
#include "cmdlist_node.h"
#include "cmdlist_dev.h"
#include "cmdlist_drv.h"

#define DEV_OPEN_MAX_CNT 32

struct cmdlist_process_data {
	int32_t ref_cnt;
	struct semaphore sem;
	int32_t tgid;
};

static int32_t cmdlist_open(struct inode *inode, struct file *file)
{
	struct cmdlist_private *cmd_priv = NULL;
	struct cmdlist_process_data *process_data = NULL;
	int32_t ret = 0;
	int32_t tgid;

	if (unlikely(!inode || !file)) {
		ukmd_pr_err("inode or file is nullptr!");
		return -EINVAL;
	}

	cmd_priv = cmdlist_get_priv_by_inode(inode);
	if (unlikely(!cmd_priv)) {
		ukmd_pr_err("cannot get cmd_priv!");
		return -EINVAL;
	}

	if (cmd_priv->device_initialized == 0) {
		ukmd_pr_err("device is not initialized!");
		return -EINVAL;
	}

	if (file->private_data) {
		process_data = (struct cmdlist_process_data *)file->private_data;
		tgid = process_data->tgid;
		if (process_data->ref_cnt >= DEV_OPEN_MAX_CNT) {
			ukmd_pr_err("ref_cnt=%d overflow, tgid=%d",
				process_data->ref_cnt, tgid);
			return -EINVAL;
		}
	} else {
		process_data = kzalloc(sizeof(*process_data), GFP_KERNEL);
		if (!process_data) {
			ukmd_pr_err("kzalloc cmdlist process data fail");
			return -ENOMEM;
		}
		tgid = task_tgid_vnr(current);
		sema_init(&process_data->sem, 1);
		process_data->tgid = tgid;
		file->private_data = process_data;
	}

	down(&process_data->sem);
	++process_data->ref_cnt;
	ukmd_pr_info("+, tgid=%d ref_cnt=%d", tgid, process_data->ref_cnt);

	ret = cmdlist_client_mgr_open(&cmd_priv->client_mgr, tgid);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("cmdlist_client_mgr_open fail!, ret=%d", ret);
		up(&process_data->sem);
		return ret;
	}

	ret = cmdlist_mem_mgr_open(&cmd_priv->mem_mgr, tgid);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("cmdlist_mem_mgr_open fail!, ret=%d", ret);
		up(&process_data->sem);
		return ret;
	}

	up(&process_data->sem);
	ukmd_pr_info("-");

	return ret;
}

static int32_t cmdlist_release(struct inode *inode, struct file *file)
{
	struct cmdlist_private *cmd_priv = NULL;
	struct cmdlist_process_data *process_data = NULL;

	if (unlikely(!inode || !file)) {
		ukmd_pr_err("inode or file is nullptr!");
		return -EINVAL;
	}

	cmd_priv = cmdlist_get_priv_by_inode(inode);
	if (unlikely(!cmd_priv)) {
		ukmd_pr_err("cannot get cmd_priv!");
		return -EINVAL;
	}

	process_data = (struct cmdlist_process_data *)file->private_data;
	if (!process_data) {
		ukmd_pr_err("process_data is null");
		return -EINVAL;
	}

	down(&process_data->sem);
	ukmd_pr_debug("ref_cnt=%d process-tgid=%d tgid=%d",
		process_data->ref_cnt, process_data->tgid, task_tgid_vnr(current));

	if (process_data->ref_cnt == 0) {
		ukmd_pr_warn("cmdlist not opened, cannot release");
		up(&process_data->sem);
		return 0;
	}

	--process_data->ref_cnt;

	if (process_data->ref_cnt == 0) {
		cmdlist_client_mgr_release(&cmd_priv->client_mgr, process_data->tgid);
		cmdlist_mem_mgr_release(&cmd_priv->mem_mgr, process_data->tgid);
		up(&process_data->sem);

		kfree(file->private_data);
		file->private_data = NULL;
		return 0;
	}

	up(&process_data->sem);
	return 0;
}

static int32_t cmdlist_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct cmdlist_private *cmd_priv = NULL;
	struct cmdlist_process_data *process_data = NULL;

	if (!file) {
		ukmd_pr_err("file is invalid, fail to mmap!");
		return -EINVAL;
	}

	if (!vma) {
		ukmd_pr_err("vma is invalid, fail to mmap!");
		return -EINVAL;
	}

	cmd_priv = cmdlist_get_priv_by_inode(file->f_inode);
	if (unlikely(!cmd_priv)) {
		ukmd_pr_err("cannot get cmd_priv!");
		return -EINVAL;
	}

	process_data = (struct cmdlist_process_data *)file->private_data;
	if (!process_data) {
		ukmd_pr_err("process_data is null");
		return -EINVAL;
	}

	down(&process_data->sem);
	ukmd_pr_debug("ref_cnt=%d process-tgid=%d tgid=%d",
		process_data->ref_cnt, process_data->tgid, task_tgid_vnr(current));

	if (process_data->ref_cnt <= 0) {
		ukmd_pr_warn("cmdlist not opened, cannot mmap");
		up(&process_data->sem);
		return -EINVAL;
	}
	up(&process_data->sem);

	return cmd_priv->mem_mgr.mmap(&cmd_priv->mem_mgr, vma);
}

static int32_t get_cmdlist_device_info(struct cmdlist_private *cmd_priv, unsigned long arg)
{
	struct cmdlist_info info = {0};
	void __user *argp = (void __user *)(uintptr_t)arg;

	info.memory_type = (uint8_t)cmd_priv->mem_mgr.memory_type;
	info.use_kernel_mode = cmd_priv->client_mgr.use_kernel_mode;
	info.addr_mode = cmd_priv->client_mgr.addr_mode;
	info.pool_size = (uint32_t)cmd_priv->mem_mgr.max_size;
	info.viraddr_base = (uint64_t)(uintptr_t)cmd_priv->mem_mgr.pool_vir_addr;
	info.dma_addr_base = cmd_priv->mem_mgr.pool_dma_addr;

	if (copy_to_user(argp, &info, sizeof(info))) {
		ukmd_pr_err("copy_to_user failed!");
		return -EFAULT;
	}

	ukmd_pr_debug("memory_type=%u pool_size=%d viraddr_base=0x%llx dma_addr_base=%llx",
		info.memory_type, info.pool_size, info.viraddr_base, info.dma_addr_base);

	return 0;
}

static int32_t cmdlist_ioctl_user_client_handler(struct cmdlist_private *cmd_priv, int32_t tgid,
	uint32_t cmd, unsigned long arg)
{
	int32_t ret;
	
	if (cmd == CMDLIST_ALLOC_MEM || cmd == CMDLIST_FREE_MEM) {
		ret = cmdlist_mem_mgr_ioctl(&cmd_priv->mem_mgr, tgid, cmd, arg);
		if (unlikely(ret != 0)) {
			ukmd_pr_err("cmdlist_mem_mgr_ioctl failed! ret=%d", ret);
		}
		return ret;
	}

	ret = cmdlist_client_mgr_ioctl(&cmd_priv->client_mgr, tgid, cmd, arg);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("cmdlist_client_mgr_ioctl failed! ret=%d", ret);
	}

	return ret;
}

static long cmdlist_ioctl(struct file *file, uint32_t cmd, unsigned long arg)
{
	long ret = 0;
	struct cmdlist_private *cmd_priv = NULL;
	struct cmdlist_process_data *process_data = NULL;

	if (!file || !arg) {
		ukmd_pr_err("file or arg is null!");
		return -EINVAL;
	}

	cmd_priv = cmdlist_get_priv_by_inode(file->f_inode);
	if (unlikely(!cmd_priv)) {
		ukmd_pr_err("cannot get cmd_priv!");
		return -EINVAL;
	}

	process_data = (struct cmdlist_process_data *)file->private_data;
	if (unlikely(!process_data)) {
		ukmd_pr_err("process_data is null");
		return -EINVAL;
	}

	down(&process_data->sem);
	ukmd_pr_debug("ref_cnt=%d process-tgid=%d tgid=%d",
		process_data->ref_cnt, process_data->tgid, task_tgid_vnr(current));

	if (process_data->ref_cnt <= 0) {
		ukmd_pr_warn("cmdlist not opened, cannot ioctl");
		up(&process_data->sem);
		return -EINVAL;
	}
	up(&process_data->sem);

	if (cmd != CMDLIST_INFO_GET)
		ret = cmdlist_ioctl_user_client_handler(cmd_priv, process_data->tgid, cmd, arg);
	else
		ret = get_cmdlist_device_info(cmd_priv, arg);

	return ret;
}

static struct file_operations cmdlist_fops = {
	.owner = THIS_MODULE,
	.open = cmdlist_open,
	.release = cmdlist_release,
	.unlocked_ioctl = cmdlist_ioctl,
	.compat_ioctl = cmdlist_ioctl,
	.mmap = cmdlist_mmap,
};

int32_t cmdlist_device_probe(struct cmdlist_private *cmd_priv)
{
	const char *name = cmd_priv->of_dev->of_node->name;

	cmd_priv->chrdev.chr_class = class_create(THIS_MODULE, name);
	if (IS_ERR_OR_NULL(cmd_priv->chrdev.chr_class)) {
		ukmd_pr_err("create cmdlist class fail!");
		return PTR_ERR(cmd_priv->chrdev.chr_class);
	}
	cmd_priv->chrdev.name = name;
	cmd_priv->chrdev.fops = &cmdlist_fops;
	cmd_priv->chrdev.drv_data = cmd_priv;

	ukmd_create_chrdev(&cmd_priv->chrdev);
	ukmd_pr_debug("add cmdlist chr_dev device succ!");
	return 0;
}

void cmdlist_device_remove(struct cmdlist_private *cmd_priv)
{
	if (cmd_priv->chrdev.chr_dev) {
		ukmd_destroy_chrdev(&cmd_priv->chrdev);
		ukmd_pr_debug("unregister cmdlist chrdev device succ!");
	}
}
