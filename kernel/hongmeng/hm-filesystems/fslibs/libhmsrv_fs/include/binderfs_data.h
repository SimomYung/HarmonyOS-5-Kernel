/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Binderfs uapp data structures
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 10 15:27:29 2020
 */

#ifndef MOBILE_BINDER_H
#define MOBILE_BINDER_H

#include <sys/ioctl.h>
#include <hmasm/types.h>
#include <hmkernel/drivers/activation/binder.h>

#define HMB_TYPE_LOCAL			0x73622a85
#define HMB_TYPE_LOCAL_WEAK		0x77622a85
#define HMB_TYPE_REMOTE 		0x73682a85
#define HMB_TYPE_REMOTE_WEAK		0x77682a85
#define HMB_TYPE_FD 			0x66642a85
#define HMB_TYPE_FD_ARRAY		0x66646185
#define HMB_TYPE_BUFFER			0x70742a85
#define HMB_TYPE_ROC			0x726f6385

#define HMB_PRIORITY_MASK		0xff
#define HMB_ACCEPTS_FDS_FLAG 		0x100
#define HMB_SEC_CTX_FLAG 		0x1000

#define HMB_HAS_PARENT_FLAG		0x01
#define HMB_PROTOCOL_VERSION		8

#ifdef CONFIG_FSLIB_HM_BINDER_32BIT
typedef __u32 binderfs_size_t;
typedef __u32 binderfs_uintptr_t;
#else
typedef __u64 binderfs_size_t;
typedef __u64 binderfs_uintptr_t;
#endif

struct hmb_object_remote {
	__u32 type;
	__u32 flags;
	union {
		binderfs_uintptr_t addr;
		__u32 desc;
	};
	binderfs_uintptr_t cookie;
};

struct hmb_object_fd {
	__u32 type;
	union {
		__u32 padding_flags;
		__u32 send_cidx;
	};
	union {
		binderfs_uintptr_t padding;
		__u32 fd;
	};
	binderfs_uintptr_t cookie;
};

struct hmb_object_buffer {
	__u32 type;
	__u32 flags;
	binderfs_uintptr_t addr;
	binderfs_size_t len;
	binderfs_size_t parent_idx;
	binderfs_size_t off_to_parent;
};

struct hmb_object_fd_array {
	__u32 type;
	union {
		__u32 padding;
		__u32 send_cidx;
	};
	binderfs_size_t fd_num;
	binderfs_size_t parent_idx;
	binderfs_size_t off_to_parent;
};

struct hmb_write_read {
	binderfs_size_t w_size;
	binderfs_size_t w_consumed;
	binderfs_uintptr_t w_buffer;
	binderfs_size_t r_size;
	binderfs_size_t r_consumed;
	binderfs_uintptr_t r_buffer;
};

struct hmb_node_debug_info {
	binderfs_uintptr_t addr;
	binderfs_uintptr_t cookie;
	__u32 has_strong_ref;
	__u32 has_weak_ref;
};

struct hmb_node_info_for_ref {
	__u32 desc;
	__u32 strong_num;
	__u32 weak_num;
	__u32 padding1;
	__u32 padding2;
	__u32 padding3;
};

struct hmb_access_token {
	__u64 tokenid;
	__u64 ftokenid;
	__u64 reserved[2];
};

struct hmb_sender_info {
	struct hmb_access_token tokens;
	__u64 sender_pid;
};

struct hmb_feature_set {
	__u64 feature_set;
};

struct hmb_object_roc {
	__u32 type;
	// Upper 16 bits is java_type_id, lower 16 bits is size_8bytes
	__u32 type_and_size;
	union {
		binderfs_uintptr_t jni_env;
		binderfs_uintptr_t binder_ptr;
	};
	binderfs_uintptr_t art_ptr;
};

struct hmb_roc_init_args {
	// The address of region_states field
	binderfs_uintptr_t region_states;
	// The address of regions_shared_info field
	binderfs_uintptr_t regions_shared_info;
};

struct hmb_roc_register_args {
	// The begin addresses of regions
	binderfs_uintptr_t regions;
	// The number of regions
	__u8 num;
};

