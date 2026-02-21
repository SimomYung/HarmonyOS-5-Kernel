/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vnotify is a communication mechanism between guest os and host os.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 12 17:57:25 2021
 */
#include <libvpipe/vnotify.h>

#include <stdlib.h>
#include <sys/mman.h>
#include <hmkernel/errno.h>
#include <hongmeng/compiler.h>
#include <hongmeng/syscall.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/atomic.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_path.h>
#include <libsysif/uvmm/api.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_uvmm.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libvpipe/vnotify_impl.h>
#include <libmem/utils.h>
#include <lib/scq.h>

raw_static_assert(sizeof(struct vnotify_msg_s) == __VNOTIFY_CHN_MSG_SIZE,
		  check_vnotify_msg_s_size);
raw_static_assert(sizeof(struct vnotify_resp_s) == __VNOTIFY_CHN_MSG_SIZE,
		  check_vnotify_resp_s_size);

#ifndef CONFIG_VNOTIFY_DRIVER_EXTENSION
void hm_vnotify_init(struct vnotify_s *vnotify)
{
	UNUSED(vnotify);
}

int hm_vnotify_attach_chn(struct vnotify_s *vnotify, const char *name, unsigned int flags)
{
	UNUSED(vnotify, name, flags);
	return E_HM_NOSYS;
}

int hm_vnotify_detach_chn(const struct vnotify_s *vnotify, bool sync, const struct timespec *timeout)
{
	UNUSED(vnotify, sync, timeout);
	return E_HM_NOSYS;
}

int hm_fast_vnotify_init(const struct vnotify_fast_args_s *args)
{
	UNUSED(args);
	return E_HM_NOSYS;
}

int hm_fast_vnotify_deinit(void)
{
	return E_HM_NOSYS;
}

int hm_vnotify_fast_notify(const struct vnotify_s *vnotify)
{
	UNUSED(vnotify);
	return E_HM_NOSYS;
}

int hm_vnotify_notify(const struct vnotify_s *vnotify)
{
	UNUSED(vnotify);
	return E_HM_NOSYS;
}

int hm_vnotify_read_status(const struct vnotify_s *vnotify, unsigned int *status)
{
	UNUSED(vnotify, status);
	return E_HM_NOSYS;
}

void hm_vnotify_set_receive_spin_times(int times)
{
	UNUSED(times);
}

int hm_vnotify_fast_receive(struct vnotify_s *vnotify,
			    const struct timespec *ts,
			    unsigned int flags)
{
	UNUSED(vnotify, ts, flags);
	return E_HM_NOSYS;
}

int hm_vnotify_inject_event(const struct vnotify_s *vnotify, unsigned int event)
{
	UNUSED(vnotify, event);
	return E_HM_NOSYS;
}

int vm_vnotify_chns_info(struct bunch *bunch)
{
	UNUSED(bunch);
	return E_HM_NOSYS;
}

int hm_vnotify_receive(struct vnotify_s *vnotify,
		       const struct timespec *ts,
		       unsigned int flags)
{
	UNUSED(vnotify, ts, flags);
	return E_HM_NOSYS;
}

int hm_vnotify_select(struct vnotify_s *vnotify, struct vnotify_select_args_s *args)
{
	UNUSED(vnotify, args);
	return E_HM_NOSYS;
}

int hm_vnotify_ppoll(struct vnotify_s *vnotify, struct vnotify_ppoll_args_s *args)
{
	UNUSED(vnotify, args);
	return E_HM_NOSYS;
}

int hm_vnotify_bind_epoll(struct vnotify_s *vnotify, unsigned int epfd, unsigned int events)
{
	UNUSED(vnotify, epfd, events);
	return E_HM_NOSYS;
}

int hm_vnotify_update_futex(struct vnotify_s *vnotify, int *futex, bool shared)
{
	UNUSED(vnotify, futex, shared);
	return E_HM_NOSYS;
}

int hm_vnotify_query_status(unsigned int chn_id, unsigned int query_type,
							struct chn_query_chn_owner_msg_resp_s *owner_info)
{
	UNUSED(chn_id, query_type, owner_info);
	return E_HM_NOSYS;
}

int hm_vnotify_init_ctrl_chn(cref_t vm_cref, __u32 cnode_idx)
{
	UNUSED(vm_cref, cnode_idx);
	return E_HM_NOSYS;
}

