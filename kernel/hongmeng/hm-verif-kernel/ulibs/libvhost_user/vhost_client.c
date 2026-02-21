/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user client implements
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>

#include "libvhost_user/vhost_msg.h"
#include "libvhost_user/vhost_client.h"

struct vhost_client_s {
	uint32_t self_cnode_idx;

	uint32_t virtio_id;
	uint32_t device_id;

	rref_t vm_rref;
	uref_t vm_uref;

	struct vhost_channel_s *channel;
};

struct vhost_client_s *vhost_new_client(uint32_t cnode_idx, uint32_t virtio_id,
					uint32_t device_id,
					vhost_channel_create channel_create)
{
	struct vhost_client_s *c = NULL;
	int ret = E_HM_OK;
	struct vhost_channel_s *channel = NULL;

	if (channel_create == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		channel = channel_create();
		if (channel == NULL || channel->request == NULL) {
			ret = E_HM_NOMEM;
		} else if (channel->init != NULL) {
			ret = channel->init(channel);
		}
	}

	if (ret == E_HM_OK) {
		c = calloc(1, sizeof(struct vhost_client_s));
		if (c != NULL) {
			c->self_cnode_idx = cnode_idx;
			c->virtio_id = virtio_id;
			c->device_id = device_id;
			c->channel = channel;
		} else {
			ret = E_HM_NOMEM;
		}
	}

	if (ret != E_HM_OK) {
		if (channel != NULL && channel->release != NULL) {
			channel->release(channel);
		}
	}

	return c;
}

void vhost_revoke_vm(struct vhost_client_s *c)
{
	if (c != NULL) {
		if (IS_REF_VALID(c->vm_uref)) {
			(void)hm_ucap_revoke(0, c->vm_uref);
		}
		c->vm_uref = 0ULL;
	}
}

void vhost_destroy_client(struct vhost_client_s *c)
{
	if (c != NULL) {
		vhost_revoke_vm(c);

		if (c->channel != NULL && c->channel->release != NULL) {
			c->channel->release(c->channel);
		}

		free(c);
	}
}

int vhost_connect(struct vhost_client_s *client, const union vhost_path_s *path)
{
	int ret;

	if (client == NULL || client->channel == NULL ||
		client->channel->connect == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = client->channel->connect(client->channel, path);
	}

	return ret;
}

static int vhost_grant_vm_request(struct vhost_client_s *c,
				  uint32_t uvmm_cnode_idx, cref_t vm_cref,
				  rref_t *vm_rref)
{
	int ret = E_HM_OK;

	if (vm_rref == NULL || c == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct vhost_user_msg_s req = {
			.op = VHOST_OP_GRANT_VM_REQUEST,
			.virtio_id = c->virtio_id,
			.device_id = c->device_id,
			.cnode_idx = c->self_cnode_idx,
			.return_code = 0,
			.u.grant_vm_request.vm_cref = vm_cref,
			.u.grant_vm_request.uvmm_cnode_idx = uvmm_cnode_idx,
		};
		struct vhost_user_msg_s rsp = {0};

		ret = c->channel->request(c->channel, &req, &rsp);
		if (ret == E_HM_OK) {
			*vm_rref = rsp.u.grant_vm_request.vm_rref;
		}
	}

	return ret;
}

static int vhost_grant_vm_done(struct vhost_client_s *c, int32_t result)
{
	int ret;

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct vhost_user_msg_s req = {
			.op = VHOST_OP_GRANT_VM_DONE,
			.virtio_id = c->virtio_id,
			.device_id = c->device_id,
			.cnode_idx = c->self_cnode_idx,
			.return_code = 0,
			.u.grant_vm_done.result = result,
		};
		struct vhost_user_msg_s rsp = {0};

		ret = c->channel->request(c->channel, &req, &rsp);
	}

	return ret;
}

int vhost_grant_vm(struct vhost_client_s *c, cref_t vm_cref,
		   uint64_t mmio_base, uint32_t irq, uint32_t mmio_size)
{
	struct vmsg_grant_arg_s grant_arg;
	rref_t vm_rref = 0;
	int ret;

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = vhost_grant_vm_request(c, c->self_cnode_idx, vm_cref, &vm_rref);
		if (ret == E_HM_OK) {
			grant_arg.mmio_base = mmio_base;
			grant_arg.irq_id = vhost_to_spi(irq);
			grant_arg.mmio_size = mmio_size;
			uref_t vm_uref = hm_ucap_grant(0, vm_rref, 0, vm_cref, CRIGHT_VM_USER, (uintptr_t)&grant_arg);
			if (IS_REF_ERR(vm_uref)) {
				ret = REF_TO_ERR(vm_uref);
			} else {
				c->vm_uref = vm_uref;
				c->vm_rref = vm_rref;
			}

			ret = vhost_grant_vm_done(c, ret);
		}
	}

	return ret;
}

