/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user client headers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#ifndef __VHOST_CLIENT_H__
#define __VHOST_CLIENT_H__

#include "libvhost_user/vhost_msg.h"
#include "libvhost_user/vhost_channel.h"

#define VHOST_KICK_MMIO_SIZE	4u

struct vhost_client_s;

typedef struct vhost_channel_s *(*vhost_channel_create)(void);

struct vhost_client_s *vhost_new_client(uint32_t cnode_idx, uint32_t virtio_id,
					uint32_t device_id,
					vhost_channel_create channel_create);
void vhost_revoke_vm(struct vhost_client_s *c);
void vhost_destroy_client(struct vhost_client_s *c);

int vhost_connect(struct vhost_client_s *client, const union vhost_path_s *path);

int vhost_grant_vm(struct vhost_client_s *c, cref_t vm_cref,
		   uint64_t mmio_base, uint32_t irq, uint32_t mmio_size);

struct vhost_virtq_info_s {
	int32_t vq_index;
	uint16_t qsize;
	uint64_t desc;
	uint64_t avail;
	uint64_t used;
};
int vhost_set_vq_base(struct vhost_client_s *c, const struct vhost_virtq_info_s *info);
int vhost_set_kick_mmio(struct vhost_client_s *c, uint32_t vcpu_num,
			uint32_t irq_id, uint64_t base, uint32_t size);
int vhost_set_dev_info(struct vhost_client_s *c, const char *name);
int vhost_set_features(struct vhost_client_s *c, uint64_t features);
int vhost_all_done(struct vhost_client_s *c, int result);
int vhost_reset_dev(struct vhost_client_s *c);
int vhost_send_custom_msg(struct vhost_client_s *c, struct vmsg_custom_msg_s *msg);

#endif
