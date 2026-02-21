/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Vhost user server implements
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#include "libvhost_user/vhost_server.h"

#include <hmasm/barrier.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/syscall.h>
#include <hmkernel/drivers/hypervisor.h>
#include <libhmactv/actv.h>
#include <hmkernel/activation/local_area.h>
#include <hmasm/activation.h>
#include <libsysif/utils.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_hmsignal.h>

#include "libvhost_user/vhost_msg.h"

#define VHOST_SERVER_MAX_HOOK_NR	64U
#define VHOST_SERVER_MAX_VM_NUM		8U

static void delete_actvs(struct vhost_device_s *device, unsigned int end)
{
	int rc;
	unsigned int i;
	if (device->actv != NULL) {
		for (i = 0u; i < end; i++) {
			rc = hm_actv_destroy(&device->actv[i]);
			mem_zero_s(device->actv[i]);

			if (rc != E_HM_OK) {
				hm_warn("vhost-server delete actv %u failed: %s\n",
					i, hmstrerror(rc));
			}
		}

		free(device->actv);
		device->actv = NULL;
	}
}

static void do_actv_destroy(struct vhost_device_s *device)
{
	delete_actvs(device, device->vcpu_num);
}

#ifndef __HOST_LLT__
DECLARE_UPCALL_ACTVHDLR(actv_handle_vhost);

DEFINE_UPCALL_ACTVHDLR(actv_handle_vhost)
{
	struct vhost_device_s *device = (struct vhost_device_s *)args.hyp_mmio_dispatcher.opaque;
	__u64 data = args.hyp_mmio_dispatcher.data;

	if (data < device->vq_num) {
		struct virtqueue_s *vq = &device->vqs[data];
		if (vq->handle_kick != NULL) {
			vq->handle_kick(vq, vq->opaque);
		}
	}

	return __ACTV_WFERET_HYP_VMEXIT_OK;
}

static const void *actv_action_table[__ACTV_KERNMAX] = {
	[__ACTV_UPCALL_HYP_MMIO_DISPATCHER] = actv_handle_vhost
};

#endif
#define ACTV_STACK_SIZE		(4096 * 4)

struct vhost_server_s {
	struct vhost_vm_s *vms[VHOST_SERVER_MAX_VM_NUM];
	struct vhost_backend_s *hooks[VHOST_SERVER_MAX_HOOK_NR];
	struct raw_mutex lock;

	struct actv_bind_ret_rref sysmgr_bind_ret;
	cref_t vhost_actv_pool_cref;
	bool vhost_actv_inited;
	bool vhost_actv_mode;
	actv_init_func_t actv_init;
};

static int do_actv_create(struct vhost_device_s *device,
			  struct vhost_server_s *server)
{
#ifndef __HOST_LLT__
	int err;

	device->actv = malloc(sizeof(struct actv) * device->vcpu_num);
	if (device->actv == NULL) {
		return E_HM_NOMEM;
	}

	for (unsigned int i = 0; i < device->vcpu_num; i++) {
		struct actv *actv = &device->actv[i];
		struct actv_attr attr;

		mem_zero_s(*actv);
		mem_zero_s(attr);
		attr.actv_pool    = server->vhost_actv_pool_cref;
		attr.stack_size   = ACTV_STACK_SIZE;
		attr.action_table = actv_action_table;
		attr.action_table_sz = sizeof(actv_action_table);
		attr.actv_init = server->actv_init;

		err = hm_actv_create(actv, &attr);
		if (err != E_HM_OK) {
			delete_actvs(device, i);
			hm_warn("vhost create actv failed: %s\n",
				hmstrerror(err));
			return err;
		}
		hm_debug("vhost_server vcpu_%u activation create success\n", i);

		err = syscap_ActivationReady(actv->obj.cref);
		if (err < 0) {
			delete_actvs(device, i + 1);
			hm_warn("vhost ready actv failed\n");
			return E_HM_RESFAULT;
		}
		hm_debug("vhost_server vcpu_%u ready activation success\n", i);
	}
#endif
	return E_HM_OK;
}