int hm_vnotify_receive_and_reply(void (*handler)(struct vnotify_msg_s *msg,
						 struct vnotify_resp_s *resp))
{
	UNUSED(handler);
	return E_HM_NOSYS;
}

int hm_vnotify_send_and_receive(struct vnotify_msg_s *msg,
				struct vnotify_resp_s *resp)
{
	UNUSED(msg, resp);
	return E_HM_NOSYS;
}

#else /* #ifndef CONFIG_VNOTIFY_DRIVER_EXTENSION */
#define VNOTIFY_SHM_INDEX 0

static const struct uvmm_param_s uvmm_gicpt = {
	.vm_type = VM_GICPT,
};

struct vnotify_shm_s {
	struct vnotify_mem_s tx_paddr;
	struct vnotify_mem_s rx_paddr;
	struct hm_vm_shm_info shm_info;
	unsigned int chn_max;
};

struct vnotify_fast_system_s {
	int (*kick)(const void *);
	struct vnotify_shm_s shm;
	struct raw_mutex lock;
	int spin_times;
	bool init;
};

static struct vnotify_fast_system_s g_fast_system = {
	.kick = NULL,
	.spin_times = 500, /* 500: default receive times */
	.shm = {
		.tx_paddr = {0},
		.rx_paddr = {0},
		.shm_info = {0},
		.chn_max = 0,
	},
	.lock = RAW_MUTEX_INITIALIZER,
	.init = false,
};

static bool is_vnotify_shared(const struct vnotify_s *vnotify)
{
	return (vnotify->flags & VNOTIFY_FLAGS_SHARED) != 0U ? true : false;
}

void hm_vnotify_init(struct vnotify_s *vnotify)
{
	if (vnotify != NULL) {
		mem_zero_s(*vnotify);
	}
}

/*
 * hm_vnotify_attach_chn - attach a channel
 * @vnotify: 	the vnotify meta data
 * @name:	the name of vnotify channel
 * @flags:	if VNOTIFY_ATTACH_CREATE is set, create chn with name if
 * 		it has not been created
 */
int hm_vnotify_attach_chn(struct vnotify_s *vnotify, const char *name,
			  unsigned int flags)
{
	int err = E_HM_OK;
	rref_t rref;
	struct vm_attach_chn_arg_s args;
	bool shared = false;

	if (vnotify == NULL || name == NULL || !is_valid_chn_name(name) ||
	    !is_valid_attach_flags(flags)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		vnotify->faddr = &vnotify->futex;
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
		}
	}

	if (err == E_HM_OK) {
		mem_zero_s(args);
		NOFAIL(strncpy_s(args.name, sizeof(args.name), name, CHN_NAME_MAX_LEN - 1));
		args.faddr = (unsigned long long)vnotify->faddr;
		args.flags = flags;
		err = actvxactcapcall_uvmmcall_uvmm_vm_process_attach_chn(false, true, rref, args);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	if (err > 0) {
		vnotify->chn_id = (unsigned int)err;
		vnotify->flags = 0;
		shared = ((flags & VNOTIFY_ATTACH_SHARED) != 0U) ? true : false;
		if (shared) {
			/*
			 * For shared chn, needs to update its futex outside,
			 * as the faddr may be not shared memory.
			 * update futex maybe failed here.
			 */
			err = E_HM_OK;
			vnotify->flags |= VNOTIFY_FLAGS_SHARED;
		} else {
			/*
			 * For private chn, guest creates chn with name and
			 * host attaches it. Chn created by guest does not
			 * fill faddr, so host must update futex when attach.
			 */
			vnotify->futex = 0;
			err = hm_vnotify_update_futex(vnotify, vnotify->faddr, shared);
			if (err != E_HM_OK) {
				(void)hm_vnotify_detach_chn(vnotify, false, NULL);
			}
		}
	}

	return err;
}

