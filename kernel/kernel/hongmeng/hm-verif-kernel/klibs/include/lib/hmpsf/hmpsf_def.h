/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 08 20:49:26 2022
 */
#ifndef HMPSF_UTILS_H
#define HMPSF_UTILS_H

#include <hmasm/types.h>

#define HMPSF_MODULE_LOCK_SIZE 64
#define HMPSF_MAX_CODE_SIZE (64 * 256 * PAGE_SIZE) // MEM_64MB

/* memory that sysmgr allocates for the kernel */
#define HMPSF_MAX_PREALLOC_PAGE_ORDER 14
#define HMPSF_MAX_PREALLOC_PAGES (1 << (HMPSF_MAX_PREALLOC_PAGE_ORDER))
#define HMPSF_MAX_PREALLOC_SIZE (1 << (HMPSF_MAX_PREALLOC_PAGE_ORDER + PAGE_SHIFT))

#define HMPSF_MAP_MAX_PAGE_ORDER 14
#define HMPSF_MAP_MAX_SIZE (1 << HMPSF_MAP_MAX_PAGE_ORDER << PAGE_SHIFT)

#define HMPSF_ARRAY_MAP_MAX_VALUE_SIZE 128

/* Phases for proxy load process */
#define HMPSF_PROXY_PHASE 0
#define HMPSF_HOST_PHASE 1

/* supported hmpsf vm types */
enum hmpsf_vm_type {
	HMPSF_T_HMXVM,
	HMPSF_T_TEST,
	HMPSF_T_NUMBER,
};

/* hmpsf running vspace */
enum hmpsf_vspaceid {
	HMPSF_VSPACE_KERNEL,
	HMPSF_VSPACE_SYSMGR,
	HMPSF_VSPACE_BINDERFS,
	HMPSF_VSPACE_NUMBER,
};

enum hmpsf_module_type {
	HMPSF_MODULE_TYPE_UNSPEC,
	HMPSF_MODULE_TYPE_BINDERFS_ROC,
	HMPSF_MODULE_TYPE_TEST,
	HMPSF_MODULE_TYPE_HMPROBE,
	HMPSF_MODULE_TYPE_NUM,
};

/* Supported hmpsf_attach_types */
enum hmpsf_attach_type {
	HMPSF_PROBE_ATTACH_TYPE,
	HMPSF_LLT_ATTACH_TYPE,
	HMPSF_TEST_ATTACH_TYPE,
	__MAX_HMPSF_ATTACH_TYPE
};
#define MAX_HMPSF_ATTACH_TYPE __MAX_HMPSF_ATTACH_TYPE

/*
 * The maximum number of maps that could be bound to a module.
 */
#define HMPSF_MODULE_MAX_MAPS			16

/* hmpsf module attributes, specified by client */
struct hmpsf_module_attr_s {
	__u32 vm_type;
	__u32 vspaceid;
	__u32 module_type;
	unsigned char opt_level;
	/* globally-shared maps to be bound to the module */
	__u32 map_ids[HMPSF_MODULE_MAX_MAPS];
	__u32 map_cnt;
	__u32 exec_type;
};

/* hmpsf attributes for loading to host */
struct hmpsf_module_host_attr_s {
	struct hmpsf_module_attr_s module_attr;

	/* code data will be freed after loading */
	__u64 code;
	__u64 code_size;
};

/*
 * hmpsf attributes for proxy loading.
 *
 * Proxy loading means the sysmgr helps the host do most of the loading procedures and generate an
 * image, and the host loads the image directly. This is like ahead-of-time (AOT) loading for the
 * host. It saves the host troubles of complex loading procedures.
 *
 * Currently only the kernel needs/uses proxy loading. But theoretically it can be used in other
 * hosts as well. So, we separate the attributes for proxy loading and attributes for loading to
 * the kernel.
 */
struct hmpsf_module_proxy_attr_s {
	struct hmpsf_module_host_attr_s host_attr;
	__u64 proxy_loaded_data;
	__u64 proxy_loaded_data_size;
	__u64 total_size;
};

#define HMPSF_TEST_MAX_BUF_SIZE		256
#define HMPSF_TEST_MAX_ARGS		6 /* Calls between xvm and host only supports 6 args. */

enum hmpsf_test_call_val_type {
	HMPSF_TEST_CALL_VAL_I64,
	HMPSF_TEST_CALL_VAL_BUF,
};

struct hmpsf_test_call_val_s {
	enum hmpsf_test_call_val_type type;
	union {
		__u64 integer;
		unsigned char buffer[HMPSF_TEST_MAX_BUF_SIZE];
	} val;
};

struct hmpsf_test_config_s {
	unsigned char opt_level;

	union {
		__u32 module_id;
		__u64 hmpsfmod_kobj;
	};
	struct hmpsf_test_call_entry_func_s {
		char name[HMPSF_TEST_MAX_BUF_SIZE];
	} call_entry;
	struct hmpsf_test_call_args_s {
		struct hmpsf_test_call_val_s args[HMPSF_TEST_MAX_ARGS];
		unsigned char args_num;
	} call_args;
	struct hmpsf_test_call_ret_s {
		struct hmpsf_test_call_val_s ret;
	} call_ret;
};

#endif