static void do_actvpool_delete(struct vhost_server_s *server)
{
	int err = hm_ucap_delete_actvpool(server->vhost_actv_pool_cref);
	if (err != E_HM_OK) {
		hm_warn("vhost failed to delete actvpool, err=%s\n",
			hmstrerror(err));
	}
	server->vhost_actv_pool_cref = 0U;
}

static int vhost_setup_actv_pool(struct vhost_server_s *server)
{
	int err;

	server->vhost_actv_pool_cref = hm_ucap_new_actvpool("vhost_actv");
	if (IS_REF_ERR(server->vhost_actv_pool_cref)) {
		err = REF_TO_ERR(server->vhost_actv_pool_cref);
		hm_error("Create vm activaton pool failed: %s\n", hmstrerror(err));
		return err;
	}

	err = hm_actv_pool_bind_sysmgr(server->vhost_actv_pool_cref,
				       0, &server->sysmgr_bind_ret);
	if (err != E_HM_OK) {
		hm_error("Failed to bind activation pool to sysmgr: %s\n",
			 hmstrerror(err));
		do_actvpool_delete(server);
		return err;
	}

	server->vhost_actv_inited = __true;
	return E_HM_OK;
}

static void lock_server(struct vhost_server_s *server)
{
	raw_mutex_lock(&server->lock);
}

static void unlock_server(struct vhost_server_s *server)
{
	raw_mutex_unlock(&server->lock);
}

struct vhost_server_s *vhost_new_server(void)
{
	struct vhost_server_s *server = calloc(1, sizeof(struct vhost_server_s));

	if (server != NULL) {
		server->vhost_actv_mode = __false;
		raw_mutex_init(&server->lock);
	}

	return server;
}

struct vhost_server_s *vhost_new_server_actv(actv_init_func_t fn)
{
	struct vhost_server_s *server = calloc(1, sizeof(struct vhost_server_s));
	int err;

	if (server != NULL) {
		raw_mutex_init(&server->lock);
		server->vhost_actv_mode = __true;
		server->actv_init = fn;
		err = vhost_setup_actv_pool(server);
		if (err != E_HM_OK) {
			free(server);
			server = NULL;
		}
	}

	return server;
}

static void vhost_destroy_vm(struct vhost_vm_s *vm)
{
	unsigned int i;

	for (i = 0; i < vm->regions.region_num; ++i) {
		(void)hm_mem_munmap(u64_to_ptr(vm->regions.regions[i].hva, void),
				    (size_t)vm->regions.regions[i].size);
	}

	free(vm->regions.regions);
	free(vm);
}

static struct vhost_vm_s *__get_vhost_vm(struct vhost_server_s *server,
					 const struct vhost_device_s *device)
{
	unsigned int i;
	struct vhost_vm_s *vm = NULL;

	for (i = 0; i < VHOST_SERVER_MAX_VM_NUM; ++i) {
		if ((server->vms[i] != NULL) &&
		    (server->vms[i]->cnode_idx == device->cnode_idx) &&
		    (server->vms[i]->vm_cref == device->vm_cref)) {
			vm = server->vms[i];
			(void)raw_refcnt_get(&vm->refcnt);
			break;
		}
	}

	return vm;
}

static void __put_vhost_vm(struct vhost_server_s *server,
			   struct vhost_vm_s *vm)
{
	unsigned int i;

	for (i = 0; i < VHOST_SERVER_MAX_VM_NUM; ++i) {
		if ((server->vms[i] != NULL) && (server->vms[i] == vm)) {
			if (raw_refcnt_put(&vm->refcnt) == 0) {
				vhost_destroy_vm(vm);
				server->vms[i] = NULL;
			}
			break;
		}
	}
}

static int vhost_destroy_thread(struct vhost_device_s *device)
{
	int ret = E_HM_OK;

	if (__atomic_load_n(&device->running, __ATOMIC_RELAXED)) {
		__atomic_clear(&device->running, __ATOMIC_RELAXED);
		ret = hm_tkill(raw_thread_tid_of(device->thread), SIGUSR1);
		if (ret == E_HM_OK) {
			ret = raw_thread_join(device->thread, NULL);
		}
	}

	return ret;
}