int vhost_set_vq_base(struct vhost_client_s *c, const struct vhost_virtq_info_s *info)
{
	struct vhost_user_msg_s rsp = {0};
	int ret;

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct vhost_user_msg_s req = {
			.op = VHOST_OP_SET_VQ_BASE,
			.virtio_id = c->virtio_id,
			.device_id = c->device_id,
			.cnode_idx = c->self_cnode_idx,
			.return_code = 0,
			.u.set_vq_base.vq_index = info->vq_index,
			.u.set_vq_base.desc = info->desc,
			.u.set_vq_base.avail = info->avail,
			.u.set_vq_base.used = info->used,
			.u.set_vq_base.qsize = info->qsize,
		};

		ret = c->channel->request(c->channel, &req, &rsp);
	}

	return (ret == E_HM_OK) ? rsp.return_code : ret;
}

int vhost_set_kick_mmio(struct vhost_client_s *c, uint32_t vcpu_num,
			uint32_t irq_id, uint64_t base, uint32_t size)
{
	struct vhost_user_msg_s rsp = {0};
	int ret;

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct vhost_user_msg_s req = {
			.op = VHOST_OP_SET_KICK_MMIO,
			.virtio_id = c->virtio_id,
			.device_id = c->device_id,
			.cnode_idx = c->self_cnode_idx,
			.return_code = 0,
			.u.set_kick_mmio.irq_id = irq_id,
			.u.set_kick_mmio.vcpu_num = vcpu_num,
			.u.set_kick_mmio.mmio_base = base,
			.u.set_kick_mmio.mmio_size = size,
		};

		ret = c->channel->request(c->channel, &req, &rsp);
	}

	return (ret == E_HM_OK) ? rsp.return_code : ret;
}

int vhost_set_dev_info(struct vhost_client_s *c, const char *name)
{
	int ret;
	struct vhost_user_msg_s rsp = {0};
	struct vhost_user_msg_s req = {0};

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		req.op = VHOST_OP_SET_DEV_INFO;
		req.virtio_id = c->virtio_id;
		req.device_id = c->device_id;
		req.cnode_idx = c->self_cnode_idx;
		req.return_code = 0;

		ret = strncpy_s(req.u.set_dev_info.name, VHOST_NAME_LEN, name, VHOST_NAME_LEN - 1);
		if (ret != EOK) {
			ret = E_HM_INVAL;
		} else {
			ret = c->channel->request(c->channel, &req, &rsp);
		}
	}

	return (ret == E_HM_OK) ? rsp.return_code : ret;
}

int vhost_set_features(struct vhost_client_s *c, uint64_t features)
{
	int ret;
	struct vhost_user_msg_s rsp = {0};
	struct vhost_user_msg_s req = {0};

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		req.op = VHOST_OP_SET_FEATURES;
		req.virtio_id = c->virtio_id;
		req.device_id = c->device_id;
		req.cnode_idx = c->self_cnode_idx;
		req.return_code = 0;
		req.u.set_features.features = features;

		ret = c->channel->request(c->channel, &req, &rsp);
	}

	return (ret == E_HM_OK) ? rsp.return_code : ret;
}

int vhost_reset_dev(struct vhost_client_s *c)
{
	struct vhost_user_msg_s rsp = {0};
	struct vhost_user_msg_s req = {0};
	int ret;

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		req.op = VHOST_OP_RESET_DEV;
		req.virtio_id = c->virtio_id;
		req.device_id = c->device_id;
		req.cnode_idx = c->self_cnode_idx;
		req.return_code = 0;

		ret = c->channel->request(c->channel, &req, &rsp);
	}

	return (ret == E_HM_OK) ? rsp.return_code : ret;
}

int vhost_send_custom_msg(struct vhost_client_s *c, struct vmsg_custom_msg_s *msg)
{
	struct vhost_user_msg_s rsp = {0};
	struct vhost_user_msg_s req = {0};
	int ret;

	req.op = VHOST_OP_CUSTOM_MSG;
	ret = memcpy_s(&req.u.custom_msg, sizeof(req.u.custom_msg), msg, sizeof(*msg));
	if (ret != EOK) {
		ret = E_HM_POSIX_FAULT;
	}

	if (ret == EOK) {
		ret = c->channel->request(c->channel, &req, &rsp);
		ret = (ret == E_HM_OK) ? rsp.return_code : ret;
	}

	return ret;
}

int vhost_all_done(struct vhost_client_s *c, int result)
{
	struct vhost_user_msg_s rsp = {0};
	int ret;

	if (c == NULL) {
		ret = E_HM_INVAL;
	} else {
		struct vhost_user_msg_s req = {
			.op = VHOST_OP_ALL_DONE,
			.virtio_id = c->virtio_id,
			.device_id = c->device_id,
			.cnode_idx = c->self_cnode_idx,
			.return_code = 0,
			.u.all_done.result = result,
		};

		ret = c->channel->request(c->channel, &req, &rsp);
	}

	return (ret == E_HM_OK) ? rsp.return_code : ret;
}
