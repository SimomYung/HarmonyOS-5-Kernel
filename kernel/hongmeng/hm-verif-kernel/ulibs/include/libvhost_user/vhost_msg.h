/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user msg headers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#ifndef __VHOST_MSG_H__
#define __VHOST_MSG_H__

#include <stdint.h>

#include <libhmactv/actv.h>

#define VHOST_NAME_LEN	16

enum {
	VHOST_OP_GRANT_VM_REQUEST,
	VHOST_OP_GRANT_VM_DONE,

	VHOST_OP_SET_VQ_BASE,
	VHOST_OP_SET_KICK_MMIO,

	VHOST_OP_SET_DEV_INFO,
	VHOST_OP_SET_FEATURES,

	VHOST_OP_CUSTOM_MSG,

	VHOST_OP_ALL_DONE,
	VHOST_OP_RESET_DEV,
};

struct vmsg_grant_arg_s {
	uint64_t mmio_base;
	uint32_t irq_id;
	uint32_t mmio_size;
};

struct vmsg_grant_vm_request_s {
	uint32_t uvmm_cnode_idx;
	cref_t vm_cref;

	rref_t vm_rref;
};

struct vmsg_grant_vm_done_s {
	int32_t result;
};

struct vmsg_set_vq_base_s {
	int32_t vq_index;
	uint16_t qsize;

	uint64_t desc;
	uint64_t avail;
	uint64_t used;
};

struct vmsg_set_kick_mmio_s {
	uint32_t vcpu_num;
	uint32_t irq_id;
	uint64_t mmio_base;
	uint32_t mmio_size;
};

struct vmsg_set_features_s {
	uint64_t features;
};

struct vmsg_set_dev_info_s {
	char name[VHOST_NAME_LEN];
};

struct vmsg_all_done_s {
	int result;
};

struct vmsg_custom_msg_s {
	unsigned int cmd;
	unsigned long arg;
};

typedef union {
	struct vmsg_grant_vm_request_s grant_vm_request;
	struct vmsg_grant_vm_done_s grant_vm_done;

	struct vmsg_set_vq_base_s set_vq_base;
	struct vmsg_set_kick_mmio_s set_kick_mmio;

	struct vmsg_set_dev_info_s set_dev_info;
	struct vmsg_set_features_s set_features;
	struct vmsg_custom_msg_s custom_msg;

	struct vmsg_all_done_s all_done;
} vmsg_u;

struct vhost_user_msg_s {
	uint32_t virtio_id;
	uint32_t device_id;
	int32_t return_code;
	uint32_t cnode_idx;

	uint16_t op;

	vmsg_u u;
};

#define vhost_to_spi(num) ((num) + 32U)

#endif