static void vhost_destroy_device(struct vhost_server_s *server,
				 struct vhost_backend_s *backend,
				 struct vhost_device_s *device)
{
	int ret;

	if (IS_REF_VALID(device->vm_rref)) {
		ret = hm_ucap_reject(0, device->vm_rref);
		if (ret < 0) {
			hm_warn("vhost reject rref failed ret = %s\n",
				hmstrerror(ret));
		}
	}

	if (server->vhost_actv_mode == __true) {
		do_actv_destroy(device);
	} else {
		ret = vhost_destroy_thread(device);
		if (ret != E_HM_OK) {
			hm_warn("vhost destroy listener thread failed ret = %s\n",
				hmstrerror(ret));
		}
	}

	if (backend->device_destroy != NULL) {
		backend->device_destroy(backend, device);
	}

	__put_vhost_vm(server, device->vm);

	free(device);
}

static void vhost_release_device(struct vhost_server_s *server,
				 struct vhost_device_s *device)
{
	uint32_t i;
	struct vhost_backend_s *h = NULL;

	h = device->backend;
	for (i = 0; i < VHOST_SERVER_MAX_DEVICE_NUM; i++) {
		if (h->devices[i] == device) {
			vhost_destroy_device(server, h, device);
			h->devices[i] = NULL;
			break;
		}
	}
}

static void vhost_destroy_devices(struct vhost_server_s *server,
				  struct vhost_backend_s *hook, uint32_t cnode_idx)
{
	if (hook != NULL) {
		unsigned int j;

		for (j = 0; j < VHOST_SERVER_MAX_DEVICE_NUM; ++j) {
			struct vhost_device_s *device = hook->devices[j];

			if (device == NULL) {
				continue;
			}

			if (cnode_idx != 0 && cnode_idx != device->cnode_idx) {
				continue;
			}

			vhost_destroy_device(server, hook, hook->devices[j]);
			hook->devices[j] = NULL;
		}
	}
}

void vhost_destroy_server(struct vhost_server_s *server)
{
	if (server != NULL) {
		unsigned int i;

		lock_server(server);

		for (i = 0; i < VHOST_SERVER_MAX_HOOK_NR; ++i) {
			vhost_destroy_devices(server, server->hooks[i], 0);
		}

		unlock_server(server);
		free(server);
	}
}

static struct vhost_backend_s *get_backend_hook(struct vhost_server_s *server,
						uint32_t virtio_id)
{
	struct vhost_backend_s *hook = NULL;

	if (server != NULL && virtio_id < VHOST_SERVER_MAX_HOOK_NR) {
		hook = server->hooks[virtio_id];
	}

	return hook;
}

int vhost_register_backend(struct vhost_server_s *server,
			   struct vhost_backend_s *hook)
{
	int ret = E_HM_OK;

	if (hook == NULL || hook->virtio_id >= VHOST_SERVER_MAX_HOOK_NR) {
		ret = E_HM_INVAL;
	} else {
		lock_server(server);

		if (server->hooks[hook->virtio_id] != NULL) {
			ret = E_HM_OBJEXIST;
		} else {
			server->hooks[hook->virtio_id] = hook;
		}

		unlock_server(server);
	}

	return ret;
}

static void vhost_bind_vhost_dispatcher(const struct vhost_device_s *device,
				       struct vhost_server_s *server)
{
	for (uint32_t vcpu_idx = 0; vcpu_idx < device->vcpu_num; ++vcpu_idx) {
		int err = syscap_VMBindDispatcherActvpool(device->vm_rref,
						     vcpu_idx,
						     (unsigned int)device->mregion_id,
						     server->vhost_actv_pool_cref,
						     ptr_to_u64(device));
		if (err != E_HM_OK) {
			hm_warn("bind vcpu mmio dispatcher failed ret = %s\n",
				hmstrerror(err));
		} else {
			hm_debug("bind vcpu_%u to vhost actv_pool\n", vcpu_idx);
		}
	}
}

