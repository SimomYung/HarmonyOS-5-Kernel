/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The binder IPC based on the Activation
 * Author: Huawei OS Kernel Lab
 * Create: Feb 21 10:40:44 2023
 */
#ifndef MAPI_UAPI_HMKERNEL_DRIVERS_ACTIVATION_BINDER_H
#define MAPI_UAPI_HMKERNEL_DRIVERS_ACTIVATION_BINDER_H

#include <hmkernel/types.h>
#include <hmkernel/capability.h>
#include <hmkernel/drivers/activation/binder_native.h>

#include <hmasm/drivers/activation/binder.h>

#define __ACTV_BINDER_MAX_WRITE_BUF_SZ		(1U << 20)

/* The max number of binder devices for the actv binder */
#define __ACTV_BINDER_MAX_DEVICES		(1U << 8U)

/* The info about the data sent from client to server */
struct __actv_binder_send_info {
	__u32 bc_cmd;
	/* The inner idx of the callee actv cref */
	__u32 callee_idx;
	/* The native binder transaction data of client */
	__uptr_t p_tr_data;
};

union __actv_binder_write_read {
	__u32 bc_cmd;
	struct __actv_binder_send_info send_info;
};

/* The info about waiting for ready actvs in actvpool */
struct __actv_binder_wait_info {
	__u32 code;
	__u32 bc_handle;
};

/*
 * Define the actv binder write/read command list, avoid overlapping
 * with the native binder write/read command.
 */
/* Nothing to do for the actv binder, fallback to the procedure of native binder */
#define __ACTV_BC_NONE			__ACTV_BINDER_IO('c', 96)
/* An unexpected error occurred when handling the actv binder write/read in kernel */
#define __ACTV_BC_ERROR			__ACTV_BINDER_IO('c', 97)

/* The extra args for handling the __ACTV_UPCALL_BINDER_IOCTL */
struct __actv_binder_ioctl_args {
	/* The inner idx of the caller actv cref */
	__u32 caller_idx;
	__u32 padding;

	union {
		union __actv_binder_write_read abwr;
		struct __actv_binder_wait_info wait_info;
	};
};

struct __actv_binder_pool_info {
	cref_t actv_pool_cref;
};

/*
 * Succeed to start/reply the actv binder transaction and send/reply the
 * transaction data via the read_buffer of `__actv_binder_work_info`.
 */
#define __ACTV_BINDER_WRKRET_TRANS_CPWK		0
/*
 * Succeed to start/reply the actv binder transaction but the target
 * actv needs to restart the binder ioctl to read transaction data.
 */
#define __ACTV_BINDER_WRKRET_TRANS_SUCC		1
/* Failed to start/reply the actv binder transaction */
#define __ACTV_BINDER_WRKRET_TRANS_FAIL		2

struct __actv_binder_work_meta {
	int retval;
	__u32 ret_op;

	__u32 read_buf_sz;
	__u32 read_consumed;

	/* The target bwr->read_consumed */
	__uptr_t p_read_consumed;
	/* The target bwr->read_buffer */
	__uptr_t dst_read_buffer;
};

#define __ACTV_BINDER_MAX_READ_BUF_SZ	(__ACTV_UPCALL_MAX_RET_ARG_SZ - sizeof(struct __actv_binder_work_meta))

struct __actv_binder_work_info {
	struct __actv_binder_work_meta meta_info;
	/* Consisting of `[br_cmd][arg]` */
	__u8 read_buffer[__ACTV_BINDER_MAX_READ_BUF_SZ];
};

/* The extra args returned for the __ACTV_UPCALL_BINDER_IOCTL */
union __actv_binder_ioctl_rlambda_args {
	struct __actv_binder_pool_info pool_info;
	struct __actv_binder_work_info work_info;
};

struct __actv_binder_actvmgr_args {
	unsigned int devno;
	cref_t actv_pool_cref;
	/* Help to block the tcb starting the actv binder ipc */
	const int *futex_uaddr;
	int futex_val;
};

struct __actv_binder_srvmgr_args {
	cref_t actv_pool_cref;
};

struct __actv_binder_wait_args {
	unsigned int devno;
	unsigned int handle;
	void *kobj_ab_ctx;
};

/*
 * Define the actv binder ioctl command list, avoid overlapping
 * with the native binder commmand.
 */
/*
 * Currently, the actv binder is not fully compatible with the native
 * binder. Add __ACTV_BINDER_WRITE_READ for independent evolution and
 * set it to __NATIVE_BINDER_WRITE_READ in future.
 */
#define __ACTV_BINDER_WRITE_READ	__ACTV_BINDER_IOWR('b', 97, struct __native_binder_write_read)
/* Spawn a kobj actv frame and restart the binder ioctl */
#define __ACTV_BINDER_SPAWN_ACTV_FRAME	__ACTV_BINDER_IO('b', 98)
/* Register an actvpool for the actv binder service */
#define __ACTV_BINDER_REGISTER_ACTVMGR	__ACTV_BINDER_IOW('b', 99, struct __actv_binder_actvmgr_args)
/* Register the actv binder service manager for the binder device */
#define __ACTV_BINDER_REGISTER_SRVMGR	__ACTV_BINDER_IOW('b', 100, struct __actv_binder_srvmgr_args)
/* Block the tcb to waiting for ready actvs in actvpool */
#define __ACTV_BINDER_WAIT_READY_ACTVS	__ACTV_BINDER_IOW('b', 101, struct __actv_binder_wait_args)

#endif /* !MAPI_UAPI_HMKERNEL_DRIVERS_ACTIVATION_BINDER_H */
