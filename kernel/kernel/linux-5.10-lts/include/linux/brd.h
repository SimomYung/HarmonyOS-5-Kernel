/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BRD_H
#define _LINUX_BRD_H

#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/mutex.h>
#include <linux/radix-tree.h>
#include <linux/fs.h>

/*
 * Each block ramdisk device has a radix_tree brd_pages of pages that stores
 * the pages containing the block device's contents. A brd page's ->index is
 * its offset in PAGE_SIZE units. This is similar to, but in no way connected
 * with, the kernel's pagecache or buffer cache (which sit above our block
 * device).
 */
struct brd_device {
	int		brd_number;
 
	struct request_queue	*brd_queue;
	struct gendisk		*brd_disk;
	struct list_head	brd_list;
 
	/*
	 * Backing store of pages and lock to protect it. This is the contents
	 * of the block device.
	 */
	spinlock_t		brd_lock;
	struct radix_tree_root	brd_pages;
};

struct brd_device *brd_alloc(int i);

#endif 