static struct vhost_device_s *get_or_alloc_vhost_device(struct vhost_backend_s *b,
							uint32_t cnode_idx,
							uint32_t dev_id)
{
	struct vhost_device_s *device = NULL;
	int slot = -1;
	unsigned int i;

	if (b == NULL) {
		return NULL;
	}

	for (i = 0; i < VHOST_SERVER_MAX_DEVICE_NUM; ++i) {
		if (b->devices[i] == NULL) {
			if (slot == -1) {
				slot = (int)i;
			}
		} else if (b->devices[i]->device_id == dev_id &&
				b->devices[i]->cnode_idx == cnode_idx) {
			device = b->devices[i];
			break;
		}
	}

	if (device == NULL && slot != -1) {
		size_t size = sizeof(struct vhost_device_s) +
				b->vq_num * sizeof(struct virtqueue_s);
		device = calloc(1, size);
		if (device != NULL) {
			device->device_id = dev_id;
			device->cnode_idx = cnode_idx;
			device->vq_num = b->vq_num;
			device->backend = b;
			device->vqs = (struct virtqueue_s *)(device + 1);
			b->devices[slot] = device;
			(void)raw_sem_init(&device->sem, 0);
		}
	}

	return device;
}

static int __vhost_map_regions(rref_t vm_rref, struct vhost_regions_s *regions)
{
	struct vm_map_to_process_args args = { 0 };
	int ret;
	size_t size = 0;
	unsigned int i;
	xref_t vm_xref = { .rref = vm_rref };

	args.vm_rref = vm_rref;
	ret = hm_vm_get_region_num(vm_xref, &args.region_num, VM_REGION_TYPE_VHOST);
	if (ret == E_HM_OK && args.region_num == 0) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		size = sizeof(struct vm_region_s) * args.region_num;
		args.regions = malloc(size);
		if (args.regions == NULL) {
			ret = E_HM_NOMEM;
		}

		regions->regions = malloc(sizeof(struct vhost_region_s) * args.region_num);
		if (ret == E_HM_OK && regions->regions == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		mem_zero_p(args.regions, size, size);
		ret = hm_vm_map_to_process(&args);
	}

	if (ret == E_HM_OK) {
		regions->region_num = args.region_num;
		for (i = 0; i < regions->region_num; ++i) {
			regions->regions[i].gpa = args.regions[i].gpa;
			regions->regions[i].hva = args.regions[i].hva;
			regions->regions[i].size = args.regions[i].size;
		}
	} else if (regions->regions != NULL) {
		free(regions->regions);
	}

	if (args.regions != NULL) {
		free(args.regions);
	}
	return ret;
}

static struct vhost_vm_s *__alloc_vhost_vm(struct vhost_server_s *server,
					   const struct vhost_device_s *device)
{
	struct vhost_vm_s *vm = NULL;
	int slot = -1;
	unsigned int i;

	for (i = 0; i < VHOST_SERVER_MAX_VM_NUM; ++i) {
		if (server->vms[i] == NULL) {
			slot = (int)i;
			vm = malloc(sizeof(struct vhost_vm_s));
			break;
		}
	}

	if (vm != NULL) {
		int ret = __vhost_map_regions(device->vm_rref, &vm->regions);
		if (ret != E_HM_OK) {
			free(vm);
			vm = NULL;
		} else {
			vm->cnode_idx = device->cnode_idx;
			vm->vm_cref = device->vm_cref;
			raw_refcnt_init(&vm->refcnt, 1, NULL);
			server->vms[slot] = vm;
		}
	}

	return vm;
}

static struct vhost_vm_s *get_or_alloc_vhost_vm(struct vhost_server_s *server,
						const struct vhost_device_s *device)
{
	struct vhost_vm_s *vm = NULL;

	vm = __get_vhost_vm(server, device);
	if (vm == NULL) {
		vm = __alloc_vhost_vm(server, device);
	}

	return vm;
}