#define HMB_WRITE_READ			_IOWR('b', 1, struct hmb_write_read)
#define HMB_SET_MAX_THREADS		_IOW('b', 5, __u32)
#define HMB_SET_CTXMGR			_IOW('b', 7, __s32)
#define HMB_THREAD_EXIT			_IOW('b', 8, __s32)
#define HMB_VERSION			_IOWR('b', 9, __s32)
#define HMB_GET_NODE_DEBUG_INFO		_IOWR('b', 11, struct hmb_node_debug_info)
#define HMB_GET_NODE_INFO_FOR_REF	_IOWR('b', 12, struct hmb_node_info_for_ref)
#define HMB_SET_CTXMGR_EXT		_IOW('b', 13, struct hmb_object_remote)
#define HMB_ENABLE_ONEWAY_SPAM_DETECTION		_IOW('b', 16, __u32)

/* callnum from 64 ~ 95 for the actv binder command handled in the binderfs */
/*
 * Set the binder service as an actv binder service, then it can provide the
 * binder ipc through the actv binder. In particular, the inner idx of the pool
 * rref will be used as the handle for referencing the hmb_refer.
 */
#define HMB_SET_ACTVMGR			_IOWR('b', 64, cref_t)
#define HMB_FEATURE_SET			_IOWR('b', 30, struct hmb_feature_set)
#define HMB_GET_ACCESS_TOKEN    _IOWR('b', 31, struct hmb_access_token)
#define HMB_GET_SENDER_INFO		_IOWR('b', 32, struct hmb_sender_info)

/*
 * callnum from 96 ~ 127 for the __ACTV_BINDER_xxx command
 *
 * __ACTV_BINDER_ENCODE_HANDLE - _IOWR('b', 96, __u64)
 * __ACTV_BINDER_WRITE_READ    - _IOWR('b', 97, struct hmb_write_read)
 * __ACTV_BINDER_SPAWN_ACTV_FRAME - _IO('b', 98)
 * __ACTV_BINDER_REGISTER_ACTVMGR - _IOW('b', 99, struct __actv_binder_actvmgr_args)
 * __ACTV_BINDER_REGISTER_SRVMGR  - _IOW('b', 100, struct __actv_binder_srvmgr_args)
 * __ACTV_BINDER_WAIT_READY_ACTVS - _IOW('b', 101, struct __actv_binder_wait_args)
 */
#define HMB_ACTVMGR_WRITE_READ		__ACTV_BINDER_WRITE_READ
#define HMB_ACTVMGR_SPAWN_ACTV_FRAME	__ACTV_BINDER_SPAWN_ACTV_FRAME
#define HMB_ACTVMGR_WAIT_READY_ACTVS	__ACTV_BINDER_WAIT_READY_ACTVS

/*
 * callnum from 151 ~ 160 for the HMB_ROC_xxx command
 */
#define HMB_ROC_INIT			_IOR('b', 151, struct hmb_roc_init_args)
#define HMB_ROC_REGISTER		_IOW('b', 152, struct hmb_roc_register_args)
#define HMB_ROC_UNREGISTER		_IO('b', 153)
#define HMB_ROC_STOP			_IO('b', 154)
#define HMB_ROC_START			_IO('b', 155)

#define TRANSAC_ASYNC			0x01
#define TRANSAC_ACCEPT_FDS		0x10
#ifdef CONFIG_FSRV_HUAWEI_KSTATE
#define TF_ASYNC_WAKEUP_LATER		0x100000
#endif /* CONFIG_FSRV_HUAWEI_KSTATE */

struct hmb_transaction_data {
	union {
		__u32 bc_handle;
		binderfs_uintptr_t br_addr;
	};
	binderfs_uintptr_t cookie;
	__u32 cmd;
	__u32 flags;
	int caller_pid;
	int caller_euid;
	binderfs_size_t data_size;
	binderfs_size_t offsets_size;
	binderfs_uintptr_t buffer_ptr;
	binderfs_uintptr_t offsets_ptr;
};

struct hmb_transaction_data_secctx {
	struct hmb_transaction_data transaction_data;
	binderfs_uintptr_t secctx;
};

struct hmb_transaction_data_sg {
	struct hmb_transaction_data transaction_data;
	binderfs_size_t buffers_size;
};

struct hmb_addr_cookie {
	binderfs_uintptr_t addr;
	binderfs_uintptr_t cookie;
};

struct hmb_desc_cookie {
	__u32 desc;
	binderfs_uintptr_t cookie;
} __attribute__((packed));

struct hmb_obj {
	union {
		__u32 type;
		struct hmb_object_remote hro;
		struct hmb_object_fd fdo;
		struct hmb_object_buffer bbo;
		struct hmb_object_fd_array fdao;
		struct hmb_object_roc roco;
	};
};

#define to_hmb_object_remote(type) \
	container_of(type, struct hmb_object_remote, type)