int hm_vnotify_detach_chn(const struct vnotify_s *vnotify, bool sync, const struct timespec *timeout)
{
	rref_t rref;
	__u64 ts = (__u64)(-1);
	int err = E_HM_INVAL;
	if (vnotify != NULL && vnotify->chn_id > 0) {
		err = E_HM_OK;
	}
	if (err == E_HM_OK) {
		if (sync && timeout != NULL) {
			err = timespec_to_values(timeout, &ts);
		}
	}
	if (err == E_HM_OK) {
		/* need to notify VM first */
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
		}
	}
	if (err == E_HM_OK) {
		err = actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(rref, vnotify->chn_id, sync, ts);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	if (err == E_HM_OK) {
		mem_zero_s(*vnotify);
	}

	return err;
}

static int __notify_share_channel(const struct vnotify_s *vnotify)
{
	rref_t rref;
	int err = E_HM_OK;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		err = actvxactcapcall_uvmmcall_uvmm_vm_process_notify_chn(false, true, rref, vnotify->chn_id);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}
	return err;
}

static int alloc_vnotify_shm(struct vnotify_shm_s *shm)
{
	rref_t rref;
	int err = E_HM_OK;
	unsigned int chns_size = 0;
	unsigned int msg_size = 0;
	unsigned int chn_max = 0;
	unsigned int map_size = 0;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		err = sysfast_vnotify_query_capacity(&map_size, &chn_max);
		UNUSED(map_size);
	}

	if (err == E_HM_OK) {
		shm->chn_max = chn_max;
		chns_size = (unsigned int)(chn_max * sizeof(struct __chn_s));
		msg_size = sizeof(struct __vnotify_chn_msg_s);

		/* For vnotify fastpath, don't need to access scq ringbuffer. */
		err = hm_vm_alloc_static_shm_with_size(rref, VNOTIFY_SHM_INDEX,
						       UVMM_SHM_O_RDWR, UVMM_SHM_DEFAULT_MODE,
						       (size_t)(((chns_size + ((size_t)msg_size << 1)) << 1)),
						       0, &shm->shm_info);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	if (err == E_HM_OK) {
		shm->rx_paddr.chns = (struct __chn_s *)
				     shm->shm_info.vaddr;
		shm->rx_paddr.host_msg = (struct __vnotify_chn_msg_s *)
					 ptr_add(shm->rx_paddr.chns, chns_size);
		shm->rx_paddr.guest_msg = (struct __vnotify_chn_msg_s *)
					  ptr_add(shm->rx_paddr.host_msg, msg_size);
		shm->tx_paddr.chns = (struct __chn_s *)
				     ptr_add(shm->rx_paddr.guest_msg, msg_size);
		shm->tx_paddr.host_msg = (struct __vnotify_chn_msg_s *)
					 ptr_add(shm->tx_paddr.chns, chns_size);
		shm->tx_paddr.guest_msg = (struct __vnotify_chn_msg_s *)
					  ptr_add(shm->tx_paddr.host_msg, msg_size);
	}

	return err;
}

static int free_vnotify_shm(struct vnotify_shm_s *shm)
{
	rref_t rref;
	int err = E_HM_OK;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		err = hm_vm_free_static_shm(rref, VNOTIFY_SHM_INDEX, &shm->shm_info);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	if (err == E_HM_OK) {
		mem_zero_b(shm, sizeof(struct vnotify_shm_s));
	}
	return err;
}

int hm_fast_vnotify_init(const struct vnotify_fast_args_s *args)
{
	int err = E_HM_OK;
	if (args == NULL || args->kick == NULL) {
		err = E_HM_INVAL;
	}

	raw_mutex_lock(&g_fast_system.lock);
	if (g_fast_system.init == false) {
		if (err == E_HM_OK) {
			err = alloc_vnotify_shm(&g_fast_system.shm);
		}

		if (err == E_HM_OK) {
			g_fast_system.kick = args->kick;
			g_fast_system.init = true;
		}
	}
	raw_mutex_unlock(&g_fast_system.lock);

	return err;
}

int hm_fast_vnotify_deinit(void)
{
	int err = E_HM_OK;
	raw_mutex_lock(&g_fast_system.lock);
	if (g_fast_system.init == true) {
		err = free_vnotify_shm(&g_fast_system.shm);
		if (err == E_HM_OK) {
			g_fast_system.kick = NULL;
			g_fast_system.init = false;
		}
	}
	raw_mutex_unlock(&g_fast_system.lock);
	return err;
}