int vhost_convert_gpa_to_hva(const struct vhost_device_s *device,
			     uint64_t gpa, uint64_t len, uint64_t *hva)
{
	unsigned int i;
	int ret = E_HM_INVAL;
	struct vhost_vm_s *vm = NULL;
	struct vhost_region_s *region = NULL;

	if (hva != NULL && device != NULL && device->vm != NULL ) {
		vm = device->vm;
		for (i = 0; i < vm->regions.region_num; ++i) {
			region = &vm->regions.regions[i];
			if ((gpa >= region->gpa) && (gpa < (region->gpa + region->size)) &&
			    (region->gpa + region->size - gpa) >= len) {
				*hva = ((gpa - region->gpa) + region->hva);
				ret = E_HM_OK;
				break;
			}
		}
	}

	return ret;
}

int vhost_send_irq(const struct vhost_device_s *device)
{
	xref_t vm_xref = { .rref = device->vm_rref };
	return hm_vm_assert_irq(vm_xref, 0, vhost_to_spi(device->irq_id), 1U);
}

static int vhandler_grant_vm_request(struct vhost_device_s *device,
				     const struct vhost_user_msg_s *req,
				     struct vhost_user_msg_s *rsp)
{
	rref_t vm_rref;
	int ret = E_HM_OK;

	if (device->vm_cref != 0) {
		ret = E_HM_EXIST;
	}
	if (ret == E_HM_OK) {
		vm_rref = hm_ucap_require(0, req->u.grant_vm_request.uvmm_cnode_idx, 0);
		if (IS_REF_ERR(vm_rref)) {
			ret = REF_TO_ERR(vm_rref);
		} else {
			device->vm_cref = req->u.grant_vm_request.vm_cref;
			device->vm_rref = vm_rref;
			rsp->u.grant_vm_request.vm_rref = vm_rref;
		}
	}

	return ret;
}

static int vhandler_grant_vm_done(struct vhost_server_s *server,
				  struct vhost_device_s *device,
				  const struct vhost_user_msg_s *req)
{
	int ret = E_HM_OK;

	if (req->u.grant_vm_done.result != E_HM_OK) {
		ret = req->u.grant_vm_done.result;
	} else {
		struct sysarg_cnode_statrref stat = {0};
		ret = sysfast_cnode_stat_rref(&stat, device->vm_rref, CNODE_STATRREF_TYPE);
		if ((ret != E_HM_OK) || (stat.captype != (unsigned)hmobj_VM)) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		device->vm = get_or_alloc_vhost_vm(server, device);
		if (device->vm == NULL) {
			ret = E_HM_RESFAULT;
		}
	}

	if (ret != E_HM_OK) {
		vhost_release_device(server, device);
	}

	return ret;
}

static int vhandler_set_dev_info(struct vhost_device_s *device,
				 const struct vhost_user_msg_s *req)
{
	int ret;

	ret = strncpy_s(device->dev_name, VHOST_NAME_LEN,
			req->u.set_dev_info.name, VHOST_NAME_LEN - 1);
	if (ret != EOK) {
		ret = E_HM_INVAL;
	} else {
		ret = E_HM_OK;
	}

	return ret;
}

static int vhandler_set_features(struct vhost_backend_s *hook,
				 const struct vhost_user_msg_s *req)
{
	hook->features = req->u.set_features.features;
	return E_HM_OK;
}

