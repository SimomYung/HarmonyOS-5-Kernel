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
#include "cmdlist_client.h"
#include "cmdlist_dev.h"
#include "cmdlist_drv.h"

struct cmdlist_ioctl_data {
	uint32_t ioctl_cmd;
	int32_t (* ioctl_func)(struct cmdlist_table *cmd_table, struct cmdlist_node_client *);
};

static struct cmdlist_ioctl_data g_ioctl_data[] = {
	{ CMDLIST_CREATE_CLIENT, cmdlist_create_client },
	{ CMDLIST_SIGNAL_CLIENT, cmdlist_signal_client },
	{ CMDLIST_RELEASE_CLIENT, cmdlist_release_client_or_list },
	{ CMDLIST_APPEND_NEXT_CLIENT, cmdlist_append_next_client },
#ifdef CONFIG_UKMD_DEBUG_ENABLE
	{ CMDLIST_DUMP_USER_CLIENT, cmdlist_dump_client },
	{ CMDLIST_DUMP_SCENE_CLIENT, cmdlist_dump_scene },
#endif
};

static int32_t cmdlist_open(struct inode *inode, struct file *file)
{
	struct cmdlist_private *cmd_priv = NULL;
	int32_t ret = 0;
	int32_t tgid;
	struct user_info_node *node = NULL;

	if (unlikely(!inode || !file)) {
		ukmd_pr_err("fail to open!");
		return -EINVAL;
	}

	cmd_priv = cmdlist_get_priv_by_inode(inode);
	if (unlikely(!cmd_priv)) {
		ukmd_pr_err("cannot get cmd_priv!");
		return -EINVAL;
	}

	file->private_data = NULL;
	if (cmd_priv->device_initialized == 0) {
		ukmd_pr_err("device is not initialized!");
		return -EINVAL;
	}
	file->private_data = cmd_priv;

	down(&cmd_priv->table_sem);
	if (cmd_priv->ref_count == 0) {
		ukmd_pr_warn("pool memery status: %u, %u!",
			gen_pool_size(cmd_priv->memory_pool), gen_pool_avail(cmd_priv->memory_pool));
		ret = cmdlist_client_table_setup(&cmd_priv->cmd_table);
		if (unlikely(ret != 0)) {
			ukmd_pr_err("cmdlist client table setup failed!");
			up(&cmd_priv->table_sem);
			return -EINVAL;
		}
	}
	cmd_priv->ref_count++;
	ukmd_pr_info("cmd_priv->ref_count=%d!", cmd_priv->ref_count);

	tgid = task_tgid_vnr(current);
	node = cmdlist_get_user_info_node(cmd_priv, tgid);
	if (likely(!node)) {
		node = kzalloc(sizeof(struct user_info_node), GFP_KERNEL);
		if (likely(node)) {
			node->user_tgid = tgid;
			node->ref_cnt = 1;
			list_add_tail(&node->list_node, &cmd_priv->user_info_head);
		} else {
			ukmd_pr_err("node is nullptr");
			up(&cmd_priv->table_sem);
			return -EINVAL;
		}
	} else {
		ukmd_pr_warn("device has opened before by tgid=%d of %u times", tgid, node->ref_cnt);
		++node->ref_cnt;
	}

	up(&cmd_priv->table_sem);

	return ret;
}

static int32_t cmdlist_close(struct inode *inode, struct file *file)
{
	struct cmdlist_private *cmd_priv = NULL;
	int32_t tgid;
	struct user_info_node *node = NULL;

	if (!file) {
		ukmd_pr_err("fail to close!");
		return -EINVAL;
	}

	cmd_priv = (struct cmdlist_private *)file->private_data;
	if (!cmd_priv) {
		ukmd_pr_err("cmd_priv is null!");
		return -EINVAL;
	}

	down(&cmd_priv->table_sem);
	cmd_priv->ref_count--;
	if (cmd_priv->ref_count == 0) {
		ukmd_pr_warn("pool memery status: %d, %d!",
			gen_pool_size(cmd_priv->memory_pool), gen_pool_avail(cmd_priv->memory_pool));
		cmdlist_client_table_clear(&cmd_priv->cmd_table, cmd_priv->dev_id);
	}

	tgid = task_tgid_vnr(current);
	node = cmdlist_get_user_info_node(cmd_priv, tgid);
	if (unlikely(!node)) {
		ukmd_pr_warn("device has not opened before by tgid=%d", tgid);
	} else {
		ukmd_pr_warn("device has opened before by tgid=%d of %u times", tgid, node->ref_cnt);
		if (node->ref_cnt > 0)
			--node->ref_cnt;

		if (node->ref_cnt == 0) {
			list_del(&node->list_node);
			kfree(node);
		}
	}
	up(&cmd_priv->table_sem);

	return 0;
}

static int32_t cmdlist_dev_mmap(struct cmdlist_private *cmd_priv, struct vm_area_struct *vma)
{
	int32_t ret = 0;
	unsigned long size;
	unsigned long addr;

	if (!cmd_priv) {
		ukmd_pr_err("cmd_priv is null!");
		return -EINVAL;
	}

	if (!vma) {
		ukmd_pr_err("vma is null!");
		return -EINVAL;
	}

	if (unlikely(vma->vm_end <= vma->vm_start)) {
		ukmd_pr_err("vma->vm_end minus vm_start is invalid");
		return -1;
	}

	addr = vma->vm_start;
	size = vma->vm_end - vma->vm_start;
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	if ((size == 0) || (size > cmd_priv->sum_pool_size)) {
		ukmd_pr_err("mmap error size:%#x", size);
		return -EINVAL;
	}

	ret = remap_pfn_range(vma, addr, (cmd_priv->pool_phy_addr >> PAGE_SHIFT), size, vma->vm_page_prot);
	if (ret != 0) {
		ukmd_pr_err("failed to remap_pfn_range! ret=%d", ret);
		return ret;
	}

	return ret;
}

