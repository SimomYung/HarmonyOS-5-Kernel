/*
 * dmabuf_reclaim_manage.h
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
#ifndef _DMABUF_RECLAIM_MANAGE_H
#define _DMABUF_RECLAIM_MANAGE_H

#include <linux/atomic.h>
#include <linux/dma-buf.h>
#include <linux/err.h>
#include <linux/hashtable.h>
#include <linux/list.h>

#define RECLAIM_LIST_LEN_MAX 64
#define DMABUF_HASH_TABLE_SIZE 256
#define DMABUF_HASH_BITS 8

enum dmabuf_state_type {
	DMABUF_STATE_INVALID = 0,
	DMABUF_STATE_RESUMING,
	DMABUF_STATE_RESUMED,
	DMABUF_STATE_RECLAIM,
};

struct reclaimed_dmabuf {
	unsigned long ino;
	struct list_head list;
};

struct reclaimed_pid_info {
	int pid;
	int state;
	struct list_head reclaimed_list;
	unsigned long reclaimed_list_len;
	struct hlist_node hnode;
};

#ifdef CONFIG_DMABUF_RECLAIM
unsigned long get_reclaimed_list_len(int pid);
bool insert_ino_to_reclaimed(int pid, unsigned long ino);
bool get_reclaimed_list(int pid, struct list_head *reclaimed_list);
int add_pid_info(int pid);
void set_reclaimed_pid_info_state(int pid, int state);
int get_reclaimed_pid_info_state(int pid);
bool set_pid_reclaim_state(int pid);
bool wait_resuming(int pid);
int free_reclaimed_pid_info(int pid);
void free_reclaimed_list(struct list_head *reclaimed_list);
#else
static unsigned long get_reclaimed_list_len(int pid)
{
	return 0;
}

static bool insert_ino_to_reclaimed(int pid, unsigned long ino)
{
	return false;
}

static bool get_reclaimed_list(int pid, struct list_head *reclaimed_list)
{
	return false;
}

static struct mutex *lock_pid(int pid)
{
	return NULL;
}

static int add_pid_info(int pid)
{
	return -EINVAL;
}

static void set_reclaimed_pid_info_state(int pid, int status)
{
}

static int get_reclaimed_pid_info_state(int pid)
{
	return -EINVAL;
}

bool set_pid_reclaim_state(int pid)
{
	return false;
}

static bool wait_resuming(int pid)
{
	return false;
}

static int free_reclaimed_pid_info(int pid)
{
	return -EINVAL;
}

static void free_reclaimed_list(struct list_head *reclaimed_list)
{
}
#endif
#endif /* _DMABUF_RECLAIM_MANAGE_H */