static int vhandler_set_kick_mmio(struct vhost_server_s *server,
				  struct vhost_device_s *device,
				  const struct vhost_user_msg_s *req)
{
	int ret = E_HM_OK;

	if (device->irq_id != 0) {
		ret = E_HM_EXIST;
	}
	if (req->u.set_kick_mmio.irq_id == 0 ||
	    req->u.set_kick_mmio.vcpu_num == 0 ||
	    req->u.set_kick_mmio.vcpu_num > VCPU_NUM_MAX) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (server->vhost_actv_mode == __true) {
			ret  = syscap_VMRegisterMMIODispatcher(device->vm_rref,
					(unsigned long)req->u.set_kick_mmio.mmio_base,
					(unsigned long)req->u.set_kick_mmio.mmio_size,
					__DISPATCHER_TYPE_MMIO_ACTV);
		} else {
			ret  = syscap_VMRegisterMMIODispatcher(device->vm_rref,
					(unsigned long)req->u.set_kick_mmio.mmio_base,
					(unsigned long)req->u.set_kick_mmio.mmio_size,
					__DISPATCHER_TYPE_MMIO_TRIGGER);
		}
		if (ret >= 0) {
			device->mregion_id = ret;
			device->irq_id = req->u.set_kick_mmio.irq_id;
			device->vcpu_num = req->u.set_kick_mmio.vcpu_num;
			ret = E_HM_OK;
		} else {
			ret = E_HM_INVAL;
		}
	}

	return ret;
}

static int virtq_convert_gpa_to_hva(void *pdev, uint64_t gpa, uint64_t len, uint64_t *hva)
{
	struct vhost_device_s *device = (struct vhost_device_s*)pdev;
	return vhost_convert_gpa_to_hva(device, gpa, len, hva);
}

static int vhandler_set_vq_base(const struct vhost_backend_s *hook,
				struct vhost_device_s *device,
				const struct vhost_user_msg_s *req)
{
	int32_t vq_index = req->u.set_vq_base.vq_index;
	struct virtqueue_s *vq = NULL;
	uint64_t hva = 0;
	int ret = E_HM_OK;

	if (vq_index < 0 || device == NULL ||
		(unsigned int)vq_index >= device->vq_num || device->vqs == NULL) {
		ret = E_HM_INVAL;
	} else {
		vq = &device->vqs[vq_index];
		if (vq->desc != NULL && vq->avail != NULL && vq->used != NULL) {
			ret = E_HM_EXIST;
		}
	}

	if (ret == E_HM_OK) {
		ret = vhost_convert_gpa_to_hva(device, req->u.set_vq_base.desc,
					DESC_SIZE(req->u.set_vq_base.qsize), &hva);
		if (ret == E_HM_OK) {
			vq->desc = u64_to_ptr(hva, vq_desc_t);
		}
	}

	if (ret == E_HM_OK) {
		ret = vhost_convert_gpa_to_hva(device, req->u.set_vq_base.avail,
					AVAIL_SIZE(req->u.set_vq_base.qsize), &hva);
		if (ret == E_HM_OK) {
			vq->avail = u64_to_ptr(hva, vq_avail_t);
		}
	}

	if (ret == E_HM_OK) {
		ret = vhost_convert_gpa_to_hva(device, req->u.set_vq_base.used,
					USED_SIZE(req->u.set_vq_base.qsize), &hva);
		if (ret == E_HM_OK) {
			vq->used = u64_to_ptr(hva, vq_used_t);
		}
	}

	if (ret == E_HM_OK) {
		vq->qsize = req->u.set_vq_base.qsize;
		vq->convert_gpa_to_hva = virtq_convert_gpa_to_hva;
		vq->device = (void *)device;

		if (hook->handle_vq_ready != NULL) {
			hook->handle_vq_ready(vq_index, device, vq);
		}
	}

	if (vq != NULL && ret != E_HM_OK && ret != E_HM_EXIST) {
		vq->desc = NULL;
		vq->avail = NULL;
		vq->used = NULL;
	}

	return ret;
}

static void vhost_bind_mmio_dispatcher(const struct vhost_device_s *device)
{
	uint32_t i;

	for (i = 0; i < device->vcpu_num; ++i) {
		int err = syscap_VMBindVcpuMMIODispatcher(device->vm_rref,
							  (unsigned int)device->mregion_id,
							  i, NULL);
		if (err != E_HM_OK) {
			hm_warn("bind vcpu mmio dispatcher failed ret = %s\n",
				hmstrerror(err));
		}
	}
}

static void dummy_handler(int signum)
{
	UNUSED(signum);
}

