/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header file for shm_ring_buffer lib.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 10:24:52 2020
 */
#ifndef ULIBS_LIBRB_SHM_RING_BUFFER_H
#define ULIBS_LIBRB_SHM_RING_BUFFER_H

#include <stdbool.h>

#include <hongmeng/types.h>
#include <libhmsync/raw_rwlock.h>

#define RING_ONWORKING			1
#define RING_IDLE			0

#define SRING_BUF_CNT_MAX	128

/*
 * sring_buf:	menagement struct of a shared memory ring buffer
 *
 * It is not shared. It could be used in each process only.
 * Do not store head/tail in here. They should be in shared memory, which could
 * be seen from both sides.
 */
struct sring_buf {
	int ring_fd;		/* fd of item buffer, including sring_info */
	void *ring_vaddr;	/* vaddr of item buffer mapped into a vspace */
	size_t ring_size;	/* Total size of item buffer,
				 * sizeof(sring_info) +
				 * sring_info->item_cnt * sizeof(sring_item) */

	int data_fd;		/* fd of obj buffer */
	void *data_vaddr;	/* obj buffer vaddr mapped into a vspace */
	size_t data_size;	/* Total size of obj buffer,
				 * srinfo->obj_size * srinfo->obj_cnt */
	struct raw_rwlock rwlock;
};

struct sring_buf *sring_buf_create(size_t item_cnt);
struct sring_buf *sring_buf_create_from_fd(uint64_t ring_fd, bool include_data,
					   uint32_t cnode_idx);
int sring_buf_install_data(struct sring_buf *srbuf, size_t obj_cnt,
			   size_t obj_size);
void sring_buf_share_data(struct sring_buf *srbuf_dst,
			  struct sring_buf *srbuf_src);
void sring_buf_init_info(struct sring_buf *srbuf, size_t item_cnt,
			 size_t obj_cnt, size_t obj_size);
void sring_buf_destroy(const struct sring_buf *srbuf, bool include_data,
		       bool unlink);
int sring_buf_connect(struct sring_buf *srbuf, bool map_data);
void sring_buf_disconnect(struct sring_buf *srbuf, bool include_data);
int sring_buf_push(struct sring_buf *srbuf, const void *obj);
void *sring_buf_pop(struct sring_buf *srbuf);
bool sring_is_onwork(struct sring_buf *srbuf);
void sring_set_state(struct sring_buf *srbuf, int state);
void sring_buf_set_info_data_fd(struct sring_buf *srbuf, uint64_t data_fd);

/*
 * The following two interfaces add regrant shm flag to the share memory
 */
struct sring_buf *sring_buf_regrant_create(size_t item_cnt);
int sring_buf_regrant_install_data(struct sring_buf *srbuf, size_t obj_cnt,
				   size_t obj_size);

#endif