static inline bool is_chn_occupied(struct __chn_s *ch)
{
	bool ret = false;
	unsigned int flag;

	flag = vatomic32_read(&ch->flags);
	if ((flag & CHN_MASK_OCCUPIED) == CHN_MASK_OCCUPIED) {
		ret = true;
	}
	return ret;
}

static int channel_id_check(const struct vnotify_shm_s *shm, unsigned int chn_id)
{
	int err = E_HM_OK;

	if (chn_id >= shm->chn_max || chn_id < VNOTIFY_RESERVED_CHANNEL ||
	    !is_chn_occupied(&shm->rx_paddr.chns[chn_id])) {
		err = E_HM_INVAL;
	}
	return err;
}

int hm_vnotify_fast_notify(const struct vnotify_s *vnotify)
{
	int err = E_HM_OK;

	if (vnotify == NULL || g_fast_system.init == false) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = hm_vnotify_notify(vnotify);
	}

	return err;
}

int hm_vnotify_notify(const struct vnotify_s *vnotify)
{
	int err = E_HM_OK;

	if (vnotify == NULL || vnotify->faddr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (is_vnotify_shared(vnotify)) {
			err = __notify_share_channel(vnotify);
		} else {
			err = sysfast_vnotify_notify_channel(vnotify->chn_id, NULL);
		}
	}
	return err;
}

static int __read_share_channel_status(const struct vnotify_s *vnotify, unsigned int *status)
{
	rref_t rref;
	int err = E_HM_OK;
	struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status actv_ret;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		err = actvxactcapcall_uvmmcall_uvmm_vm_process_read_chn_status(false, true, rref, vnotify->chn_id,
									       &actv_ret);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	if (err == E_HM_OK) {
		*status = actv_ret.status;
	}
	return err;
}

int hm_vnotify_read_status(const struct vnotify_s *vnotify, unsigned int *status)
{
	int err = E_HM_OK;
	if (vnotify == NULL || vnotify->faddr == NULL || status == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (is_vnotify_shared(vnotify)) {
			err = __read_share_channel_status(vnotify, status);
		} else {
			err = sysfast_vnotify_read_status(vnotify->chn_id, status);
		}
	}

	return err;
}

void hm_vnotify_set_receive_spin_times(int times)
{
	g_fast_system.spin_times = times;
}

static int do_vnotify_fast_receive(struct vnotify_s *vnotify,
				   const struct timespec *ts,
				   unsigned int flags)
{
	int max_spin_times = g_fast_system.spin_times;
	struct __chn_s *rx = NULL;
	unsigned int status_new = 0U;
	unsigned int status_old = 0U;
	unsigned int chn_id = vnotify->chn_id;
	int val = 0;
	int index = 0;
	int ret = E_HM_OK;

	rx = &g_fast_system.shm.rx_paddr.chns[chn_id];
	while (index < max_spin_times) {
		status_old = (unsigned int)vatomic32_read(&rx->status);
		status_new = status_old & (~__VNOTIFY_NOTIFYING);
		if (vatomic32_cmpxchg(&rx->status, status_old, status_new) == status_old) {
			if ((status_old & __VNOTIFY_NOTIFYING) == __VNOTIFY_NOTIFYING) {
				break;
			}
		}
		status_old = 0;

		val = raw_atomic_int_swap((raw_atomic_int_t *)(uintptr_t)(vnotify->faddr), 0);
		if (val != 0) {
			break;
		}
		index++;
	}

	if (val != 0) {
		/* if guest detach chn, __VNOTIFY_CLOSED bit will be set */
		if (((unsigned int)val & __VNOTIFY_CLOSED) == __VNOTIFY_CLOSED) {
			vnotify->flags |= VNOTIFY_FLAGS_ClOSED;
			/*
			 * if there is no message and guest vnotify has been closed,
			 * return E_HM_PIPE directly.
			 */
			if (((unsigned int)val & __VNOTIFY_INTR) != __VNOTIFY_INTR) {
				ret = E_HM_PIPE;
			}
		}
	} else if ((status_old & __VNOTIFY_NOTIFYING) == 0) {
		ret = hm_vnotify_receive(vnotify, ts, flags);
	} else {
		/* do nothing */
	}

	return ret;
}

