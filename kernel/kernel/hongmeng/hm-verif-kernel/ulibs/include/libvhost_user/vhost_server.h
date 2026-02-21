/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user server headers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#ifndef __VHOST_SERVER_H__
#define __VHOST_SERVER_H__

#include <stdint.h>

#include <libhmsync/raw_sem.h>
#include <libhmsrv_sys/hm_vm.h>
#include <libhmsync/raw_thread.h>
#include <libvhost_user/vhost_msg.h>
#include <libhmsync/raw_scopedptr.h>
#include <libvhost_user/vhost_virtqueue.h>

#define VHOST_SERVER_MAX_DEVICE_NUM	8u
#define VCPU_NUM_MAX 255

/**
 * @desc represent a vm memory region
 * @hva: host virtual address
 * @gpa: guest physical address
 * @size: size of this region
 */
struct vhost_region_s {
	uint64_t hva;
	uint64_t gpa;
	uint64_t size;
};

/**
 * @desc a vm may have multi memory regions
 */
struct vhost_regions_s {
	struct vhost_region_s *regions;
	unsigned int region_num;
};

/**
 * @desc represent a VM in vhost process, for VM's memory will
 * 	 be map into address space of vhost process, so this struct
 * 	 manager the memory regions, and guarantee each vm will be mapped
 * 	 only once in vhost process, following maps operations will get
 * 	 reference to this struct.
 * 	 each vm is identified by (cnode_idx, vm_cref).
 */
struct vhost_vm_s {
	uint32_t cnode_idx;
	uint32_t pid;
	cref_t vm_cref;
	struct raw_refcnt refcnt;

	struct vhost_regions_s regions;
};

struct vhost_backend_s;

/**
 * @desc each vhost_device_s repesent a virtio device
 * 	 E.g vsock device, virtio-net device.
 * 	 each device has a @device_id for each virtio-type may
 * 	 has multi device, E.g a vm may start multi virtio-net device.
 * 	 vhost_device_s has it's own @vqs and @irq_id
 * 	 each successfully probed device will start a thread to recv
 * 	 virtq notification from guest.
 */

struct vhost_device_s {
	uint32_t device_id;
	char dev_name[VHOST_NAME_LEN];

	struct virtqueue_s *vqs;
	uint32_t vq_num;

	uint32_t vcpu_num;

	uint32_t irq_id;
	int mregion_id;

	uint32_t pid;
	cref_t vm_cref;
	rref_t vm_rref;

	struct vhost_vm_s *vm;
	struct actv *actv;

	struct vhost_backend_s *backend;

	bool running;
	raw_thread_t thread;
	struct raw_sem sem;

	void *priv;
};

/**
 * @desc a backend hook provides a hook for real vhost backend implements,
 * 	 E.g vsock and virtio-net will have there backend hook respectively,
 * 	 when vhost backend probe, hook should be registered by @vhost_register_hook,
 * 	 and initially set the @virtio_id, @vq_num leaves other field empty which will
 * 	 be filled during vhost client and server interactive.
 */
struct vhost_backend_s {
	uint32_t virtio_id;
	uint32_t vq_num;
	uint64_t features;

	struct vhost_device_s *devices[VHOST_SERVER_MAX_DEVICE_NUM];

	int (*handle_vq_ready)(int32_t vq_index, struct vhost_device_s *d,
			       struct virtqueue_s *vq);
	int (*device_done)(struct vhost_backend_s *b, struct vhost_device_s *d);
	void (*device_destroy)(struct vhost_backend_s *b, struct vhost_device_s *d);
};

struct vhost_server_s;

struct vhost_server_s *vhost_new_server(void);
struct vhost_server_s *vhost_new_server_actv(actv_init_func_t fn);
void vhost_destroy_server(struct vhost_server_s *server);

int vhost_register_backend(struct vhost_server_s *server, struct vhost_backend_s *hook);

int vhost_server_handle_msg(struct vhost_server_s *server, uint32_t caller_cnode,
			    const struct vhost_user_msg_s *req, struct vhost_user_msg_s *rsp);
int vhost_server_revoke(struct vhost_server_s *server, uint32_t cnode_idx);

int vhost_convert_gpa_to_hva(const struct vhost_device_s *device,
			     uint64_t gpa, uint64_t len, uint64_t *hva);
int vhost_send_irq(const struct vhost_device_s *device);
int vhost_device_revoke(struct vhost_server_s *server,
			struct vhost_device_s *device);
void vhost_device_dump(struct vhost_server_s *server,
		       const struct vhost_device_s *device);
#endif
