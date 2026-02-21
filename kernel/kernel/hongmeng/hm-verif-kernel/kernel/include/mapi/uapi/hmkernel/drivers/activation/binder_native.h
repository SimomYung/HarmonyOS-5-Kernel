/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The declarations of the native binder IPC
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 09 10:40:30 2023
 */
#ifndef MAPI_UAPI_HMKERNEL_DRIVERS_ACTIVATION_BINDER_NATIVE_H
#define MAPI_UAPI_HMKERNEL_DRIVERS_ACTIVATION_BINDER_NATIVE_H

#include <hmasm/drivers/activation/binder.h>

/* Only compatible with the native binder IPC on aarch64 currently */
typedef __u64 __native_binder_size_t;
typedef __u64 __native_binder_uptr_t;

/* Compatible with native binder IPC protocol of version 8 */
#define __NATIVE_BINDER_PROTOCOL_VERSION		8

/* Compatible with `struct binder_write_read` in native binder */
struct __native_binder_write_read {
	__native_binder_size_t write_buf_sz;
	__native_binder_size_t write_consumed;
	__native_binder_uptr_t write_buffer;
	__native_binder_size_t read_buf_sz;
	__native_binder_size_t read_consumed;
	__native_binder_uptr_t read_buffer;
};

struct __native_binder_transaction_header {
	union {
		__u32 bc_handle;

		struct {
			__native_binder_uptr_t br_handle;
			__native_binder_uptr_t br_cookie;
		};
	};

	__u32 code;
	__u32 flags;
};

/* Compatible with `struct binder_transaction_data` in native binder */
struct __native_binder_transaction_data {
	struct __native_binder_transaction_header header;

	int sender_pid;
	int sender_euid;

	__native_binder_size_t data_buf_sz;
	__native_binder_size_t objs_buf_sz;

	__native_binder_uptr_t data_buffer;
	__native_binder_uptr_t objs_buffer;
};

/* Compatible with `struct binder_transaction_data_sg` in native binder */
struct __native_binder_transaction_data_sg {
	struct __native_binder_transaction_data tr_data;
	__native_binder_size_t buffers_size;
};

/* Compatible with `struct binder_transaction_data_secctx` in native binder */
struct __native_binder_transaction_data_secctx {
	struct __native_binder_transaction_data tr_data;
	__native_binder_uptr_t secctx;
};

/* Compatible with `struct binder_ptr_cookie` in native binder */
struct __native_binder_ptr_cookie {
	__native_binder_uptr_t ptr;
	__native_binder_uptr_t cookie;
};

/* Compatible with `struct binder_handle_cookie` in native binder */
struct __native_binder_handle_cookie {
	__u32 handle;
	__native_binder_uptr_t cookie;
} __attribute__((packed));

/* Compatible with `struct flat_binder_object` in native binder */
struct __native_binder_flat_binder_object {
	__u32 type;
	__u32 flags;

	union {
		__native_binder_uptr_t binder;
		__u32 handle;
	};

	__native_binder_uptr_t cookie;
};

#define __NATIVE_BINDER_CMD_SZ		(sizeof(__u32))
#define __NATIVE_BINDER_TR_DATA_SZ	(sizeof(struct __native_binder_transaction_data))
#define __NATIVE_BINDER_TR_SECCTX_SZ	(sizeof(struct __native_binder_transaction_data_secctx))

/* Compatible with `TF_ONE_WAY` flag in native binder */
#define __NATIVE_BINDER_TF_ONE_WAY	0x01

/* Compatible with `BINDER_WRITE_READ` in native binder */
#define __NATIVE_BINDER_WRITE_READ	__ACTV_BINDER_IOWR('b', 1, struct __native_binder_write_read)
/* Compatible with `BC_TRANSACTION` in native binder */
#define __NATIVE_BC_TRANSACTION		__ACTV_BINDER_IOW('c', 0, struct __native_binder_transaction_data)
/* Compatible with `BC_REPLY` in native binder */
#define __NATIVE_BC_REPLY		__ACTV_BINDER_IOW('c', 1, struct __native_binder_transaction_data)
/* Compatible with `BC_FREE_BUFFER` in native binder */
#define __NATIVE_BC_FREE_BUFFER		__ACTV_BINDER_IOW('c', 3, __native_binder_uptr_t)
/* Compatible with `BC_INCREFS` in native binder */
#define __NATIVE_BC_INCREFS		__ACTV_BINDER_IOW('c', 4, __u32)
/* Compatible with `BC_ACQUIRE` in native binder */
#define __NATIVE_BC_ACQUIRE		__ACTV_BINDER_IOW('c', 5, __u32)
/* Compatible with `BC_RELEASE` in native binder */
#define __NATIVE_BC_RELEASE		__ACTV_BINDER_IOW('c', 6, __u32)
/* Compatible with `BC_DECREFS` in native binder */
#define __NATIVE_BC_DECREFS		__ACTV_BINDER_IOW('c', 7, __u32)
/* Compatible with `BC_INCREFS_DONE` in native binder */
#define __NATIVE_BC_INCREFS_DONE	__ACTV_BINDER_IOW('c', 8, struct __native_binder_ptr_cookie)
/* Compatible with `BC_ACQUIRE_DONE` in native binder */
#define __NATIVE_BC_ACQUIRE_DONE	__ACTV_BINDER_IOW('c', 9, struct __native_binder_ptr_cookie)
/* Compatible with `BC_REGISTER_LOOPER` in native binder */
#define __NATIVE_BC_REGISTER_LOOPER	__ACTV_BINDER_IO('c', 11)
/* Compatible with `BC_ENTER_LOOPER` in native binder */
#define __NATIVE_BC_ENTER_LOOPER	__ACTV_BINDER_IO('c', 12)
/* Compatible with `BC_EXIT_LOOPER` in native binder */
#define __NATIVE_BC_EXIT_LOOPER		__ACTV_BINDER_IO('c', 13)
/* Compatible with `BC_REQUEST_DEATH_NOTIFICATION` in native binder */
#define __NATIVE_BC_REQUEST_DEATH_NOTIFICATION	__ACTV_BINDER_IOW('c', 14, struct __native_binder_handle_cookie)
/* Compatible with `BC_CLEAR_DEATH_NOTIFICATION` in native binder */
#define __NATIVE_BC_CLEAR_DEATH_NOTIFICATION	__ACTV_BINDER_IOW('c', 15, struct __native_binder_handle_cookie)
/* Compatible with `BC_DEAD_BINDER_DONE` in native binder */
#define __NATIVE_BC_DEAD_BINDER_DONE	__ACTV_BINDER_IOW('c', 16, __native_binder_uptr_t)
/* Compatible with `BC_TRANSACTION_SG` in native binder */
#define __NATIVE_BC_TRANSACTION_SG	__ACTV_BINDER_IOW('c', 17, struct __native_binder_transaction_data_sg)
/* Compatible with `BC_REPLY_SG` in native binder */
#define __NATIVE_BC_REPLY_SG		__ACTV_BINDER_IOW('c', 18, struct __native_binder_transaction_data_sg)
/* Compatible with `BC_TRANSLATION` in native binder */
#define __NATIVE_BC_TRANSLATION		__ACTV_BINDER_IOW('c', 21, struct __native_binder_flat_binder_object)

#endif /* MAPI_UAPI_HMKERNEL_DRIVERS_ACTIVATION_BINDER_NATIVE_H */