int hm_vnotify_fast_receive(struct vnotify_s *vnotify,
			    const struct timespec *ts,
			    unsigned int flags)
{
	int ret = E_HM_OK;

	if (vnotify == NULL || vnotify->faddr == NULL || g_fast_system.init == false) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if ((vnotify->flags & VNOTIFY_FLAGS_ClOSED) == VNOTIFY_FLAGS_ClOSED) {
			ret = E_HM_PIPE;
		}
	}

	if (ret == E_HM_OK) {
		ret = channel_id_check(&g_fast_system.shm, vnotify->chn_id);
	}

	if (ret == E_HM_OK) {
		ret = do_vnotify_fast_receive(vnotify, ts, flags);
	}

	return ret;
}

int hm_vnotify_receive(struct vnotify_s *vnotify,
		       const struct timespec *ts,
		       unsigned int flags)
{
	int err = E_HM_OK;
	unsigned int futex_flags;
	int val = 0;

	if (vnotify == NULL || vnotify->faddr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if ((vnotify->flags & VNOTIFY_FLAGS_ClOSED) == VNOTIFY_FLAGS_ClOSED) {
			err = E_HM_PIPE;
		}
	}

	if (err == E_HM_OK) {
		val = raw_atomic_int_swap((raw_atomic_int_t *)(uintptr_t)(vnotify->faddr), 0);
	}

	/*
	 * val != 0 means there has been message pending on the channel,
	 * so do not futex_wait in this case.
	 */
	if (err == E_HM_OK && val == 0) {
		if ((flags & VNOTIFY_NONBLOCK) == 0U) {
			futex_flags = is_vnotify_shared(vnotify) ? FUTEX_TYPE_SHARED : 0U;

#ifdef CONFIG_USER_FUTEX
			/*
			 * Cannot use user futex here since the correponding futex wake is done in kernel.
			 * User-futex requires that both wait and wake calls are done through the Hm_futex ulib.
			 * __FUTEX_FLAG_USER_NOT ensures this futex_wait will not use user futex.
			 */
			futex_flags |= __FUTEX_FLAG_USER_NOT;
#endif /* CONFIG_USER_FUTEX */

			if ((flags & VNOTIFY_INTR) == 0U) {
				err = hm_futex_wait((volatile int *)(uintptr_t)vnotify->faddr, 0, ts, futex_flags);
			} else {
				err = hm_futex_wait_interruptible((volatile int *)(uintptr_t)vnotify->faddr,
								  0, ts, futex_flags);
			}
			if (err == E_HM_AGAIN) {
				err = E_HM_OK;
			}
			/*
			 * Read the futex value again, the value indicates
			 * the reason for being woke up.
			 * If err != E_HM_OK, the faddr should not be cleared.
			 * Otherwise, notifications will be lost in this scenario:
			 * 1. signals wake up hm_futex_wait.
			 * 2. guest sends message to host and update faddr.
			 * 3. invokes raw_atomic_int_swap to clear faddr.
			 */
			if (err == E_HM_OK) {
				val = raw_atomic_int_swap((raw_atomic_int_t *)(uintptr_t)vnotify->faddr, 0);
			}
		} else {
			/*
			 * No message received and in non-block mode, keep the errno the same as block mode.
			 * so return E_HM_TIMEDOUT to caller.
			 */
			err = E_HM_TIMEDOUT;
		}
	}

	if (err == E_HM_OK) {
		if (((unsigned int)val & __VNOTIFY_CLOSED) == __VNOTIFY_CLOSED) {
			vnotify->flags |= VNOTIFY_FLAGS_ClOSED;
			/* if there is no message and guest vnotify has been closed, return E_HM_PIPE directly */
			if (((unsigned int)val & __VNOTIFY_INTR) != __VNOTIFY_INTR) {
				err = E_HM_PIPE;
			}
		}
	}
	return err;
}

int hm_vnotify_attach_fd(struct vnotify_s *vnotify)
{
	int err = E_HM_OK;
	rref_t rref;

	if (vnotify == NULL || vnotify->faddr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
		}
	}

	if (err == E_HM_OK) {
		err = actvxactcapcall_uvmmcall_uvmm_vnotify_attach_fd(false, true, rref, vnotify->chn_id);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}
	return err;
}