static void vhost_prepare_signal(void)
{
	struct __hmsigmask block = { 1UL << HMSIGNAL_POSIX };
	struct k_sigaction act;
	int ret;

	mem_zero_s(act);
	act.ksa_handler = dummy_handler;
	ret = hm_sigaction(SIGUSR1, &act, NULL);
	if (ret != 0) {
		hm_warn("vhost register sig action failed\n");
	} else {
		ret = hm_thread_set_hmsignal_mask(0, &block, NULL);
		if (ret != E_HM_OK) {
			hm_warn("block hm posix signal failed, ret = %s", hmstrerror(ret));
		}
	}
}

static void *vhost_user_thread(void *args)
{
	struct vhost_device_s *device = (struct vhost_device_s *)args;
	int ret;

	vhost_bind_mmio_dispatcher(device);
	vhost_prepare_signal();
	(void)raw_sem_post(&device->sem);

	while (__atomic_load_n(&device->running, __ATOMIC_RELAXED)) {
		struct __hmsigmask mask = { 0 };
		__u64 data = 0;

		ret= hm_trigger_wait(0UL, &mask);
		if (ret == E_HM_HMSIGINTR) {
			continue;
		} else if (ret < 0) {
			hm_error("vhost trigger wait failed ret=%d\n", ret);
			break;
		}

		ret = sysfast_trigger_read(&data, sizeof(data));
		if (ret == E_HM_OK && data < device->vq_num) {
			struct virtqueue_s *vq = &device->vqs[data];
			if (vq->handle_kick != NULL) {
				vq->handle_kick(vq, vq->opaque);
			}
		}

		ret = sysfast_trigger_done();
		if (ret < 0) {
			hm_error("vhost user trigger done: err=%s\n", hmstrerror(ret));
		}
	}

	return NULL;
}

static int vhandler_device_done(struct vhost_backend_s *h,
				struct vhost_device_s *device,
				struct vhost_server_s *server)
{
	int ret;
	if (server->vhost_actv_mode == __true) {
		ret = do_actv_create(device, server);
		if (ret != E_HM_OK) {
			hm_error("vhost do_actv_create: err=%s\n", hmstrerror(ret));
		} else {
			vhost_bind_vhost_dispatcher(device, server);
		}
	} else {
		ret = raw_sem_wait(&device->sem);
		if (ret != E_HM_OK) {
			hm_error("wait for vhost user thread failed ret = %d\n", ret);
			(void)vhost_destroy_thread(device);
		}
	}

	if (ret == E_HM_OK) {
		ret = h->device_done(h, device);
		if (ret != E_HM_OK) {
			hm_error("vhost device_done: err=%s\n", hmstrerror(ret));
		}
	}
	return ret;
}

static int do_vhandler_all_done(struct vhost_server_s *server,
			     struct vhost_backend_s *h,
			     struct vhost_device_s *device)
{
	int ret = E_HM_OK;

	if (server->vhost_actv_mode == __true) {
		if (server->vhost_actv_inited == __false) {
			ret = E_HM_NOOBJ;
			hm_error("Actv pool is not inited\n");
		}
	} else {
		ret = raw_thread_create(&device->thread, NULL,
					vhost_user_thread, device);
		if (ret != E_HM_OK) {
			hm_error("create vhost user thread failed ret = %d\n", ret);
		}
	}

	if (ret == E_HM_OK) {
		ret = vhandler_device_done(h, device, server);
	}

	return ret;
}

static int vhandler_all_done(struct vhost_server_s *server,
			     struct vhost_backend_s *h,
			     struct vhost_device_s *device,
			     const struct vhost_user_msg_s *req)
{
	int ret = E_HM_OK;

	if (req->u.all_done.result == E_HM_OK) {
		/*
		 * When vm reset, it should only affect virtual machine
		 * related resources. The vhost-user thread should not be
		 * repeatly created, otherwise there will be a resource leak
		 * for thread.
		 */
		if (!__atomic_test_and_set(&device->running, __ATOMIC_RELAXED)) {
			ret = do_vhandler_all_done(server, h, device);
		}
	}

	if (ret != E_HM_OK || req->u.all_done.result != E_HM_OK) {
		vhost_release_device(server, device);
	}

	return ret;
}

