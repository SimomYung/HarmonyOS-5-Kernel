/*
 * dmabuf_reclaim_manage.c
 *
 * Copyright (C) 2024 Hisilicon Technologies Co., Ltd.
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

#include <linux/dma-buf.h>
#include <linux/dmabuf/dmabuf_reclaim_manage.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "mm_heap_helpers.h"
#include "securec.h"

static DEFINE_MUTEX(table_lock); // table lock is used while manipulating Hash Tables
static DEFINE_HASHTABLE(reclaimed_pid_table, DMABUF_HASH_BITS);

static int hash_int(int pid)
{
	return pid % DMABUF_HASH_TABLE_SIZE;
}

static struct reclaimed_pid_info *find_pid_info(int pid)
{
	struct reclaimed_pid_info *info = NULL;
	int bucket = hash_int(pid);

	hlist_for_each_entry(info, &reclaimed_pid_table[bucket], hnode) {
		if (info->pid == pid) {
			return info;
		}
	}

	return NULL;
}

int add_pid_info(int pid)
{
	int bucket =  0;
	struct reclaimed_pid_info *info = NULL;

	bucket = hash_int(pid);
	mutex_lock(&table_lock);

	info = find_pid_info(pid);
	if (info) {
		mutex_unlock(&table_lock);
		return 0;
	}

	info = (struct reclaimed_pid_info *)kmalloc(sizeof(struct reclaimed_pid_info), GFP_KERNEL);
	if (!info) {
		pr_err("%s: malloc failed", __func__);
		mutex_unlock(&table_lock);
		return -ENOMEM;
	}
	info->pid = pid;
	info->state = DMABUF_STATE_RESUMED;
	INIT_LIST_HEAD(&info->reclaimed_list);
	info->reclaimed_list_len = 0;

	hlist_add_head(&info->hnode, &reclaimed_pid_table[bucket]);
	mutex_unlock(&table_lock);
	return 0;
}

static int remove_pid_info(int pid)
{
	struct reclaimed_pid_info *info = NULL;
	mutex_lock(&table_lock);
	info = find_pid_info(pid);
	if (!info) {
		mutex_unlock(&table_lock);
		return -ENOENT;
	}
	hlist_del(&info->hnode);
	mutex_unlock(&table_lock);
	free_reclaimed_list(&info->reclaimed_list);
	kfree(info);
	return 0;
}

void free_reclaimed_list(struct list_head *reclaimed_list)
{
	struct reclaimed_dmabuf *entry, *tmp;
	list_for_each_entry_safe(entry, tmp, reclaimed_list, list) {
		list_del(&entry->list);
		kfree(entry);
	}
}

unsigned long get_reclaimed_list_len(int pid)
{
	struct reclaimed_pid_info *info;
	unsigned long len = RECLAIM_LIST_LEN_MAX;
	mutex_lock(&table_lock);
	info = find_pid_info(pid);
	if (!info) {
		pr_err("%s: get info failed\n", __func__);
		goto done;
	}
	len = info->reclaimed_list_len;
done:
	mutex_unlock(&table_lock);
	return len;
}

/* avoid concurrency issues caused by swap in */
bool insert_ino_to_reclaimed(int pid, unsigned long ino)
{
	bool ret = false;
	struct reclaimed_pid_info *info = NULL;
	struct reclaimed_dmabuf *new_buf = NULL;
	mutex_lock(&table_lock);
	info = find_pid_info(pid);
	if (!info) {
		pr_err("%s: get state_machine failed\n", __func__);
		goto done;
	}
	if (info->state != DMABUF_STATE_RECLAIM) {
		pr_err("%s: pid state not reclaim\n", __func__);
		goto done;
	}
	new_buf = (struct reclaimed_dmabuf *)kmalloc(sizeof(struct reclaimed_dmabuf), GFP_KERNEL);
	if (!new_buf) {
		pr_err("%s, malloc reclaimed dmabuf info failed\n", __func__);
		goto done;
	}
	new_buf->ino = ino;
	list_add(&new_buf->list, &info->reclaimed_list);
	info->reclaimed_list_len += 1;
	ret = true;
done:
	mutex_unlock(&table_lock);
	return ret;
}

bool get_reclaimed_list(int pid, struct list_head *reclaimed_list)
{
	bool ret = false;
	struct reclaimed_dmabuf *entry, *tmp;
	struct reclaimed_pid_info *info = NULL;
	mutex_lock(&table_lock);
	info = find_pid_info(pid);
	if (!info) {
		pr_err("%s: get node failed\n", __func__);
		goto done;
	}

	list_splice_init(&info->reclaimed_list, reclaimed_list); // transfer reclaimed list

	info->reclaimed_list_len = 0;
	ret = true;
done:
	mutex_unlock(&table_lock);
	return ret;
}

void set_reclaimed_pid_info_state(int pid, int state)
{
	struct reclaimed_pid_info *info = NULL;
	mutex_lock(&table_lock);
	info = find_pid_info(pid);
	if (!info) {
		mutex_unlock(&table_lock);
		return;
	}
	info->state = state;
	mutex_unlock(&table_lock);
}

int get_reclaimed_pid_info_state(int pid)
{
	int state = 0;
	mutex_lock(&table_lock);
	struct reclaimed_pid_info *info = find_pid_info(pid);
	if (!info) {
		mutex_unlock(&table_lock);
		return DMABUF_STATE_INVALID;
	}
	state = info->state;
	mutex_unlock(&table_lock);
	return state;
}

bool set_pid_reclaim_state(int pid)
{
	struct reclaimed_pid_info *info = NULL;
	bool ret = false;
	mutex_lock(&table_lock);
	info = find_pid_info(pid);
	if (!info) {
		goto done;
	}
	if (info->state == DMABUF_STATE_RESUMING) {
		goto done;
	}
	info->state = DMABUF_STATE_RECLAIM;
	ret = true;
done:
	mutex_unlock(&table_lock);
	return ret;
}

bool wait_resuming(int pid)
{
	int state = 0;
	mutex_lock(&table_lock);
	struct reclaimed_pid_info *info = find_pid_info(pid);
	if (!info) {
		goto done;
	}
	if (info->state == DMABUF_STATE_RESUMING) {
		mutex_unlock(&table_lock);
		return true;
	} else {
		info->state = DMABUF_STATE_RESUMING;
	}
done:
	mutex_unlock(&table_lock);
	return false;
}

int free_reclaimed_pid_info(int pid)
{
	return remove_pid_info(pid);
}