int hm_vnotify_select(struct vnotify_s *vnotify, struct vnotify_select_args_s *args)
{
	int err = E_HM_OK;

	if (vnotify == NULL || args == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = sysfast_vnotify_select_channel(args->nfds, args->readfds,
						     args->writefds, args->exceptfds,
						     args->timeout, args->sigmask,
						     vnotify->chn_id);
	}
	return err;
}

int hm_vnotify_ppoll(struct vnotify_s *vnotify, struct vnotify_ppoll_args_s *args)
{
	int err = E_HM_OK;

	if (vnotify == NULL || args == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = sysfast_vnotify_ppoll_channel(args->fds, args->nfds,
						    args->timeout, args->sigmask,
						    args->sigsetsize, vnotify->chn_id);
	}
	return err;
}

int hm_vnotify_bind_epoll(struct vnotify_s *vnotify, unsigned int epfd, unsigned int events)
{
	int err = E_HM_OK;

	if (vnotify == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = sysfast_vnotify_bind_epoll(vnotify->chn_id, epfd, events);
	}
	return err;
}

int hm_vnotify_update_futex(struct vnotify_s *vnotify, int *futex, bool shared)
{
	int err = E_HM_OK;
#ifdef CONFIG_FORK_COW
	uintptr_t aligned_futex;
#endif
	rref_t rref;

	if (vnotify == NULL || vnotify->faddr == NULL || futex == NULL) {
		err = E_HM_INVAL;
	}
#ifdef CONFIG_FORK_COW
	if (err == E_HM_OK) {
		/*
		 * futex(which is used to wake up vnotify) will be changed
		 * by both microkernel and user process. microkernel directly
		 * rewrites the physical address corresponding to the futex.
		 * after cow, process futex will remaps to another physical
		 * memory. as a result, the modification changed by microkernel
		 * cannot be synchronized to the process.
		 */
		if (!shared) {
			aligned_futex = PAGE_ALIGN_DOWN((uintptr_t)futex);
			err = hm_mem_madvise((const void *)aligned_futex, sizeof(int), MADV_FLAG_NOCOW);
		}
	}
#endif

	if (err == E_HM_OK) {
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
		}
	}

	if (err == E_HM_OK) {
		vnotify->faddr = futex;
		err = actvxactcapcall_uvmmcall_uvmm_vnotify_update_futex(false, true, rref, vnotify->chn_id, futex, shared);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}
	return err;
}

static int __inject_share_channel_event(const struct vnotify_s *vnotify)
{
	rref_t rref;
	int err = E_HM_OK;

	rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
	}

	if (err == E_HM_OK) {
		err = actvxactcapcall_uvmmcall_uvmm_vm_process_inject_chn(false, true, rref, vnotify->chn_id);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}
	return err;
}

int hm_vnotify_inject_event(const struct vnotify_s *vnotify, unsigned int event)
{
	int err = E_HM_INVAL;
	if (vnotify != NULL && vnotify->faddr != NULL) {
		if (is_vnotify_shared(vnotify)) {
			err = __inject_share_channel_event(vnotify);
		} else {
			err = sysfast_vnotify_inject_event(vnotify->chn_id, event);
		}
	}
	return err;
}

static int vnotify_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	rref_t rref = *(rref_t *)data;
	return actvxactcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(false, true, rref, *attr);
}

int vm_vnotify_chns_info(struct bunch *bunch)
{
	int err = E_HM_OK;
	rref_t rref;
	struct bunch_ipc_reader_ctx ctx;

	if (bunch == NULL) {
		hm_error("bunch=0x%lx\n", vaddr_of(bunch));
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(ctx);
		ctx.target_info = rref;
		ctx.ipc = vnotify_stat_ipc;
		ctx.private_data = ptr_to_void(&rref);
		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_error("bunch ipc try request data with shm failed: %s\n",
				hmstrerror(err));
		}
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	return err;
}

int hm_vnotify_query_status(unsigned int chn_id, unsigned int query_type,
							struct chn_query_chn_owner_msg_resp_s *owner_info)
{
	rref_t rref;
	int err = E_HM_OK;
	struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status actv_ret;

	if (query_type == VNOTIFY_QUERY_CHN_OWNER_MSG) {
		if (owner_info == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		/* need to notify VM first */
		rref = hm_uvmm_rref_acquire(&uvmm_gicpt);
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
		}
	}