static void vq_reset_clear(struct virtqueue_s *virtq)
{
	if (virtq != NULL) {
		virtq->avail = NULL;
		virtq->used = NULL;
		virtq->desc = NULL;
		virtq->last_avail = 0;
		virtq->saved_used = 0;
		virtq->qsize = 0;
		virtq->ready = 0;
	}
}

/*
 * When the virtual machine restarts, some resources need to
 * be cleaned up(i.e., virtqueue clear), other resources that
 * are reset in the future, should also be added here.
 */
static int vhandler_reset_dev(struct vhost_device_s *device)
{
	int ret = E_HM_OK;
	struct virtqueue_s *vq = NULL;

	if (device == NULL || device->vqs == NULL) {
		ret = E_HM_INVAL;
	} else {
		for (unsigned int i = 0; i < device->vq_num; i++) {
			vq = &device->vqs[i];
			vq_reset_clear(vq);
		}
	}

	return ret;
}

void vhost_device_dump(struct vhost_server_s *server,
		       const struct vhost_device_s *device)
{
	struct virtqueue_s *vq = NULL;

	if ((server == NULL) || (device == NULL) || (device->vqs == NULL)) {
		return;
	}

	lock_server(server);
	for (unsigned int i = 0; i < device->vq_num; i++) {
		vq = &device->vqs[i];
		if (vq->desc != NULL) {
			hm_info("dump device: %u vq: %u info: \n", device->device_id, i);
			virtq_dump(vq);
		}
	}
	unlock_server(server);
}

int vhost_server_handle_msg(struct vhost_server_s *server, uint32_t caller_cnode,
			    const struct vhost_user_msg_s *req, struct vhost_user_msg_s *rsp)
{
	struct vhost_backend_s *hook = NULL;
	struct vhost_device_s *device = NULL;
	int ret = E_HM_OK;

	lock_server(server);

	hook = get_backend_hook(server, req->virtio_id);
	if (hook == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		device = get_or_alloc_vhost_device(hook, caller_cnode,
						   req->device_id);
		if (device == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		switch (req->op) {
		case VHOST_OP_GRANT_VM_REQUEST:
			ret = vhandler_grant_vm_request(device, req, rsp);
			break;
		case VHOST_OP_GRANT_VM_DONE:
			ret = vhandler_grant_vm_done(server, device, req);
			break;
		case VHOST_OP_SET_KICK_MMIO:
			ret = vhandler_set_kick_mmio(server, device, req);
			break;
		case VHOST_OP_SET_VQ_BASE:
			ret = vhandler_set_vq_base(hook, device, req);
			break;
		case VHOST_OP_SET_DEV_INFO:
			ret = vhandler_set_dev_info(device, req);
			break;
		case VHOST_OP_SET_FEATURES:
			ret = vhandler_set_features(hook, req);
			break;
		case VHOST_OP_ALL_DONE:
			ret = vhandler_all_done(server, hook, device, req);
			break;
		case VHOST_OP_RESET_DEV:
			ret = vhandler_reset_dev(device);
			break;
		default:
			ret = E_HM_INVAL;
			break;
		}
	}

	unlock_server(server);

	return ret;
}

int vhost_server_revoke(struct vhost_server_s *server, uint32_t cnode_idx)
{
	unsigned int i;

	lock_server(server);

	for (i = 0; i < VHOST_SERVER_MAX_HOOK_NR; ++i) {
		vhost_destroy_devices(server, server->hooks[i], cnode_idx);
	}
	if (server->vhost_actv_mode == __true) {
		do_actvpool_delete(server);
	}
	unlock_server(server);

	return E_HM_OK;
}

int vhost_device_revoke(struct vhost_server_s *server,
			struct vhost_device_s *device)
{
	int ret = E_HM_OK;

	if ((server == NULL) || (device == NULL) || (device->backend == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		lock_server(server);
		vhost_release_device(server, device);
		unlock_server(server);
	}

	return ret;
}
