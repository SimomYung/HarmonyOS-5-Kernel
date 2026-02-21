/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:30:00 2022
 */

#ifndef __DEVHOST_NET_SOCKET_H__
#define __DEVHOST_NET_SOCKET_H__

#include <linux/net.h>
#include <devhost/memory.h>

struct devhost_socket {
	struct socket *sock;
	int fd;
	int file_flags;
	struct file *file;
	void *recv_queue;
};

enum {
	RECV_QUEUE_KERNEL_DONE_BIT = 0,
	RECV_QUEUE_READ_BIT,
	RECV_QUEUE_WRITE_BIT,
};

#define RECV_QUEUE_FLAG_KERNEL_DONE_BIT (1 << RECV_QUEUE_KERNEL_DONE_BIT)
#define RECV_QUEUE_FLAG_READ_BIT (1 << RECV_QUEUE_READ_BIT)
#define RECV_QUEUE_FLAG_WRITE_BIT (1 << RECV_QUEUE_WRITE_BIT)

#define RECV_QUEUE_BATCH_CNT (20U)
#define RECV_QUEUE_PAGE_CNT ((RECV_QUEUE_BATCH_CNT >> 1) + 1)
#define RECV_QUEUE_CONTROL_SIZE (32U)
#define RECV_QUEUE_IOV_SIZE (1470U)
#define RECV_QUEUE_META_OFFSET (PAGE_SIZE >> 1)
#define RECV_QUEUE_IOV_OFFSET (PAGE_SIZE >> 1)
#define RECV_QUEUE_CONTROL_ADDR(meta, offset) ((meta)->msg[offset].control)
#define RECV_QUEUE_META_ADDR(data) \
	((struct recv_queue_meta *)((char *)(data) + RECV_QUEUE_META_OFFSET))
#define RECV_QUEUE_IOV_ADDR(data, offset) \
	(void *)((char *)(data) + PAGE_SIZE + (offset) * RECV_QUEUE_IOV_OFFSET)
/*
 * recv queue layout(consistent with kernel):
 *    page 0: struct recv_queue_control
 *                |
 *      RECV_QUEUE_META_OFFSET
 *                |
 *            struct recv_queue_meta
 *               ...
 *    page 1:   iov[0]
 *                |
 *      RECV_QUEUE_IOV_OFFSET
 *                |
 *              iov[1]
 *    page 2:   iov[2]
 *               ...
*/
struct recv_queue_meta {
	int flags;
	int count;
	int head;
	int tail;

	struct {
		char control[RECV_QUEUE_CONTROL_SIZE];
	} msg[RECV_QUEUE_BATCH_CNT];
};

static inline void meta_consumer_pop(struct recv_queue_meta *meta)
{
	meta->count--;
	meta->head++;
	meta->head = meta->head % RECV_QUEUE_BATCH_CNT;
}

static inline void meta_producer_push(struct recv_queue_meta *meta)
{
	meta->count++;
	meta->tail++;
	meta->tail = meta->tail % RECV_QUEUE_BATCH_CNT;
}

struct device_info;

extern int devhost_net_fops_poll(void *ctx, void *pinfo, unsigned int *revents);
extern int devhost_net_fops_mmap(void *ctx, struct devhost_vma_info *vma_info);
extern int liblinux_pal_is_compat_client(void);
extern int liblinux_pal_copy_msghdr_from_user(void *dst, void *src);
extern int liblinux_pal_copy_msghdr_to_user(void *dst, void *src, void *uaddr, void *ucmsg);

#endif /* __DEVHOST_NET_SOCKET_H__ */