	if (err == E_HM_OK) {
		err = actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(rref, chn_id, query_type, &actv_ret);
		hm_uvmm_rref_release(&uvmm_gicpt, rref);
	}

	if (query_type == VNOTIFY_QUERY_CHN_OWNER_MSG && err == E_HM_OK) {
		owner_info->owner_host.pid = actv_ret.host_info.pid;
		owner_info->owner_host.uid = actv_ret.host_info.uid;
		owner_info->owner_host.gid = actv_ret.host_info.gid;
		owner_info->owner_guest.pid = actv_ret.guest_info.pid;
		owner_info->owner_guest.uid = actv_ret.guest_info.uid;
		owner_info->owner_guest.gid = actv_ret.guest_info.gid;
	}

	return err;
}

static struct {
	struct raw_mutex host_mutex;
	struct raw_mutex guest_mutex;
	struct vnotify_s vnotify_host;
	struct vnotify_s vnotify_guest;
} ctrl_vnotify;

int hm_vnotify_init_ctrl_chn(cref_t vm_cref, __u32 cnode_idx)
{
	int rc;
	struct __vnotify_config_bind_s bind_config;

	mem_zero_s(ctrl_vnotify);

	raw_mutex_init(&ctrl_vnotify.host_mutex);
	raw_mutex_init(&ctrl_vnotify.guest_mutex);
	ctrl_vnotify.vnotify_host.chn_id = VNOTIFY_HOST_CTRL_CHANNEL;
	ctrl_vnotify.vnotify_host.faddr = &ctrl_vnotify.vnotify_host.futex;
	rc = sysfast_vnotify_update_futex(ctrl_vnotify.vnotify_host.chn_id,
					  &ctrl_vnotify.vnotify_host.futex, 0);
	if (rc == E_HM_OK) {
		ctrl_vnotify.vnotify_guest.chn_id = VNOTIFY_GUEST_CTRL_CHANNEL;
		ctrl_vnotify.vnotify_guest.faddr = &ctrl_vnotify.vnotify_guest.futex;
		rc = sysfast_vnotify_update_futex(ctrl_vnotify.vnotify_guest.chn_id,
						  &ctrl_vnotify.vnotify_guest.futex, 0);
		/* No need to rollback for host ctrl channel futex */
	}

	if (rc == E_HM_OK) {
		bind_config.__id = VNOTIFY_HOST_CTRL_CHANNEL;
		bind_config.__binder_idx = cnode_idx;
		rc = syscap_VMVnotifyConfig(vm_cref, VNOTIFY_CONFIG_BIND_CHANNEL,
					    &bind_config, sizeof(bind_config));
	}

	if (rc == E_HM_OK) {
		bind_config.__id = VNOTIFY_GUEST_CTRL_CHANNEL;
		bind_config.__binder_idx = cnode_idx;
		rc = syscap_VMVnotifyConfig(vm_cref, VNOTIFY_CONFIG_BIND_CHANNEL,
					    &bind_config, sizeof(bind_config));
		if (rc != E_HM_OK) {
			struct __vnotify_config_unbind_s unbind_config = {.__id = VNOTIFY_HOST_CTRL_CHANNEL};
			(void)syscap_VMVnotifyConfig(vm_cref, VNOTIFY_CONFIG_UNBIND_CHANNEL,
						     &unbind_config, sizeof(unbind_config));
		}
	}

	return rc;
}

int hm_vnotify_receive_and_reply(void (*handler)(struct vnotify_msg_s *msg,
						 struct vnotify_resp_s *resp))
{
	struct vnotify_msg_s msg;
	struct vnotify_resp_s resp;
	int rc;

	mem_zero_s(msg);
	mem_zero_s(resp);

	/*
	 * No need to read futex value before, if futex value is not
	 * equal to 0, the hm_futex_wait will return E_HM_AGAIN.
	 */
	unsigned int futex_flags = 0;

#ifdef CONFIG_USER_FUTEX
	/*
	 * Cannot use user futex here since the correponding futex wake is done in kernel.
	 * User-futex requires that both wait and wake calls are done through the Hm_futex ulib.
	 * __FUTEX_FLAG_USER_NOT ensures this futex_wait will not use user futex.
	 */
	futex_flags = __FUTEX_FLAG_USER_NOT;
#endif /* CONFIG_USER_FUTEX */