#define to_hmb_object_fd(type) container_of(type, struct hmb_object_fd, type)

#define to_hmb_object_buffer(type) \
	container_of(type, struct hmb_object_buffer, type)

#define to_hmb_object_fd_array(type) \
	container_of(type, struct hmb_object_fd_array, type)

#define to_hmb_object_roc(type) \
	container_of(type, struct hmb_object_roc, type)

#define hmb_check_ovfl(b, o) ({				\
	uintptr_t ____b = (uintptr_t)(b);			\
	uintptr_t ____p = (uintptr_t)&(((uint8_t *)(____b))[o]);\
	____b > ____p ? true : false;						\
})

struct release_object_argu {
	binderfs_size_t buffer_off;
	binderfs_size_t off_begin;
	bool need_release;
};

#define	HMBR_ERROR 				_IOR('r', 0, __s32)
#define	HMBR_OK					_IO('r', 1)
#define	HMBR_TRANSACTION_SEC_CTX 		_IOR('r', 2, struct hmb_transaction_data_secctx)
#define	HMBR_TRANSACTION 			_IOR('r', 2, struct hmb_transaction_data)
#define	HMBR_REPLY 				_IOR('r', 3, struct hmb_transaction_data)
#define	HMBR_DEAD_REPLY 			_IO('r', 5)
#define	HMBR_TRANSACTION_COMPLETE		_IO('r', 6)
#define	HMBR_INCREFS				_IOR('r', 7, struct hmb_addr_cookie)
#define	HMBR_ACQUIRE 				_IOR('r', 8, struct hmb_addr_cookie)
#define	HMBR_RELEASE				_IOR('r', 9, struct hmb_addr_cookie)
#define	HMBR_DECREFS 				_IOR('r', 10, struct hmb_addr_cookie)
#define	HMBR_NOOP 				_IO('r', 12)
#define	HMBR_SPAWN_LOOPER			_IO('r', 13)
#define	HMBR_DEAD_BINDER 			_IOR('r', 15, binderfs_uintptr_t)
#define	HMBR_CLEAR_DEATH_NOTIFICATION_DONE	_IOR('r', 16, binderfs_uintptr_t)
#define	HMBR_FAILED_REPLY 			_IO('r', 17)
#define	HMBR_ONEWAY_SPAM_SUSPECT 			_IO('r', 19)
#define	HMBR_TRANSLATION_COMPLETE 		_IOR('r', 20, __u32)


#define	HMBC_TRANSACTION			_IOW('c', 0, struct hmb_transaction_data)
#define	HMBC_REPLY				_IOW('c', 1, struct hmb_transaction_data)
#define	HMBC_FREE_BUFFER			_IOW('c', 3, binderfs_uintptr_t)
#define	HMBC_INCREFS				_IOW('c', 4, __u32)
#define	HMBC_ACQUIRE				_IOW('c', 5, __u32)
#define	HMBC_RELEASE				_IOW('c', 6, __u32)
#define	HMBC_DECREFS				_IOW('c', 7, __u32)
#define	HMBC_INCREFS_DONE			_IOW('c', 8, struct hmb_addr_cookie)
#define	HMBC_ACQUIRE_DONE			_IOW('c', 9, struct hmb_addr_cookie)
#define	HMBC_REGISTER_LOOPER			_IO('c', 11)
#define	HMBC_ENTER_LOOPER			_IO('c', 12)
#define	HMBC_EXIT_LOOPER			_IO('c', 13)
#define	HMBC_REQUEST_DEATH_NOTIFICATION		_IOW('c', 14, struct hmb_desc_cookie)
#define	HMBC_CLEAR_DEATH_NOTIFICATION		_IOW('c', 15, struct hmb_desc_cookie)
#define	HMBC_DEAD_BINDER_DONE			_IOW('c', 16, binderfs_uintptr_t)
#define	HMBC_TRANSACTION_SG			_IOW('c', 17, struct hmb_transaction_data_sg)
#define	HMBC_REPLY_SG				_IOW('c', 18, struct hmb_transaction_data_sg)
#define	HMBC_TRANSLATION				_IOW('c', 21, struct hmb_object_remote)

/*
 * callnum from 96 ~ 127 for the __ACTV_BC_xxx command
 *
 * __ACTV_BC_NONE - _IO('c', 96)
 */
#define HMBC_ACTVMGR_NONE			__ACTV_BC_NONE
#define HMBC_ACTVMGR_ERROR			__ACTV_BC_ERROR

#endif