static int32_t cmdlist_mmap(struct file *file, struct vm_area_struct *vma)
{
	int32_t ret = 0;
	struct cmdlist_private *cmd_priv = NULL;

	if (!file) {
		ukmd_pr_err("fail to mmap!");
		return -EINVAL;
	}

	cmd_priv = (struct cmdlist_private *)file->private_data;
	if (!cmd_priv) {
		ukmd_pr_err("cmd_priv is null!");
		return -EINVAL;
	}

	down(&cmd_priv->table_sem);
	if (cmd_priv->ref_count <= 0) {
		ukmd_pr_err("dev is not open correct!");
		up(&cmd_priv->table_sem);
		return -EINVAL;
	}

	ret = cmdlist_dev_mmap(cmd_priv, vma);

	up(&cmd_priv->table_sem);

	return ret;
}

static int32_t get_cmdlist_device_info(struct cmdlist_private *cmd_priv, unsigned long arg)
{
	struct cmdlist_info info = {0};
	void __user *argp = (void __user *)(uintptr_t)arg;

	info.dev_id = cmd_priv->dev_id;
	info.pool_size = (uint32_t)cmd_priv->sum_pool_size;
	info.viraddr_base = (uint64_t)(uintptr_t)cmd_priv->pool_vir_addr;

	if (copy_to_user(argp, &info, sizeof(info))) {
		ukmd_pr_err("copy_to_user failed!");
		return -EFAULT;
	}

	ukmd_pr_info("dev_id=%u pool_size=%d", info.dev_id, info.pool_size);

	return 0;
}

static int32_t cmdlist_ioctl_user_client_handler(struct cmdlist_private *cmd_priv, uint32_t cmd, unsigned long arg)
{
	int32_t i;
	int32_t ret = -EINVAL;
	struct cmdlist_node_client user_client = {0};
	struct cmdlist_node_client __user *argp = (struct cmdlist_node_client __user *)(uintptr_t)arg;

	if (copy_from_user(&user_client, argp, sizeof(user_client)) != 0) {
		ukmd_pr_err("copy for user failed! ret=%d", ret);
		return -EINVAL;
	}

	user_client.dev_id = cmd_priv->dev_id;
	ukmd_pr_debug("cmd=%#x dev_id=%u id=%u type=%u scene_id=%u node_size=%u append_next_id=%u", cmd, user_client.dev_id,
		user_client.id, user_client.type, user_client.scene_id, user_client.node_size, user_client.append_next_id);

	for (i = 0; i < ARRAY_SIZE(g_ioctl_data); i++) {
		if (cmd == g_ioctl_data[i].ioctl_cmd) {
			ret = g_ioctl_data[i].ioctl_func(&cmd_priv->cmd_table, &user_client);
			break;
		}
	}

	if (copy_to_user(argp, &user_client, sizeof(user_client)) != 0) {
		cmdlist_release_client_or_list(&cmd_priv->cmd_table, &user_client);
		ukmd_pr_err("copy_to_user failed!");
		ret = -EFAULT;
	}

	return ret;
}

static long cmdlist_ioctl(struct file *file, uint32_t cmd, unsigned long arg)
{
	long ret = 0;
	struct cmdlist_private *cmd_priv = NULL;

	if (!file || !arg) {
		ukmd_pr_err("file or arg is null!");
		return -EINVAL;
	}

	cmd_priv = (struct cmdlist_private *)file->private_data;
	if (!cmd_priv) {
		ukmd_pr_err("cmd_priv is null!");
		return -EINVAL;
	}

	down(&cmd_priv->table_sem);
	if (cmd_priv->ref_count <= 0) {
		ukmd_pr_err("dev is not open correct!");
		up(&cmd_priv->table_sem);
		return -EINVAL;
	}

	if (cmd == CMDLIST_INFO_GET)
		ret = get_cmdlist_device_info(cmd_priv, arg);
	else
		ret = cmdlist_ioctl_user_client_handler(cmd_priv, cmd, arg);

	up(&cmd_priv->table_sem);

	return ret;
}

static struct file_operations cmdlist_fops = {
	.owner = THIS_MODULE,
	.open = cmdlist_open,
	.release = cmdlist_close,
	.unlocked_ioctl = cmdlist_ioctl,
	.compat_ioctl = cmdlist_ioctl,
	.mmap = cmdlist_mmap,
};

int32_t cmdlist_device_setup(struct cmdlist_private *cmd_priv)
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

	if (unlikely(ukmd_create_chrdev(&cmd_priv->chrdev) != 0)) {
		ukmd_pr_err("ukmd create chrdev failed!");
		class_destroy(cmd_priv->chrdev.chr_class);
		return -EINVAL;
	}

	if (cmdlist_client_table_setup(&cmd_priv->cmd_table) != 0) {
		ukmd_pr_err("cmd table setup failed!");
		ukmd_destroy_chrdev(&cmd_priv->chrdev);
		return -EINVAL;
	}
	ukmd_pr_debug("add cmdlist chr_dev device succ!");
	return 0;
}

void cmdlist_device_release(struct cmdlist_private *cmd_priv)
{
	if (!cmd_priv) {
		ukmd_pr_err("cmd_priv is null!");
		return;
	}

	down(&cmd_priv->table_sem);
	cmdlist_client_table_release(&cmd_priv->cmd_table, cmd_priv->dev_id);
	up(&cmd_priv->table_sem);

	if (cmd_priv->chrdev.chr_dev) {
		ukmd_destroy_chrdev(&cmd_priv->chrdev);
		ukmd_pr_debug("unregister cmdlist chrdev device succ!");
	}
}