	rc = hm_futex_wait(ctrl_vnotify.vnotify_guest.faddr, 0, NULL, futex_flags);
	if (rc == E_HM_AGAIN) {
		rc = E_HM_OK;
	}
	if (rc == E_HM_OK) {
		(void)raw_atomic_int_set((raw_atomic_int_t *)(uintptr_t)(ctrl_vnotify.vnotify_guest.faddr), 0);
		raw_mutex_lock(&ctrl_vnotify.guest_mutex);
		rc = sysfast_vnotify_read_chn_msg(ctrl_vnotify.vnotify_guest.chn_id,
						  (struct __vnotify_chn_msg_s *)(uintptr_t)&msg);
		if (rc == E_HM_OK && handler != NULL) {
			handler(&msg, &resp);
		}
		if (rc == E_HM_OK) {
			rc = sysfast_vnotify_notify_ctrl_channel(ctrl_vnotify.vnotify_guest.chn_id,
								 (struct __vnotify_chn_msg_s *)&resp);
		}
		raw_mutex_unlock(&ctrl_vnotify.guest_mutex);
	}
	return rc;
}

static int __send_and_receive_msg(struct vnotify_msg_s *msg,
				  struct vnotify_resp_s *resp)
{
	int rc;
	unsigned int futex_flags = 0;

#ifdef CONFIG_USER_FUTEX
	/*
	 * Cannot use user futex here since the correponding futex wake is done in kernel.
	 * User-futex requires that both wait and wake calls are done through the Hm_futex ulib.
	 * __FUTEX_FLAG_USER_NOT ensures this futex_wait will not use user futex.
	 */
	futex_flags = __FUTEX_FLAG_USER_NOT;
#endif /* CONFIG_USER_FUTEX */

	raw_mutex_lock(&ctrl_vnotify.host_mutex);
	rc = sysfast_vnotify_notify_ctrl_channel(ctrl_vnotify.vnotify_host.chn_id,
						 (struct __vnotify_chn_msg_s *)msg);
	if (rc == E_HM_OK) {
		rc = hm_futex_wait(ctrl_vnotify.vnotify_host.faddr, 0, NULL, futex_flags);
		if (rc == E_HM_AGAIN) {
			rc = E_HM_OK;
		}
	}
	if (rc == E_HM_OK) {
		(void)raw_atomic_int_set((raw_atomic_int_t *)(uintptr_t)(ctrl_vnotify.vnotify_host.faddr), 0);
		rc = sysfast_vnotify_read_chn_msg(ctrl_vnotify.vnotify_host.chn_id,
						  (struct __vnotify_chn_msg_s *)(uintptr_t)resp);
	}
	raw_mutex_unlock(&ctrl_vnotify.host_mutex);
	return rc;
}

int hm_vnotify_send_and_receive(struct vnotify_msg_s *msg,
				struct vnotify_resp_s *resp)
{
	int rc = E_HM_INVAL;

	if (msg != NULL && resp != NULL) {
		rc = __send_and_receive_msg(msg, resp);
	}
	return rc;
}

#endif /* #ifndef CONFIG_VNOTIFY_DRIVER_EXTENSION */

bool is_valid_chn_name(const char *name)
{
	int len;
	bool ret = false;

	if (name != NULL) {
		len = (int)strnlen(name, CHN_NAME_MAX_LEN);
		ret = (len > 0 && len < CHN_NAME_MAX_LEN) ? true : false;
	}
	return ret;
}

bool is_valid_attach_flags(unsigned int flags)
{
	bool rc = true;

	/* only support VNOTIFY_ATTACH_CREATE, VNOTIFY_ATTACH_SHARED and VNOTIFY_ATTACH_PERSISTENT for now */
	/* perm flags only support VNOTIFY_CAP_OTHER, VNOTIFY_CAP_GROUP and VNOTIFY_CAP_USER for now */
	if ((flags & ~(VNOTIFY_ATTACH_CREATE | VNOTIFY_ATTACH_SHARED | VNOTIFY_ATTACH_PERSISTENT
		       | VNOTIFY_CAP_OTHER | VNOTIFY_CAP_GROUP | VNOTIFY_CAP_USER)) != 0U) {
		rc = false;
	}
	return rc;
}
