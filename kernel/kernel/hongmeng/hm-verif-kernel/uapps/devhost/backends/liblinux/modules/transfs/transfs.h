/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of trans module interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 1 16:28:08 2021
 */

#ifndef __LIBLINUX_TRANSFS_H__
#define __LIBLINUX_TRANSFS_H__

#include <linux/fs.h>
#include <linux/version.h>

#define MAX_FS_NAME_LEN 16
#define	SHRINK_SIZE_ALL	(-1)

struct trans_mount_info {
	char mnt_opts[256];
	struct block_device *bdev;
	void *data;
};

#define DIRENT_DNAME_MAX_LEN 256
struct transfs_dirent {
	uint64_t d_ino;
	uint64_t d_off;
	unsigned char d_type;
	size_t d_name_len;
	char d_name[DIRENT_DNAME_MAX_LEN];
};

struct linux_dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char		d_name[1];
};

struct getdents_callback {
	struct dir_context ctx;
	struct linux_dirent __user * current_dir;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,0)
	int prev_reclen;
#else
	struct linux_dirent __user * previous;
#endif
	int count;
	int error;
};

struct trans_node_meta {
	unsigned long dev_id;
	uint32_t index;
	unsigned lnkcnt;
	uid_t uid;
	gid_t gid;
	unsigned mode;
	unsigned flags;
	unsigned long devno;
	uint64_t size;
	unsigned long blksize;
	long blocks;
	int64_t a_tv_sec;
	long a_tv_nsec;
	int64_t c_tv_sec;
	long c_tv_nsec;
	int64_t m_tv_sec;
	long m_tv_nsec;
};

struct transfs_attr_t {
	unsigned long long blocks;
	unsigned long blksize;
};

#if LINUX_VERSION_CODE != KERNEL_VERSION(4,14,61)
extern void transfs_drop_caches(unsigned int mode);
void liblinux_enable_page_cache(struct page *(*pagecache_alloc_fn)(gfp_t gfp_mask, unsigned order));
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
extern struct file *alloc_file(const struct path *path, int flags, const struct file_operations *fop);
extern unsigned long kallsyms_lookup_name(const char *name);
#endif

#define DEFAULT_PAGE_CLEAN_MAX_NR	(~0UL)
#define DEFAULT_PAGE_SYNC_MAX_NR	0

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
#define liblinux_get_pagecache_nr() global_node_page_state(NR_FILE_PAGES)
#else
#define liblinux_get_pagecache_nr() global_page_state(NR_FILE_PAGES)
#endif

#endif /* __LIBLINUX_TRANSFS_H__ */
