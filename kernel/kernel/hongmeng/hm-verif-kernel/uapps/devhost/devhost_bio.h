/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for block device support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 26 15:01:02 2019
 */

#ifndef __DEVHOST_BIO_H__
#define __DEVHOST_BIO_H__

#include <devhost/device.h>
#include <lib/dlist.h>
#include <devhost/file.h>
#include <libsysif/devhost/api.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_io/blk.h>
#include <libhmsrv_sys/hm_timer.h>

struct bd_shm_info {
	size_t len;
	size_t bd_shm_size;
	unsigned long long *paddrs;
	void *vaddr;
	struct devhost_vma_info *vma_info;
	void *handle; /* store memory block information */
};

/* queue to process struct bd_io from one vfs */
struct bd_vfs_queue {
	struct filp_node *node; /* filp related to this queue */
	struct bd_queue bd_queue;
	/* shm to store data in bio */
	struct bd_shm_info shm_info;
	size_t obj_cnt;		/* len of the data */
	size_t item_cnt;	/* len of the ring */
	struct dlist_node list;
};

/* bd queue info in each device_info */
struct bd_queue_info {
	struct raw_rwlock bd_vfs_queue_rwlock;
	struct dlist_node bd_vfs_queue_list;
};

struct bd_grant_ret_s {
	uint64_t shm_id;
	uint64_t submit_fd;
	uint64_t end_fd;
};

static inline void devhost_update_bio_time(struct bd_time *bd_time)
{
	struct timespec now;
	int ret;

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &now);
	if (ret == E_HM_OK) {
		bd_time->tv_sec = now.tv_sec;
		bd_time->tv_nsec = now.tv_nsec;
	} else {
		bd_time->tv_sec = 0;
		bd_time->tv_nsec = 0;
	}
}

int devhost_submit_bio(struct filp_node *node, unsigned long long io_rw,
		       unsigned long long sector, struct hmsrv_io_blk_vec *vec,
		       int vec_cnt, unsigned int priority,
		       void *bi_crypt_context);
int devhost_submit_cmd(struct filp_node *node, unsigned long long io_cmd,
		       unsigned long long sector, unsigned int size,
		       unsigned int priority);

int devhost_do_async_submit_bio(struct filp_node *node);
int devhost_do_sync_submit_bio(struct filp_node *node, unsigned int idx);
int devhost_init_bio(struct filp_node *node,
		     struct bd_vfs_queue **bd_vfs_queue,
		     size_t bd_shm_size);
int devhost_grant_bio(struct bd_vfs_queue *vfs_queue, struct bd_grant_ret_s *ret);
void devhost_destroy_bio(struct filp_node *node, bool success);

#endif /* __DEVHOST_BIO_H__ */
