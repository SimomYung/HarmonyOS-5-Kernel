/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: xvm syscall definitions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 5 14:38:30 2024
 */

#ifndef ULIBS_LIBHMPSF_XVM_LSYSCALLS_H
#define ULIBS_LIBHMPSF_XVM_LSYSCALLS_H

#include <stdbool.h>
#include <lib/hmpsf/hmpsf_def.h>

#define XVM_SC_CMD_MODULE_LOAD				0x0 /* arg: pointer to xvm_sc_module_load_args_s */
#define XVM_SC_CMD_MODULE_RELEASE			0x1 /* arg: module id */
#define XVM_SC_CMD_INT_HASH_MAP_CREATE_GS		0x100 /* arg: pointer to xvm_int_hash_map_t */
#define XVM_SC_CMD_INT_HASH_MAP_LOAD			0x101 /* arg: pointer to xvm_sc_int_hash_map_load_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_STORE			0x102 /* arg: pointer to xvm_sc_int_hash_map_store_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_NEXT_KEY		0x103 /* arg: pointer to xvm_sc_int_hash_map_next_key_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_EXCHANGE		0x104 /* arg: pointer to xvm_sc_int_hash_map_exchange_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_COMPARE_EXCHANGE	0x105 /* arg: pointer to xvm_sc_int_hash_map_compare_exchange_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_ADD_FETCH		0x106 /* arg: pointer to xvm_sc_int_hash_map_add_fetch_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_SUB_FETCH		0x107 /* arg: pointer to xvm_sc_int_hash_map_sub_fetch_args_s */
#define XVM_SC_CMD_INT_HASH_MAP_CLEAR			0x108 /* arg: pointer to xvm_sc_int_hash_map_clear_args_s */
#define XVM_SC_CMD_BYTES_HASH_MAP_CREATE_GS		0x200 /* arg: pointer to xvm_sc_bytes_hash_map_t */
#define XVM_SC_CMD_BYTES_HASH_MAP_SET			0x201 /* arg: pointer to xvm_sc_bytes_hash_map_set_args_s */
#define XVM_SC_CMD_BYTES_HASH_MAP_GET			0x202 /* arg: pointer to xvm_sc_bytes_hash_map_get_args_s */
#define XVM_SC_CMD_BYTES_HASH_MAP_NEXT_KEY		0x203 /* arg: pointer to xvm_sc_bytes_hash_map_next_key_args_s */
#define XVM_SC_CMD_BYTES_HASH_MAP_CLEAR			0x204 /* arg: pointer to xvm_sc_bytes_hash_map_clear_args_s */
#define XVM_SC_CMD_MAP_RELEASE_GS			0x301 /* arg: map id */
#define XVM_SC_CMD_PERFBUF_CREATE			0x401 /* arg: pointer to user_pb */
#define XVM_SC_CMD_PERFBUF_ELEM_UPDATE			0x402 /* arg: pointer to xvm_sc_perfbuf_elem_update_args_s */
#define XVM_SC_CMD_PERFBUF_REMOVE			0x403 /* arg: map id */

struct xvm_sc_module_load_args_s {
	struct hmpsf_module_attr_s attr;
	unsigned char *code;
	unsigned long code_size;
};

struct xvm_sc_map_base_args_s {
	unsigned int module_id;
	unsigned int scope;
	unsigned int map_id;
	const void *key;
	unsigned int key_size;
};

struct xvm_sc_int_hash_map_load_args_s {
	struct xvm_sc_map_base_args_s base;
	unsigned long long *val;
};

#define xvm_sc_int_hash_map_add_fetch_args_s xvm_sc_int_hash_map_load_args_s
#define xvm_sc_int_hash_map_sub_fetch_args_s xvm_sc_int_hash_map_load_args_s

struct xvm_sc_int_hash_map_store_args_s {
	struct xvm_sc_map_base_args_s base;
	unsigned long long val;
};

struct xvm_sc_hash_map_next_key_args_s {
	struct xvm_sc_map_base_args_s base;
	void *next_key;
	unsigned int *next_key_size;
};

#define xvm_sc_int_hash_map_next_key_args_s xvm_sc_hash_map_next_key_args_s
#define xvm_sc_bytes_hash_map_next_key_args_s xvm_sc_hash_map_next_key_args_s

struct xvm_sc_int_hash_map_exchange_args_s {
	struct xvm_sc_map_base_args_s base;
	unsigned long long val;
	unsigned long long *old;
};

struct xvm_sc_int_hash_map_compare_exchange_args_s {
	struct xvm_sc_map_base_args_s base;
	unsigned long long *expected;
	unsigned long long desired;
};

struct xvm_sc_hash_map_clear_args_s {
	unsigned int module_id;
	unsigned int scope;
	unsigned int map_id;
};

#define xvm_sc_int_hash_map_clear_args_s xvm_sc_hash_map_clear_args_s
#define xvm_sc_bytes_hash_map_clear_args_s xvm_sc_hash_map_clear_args_s

struct xvm_sc_bytes_hash_map_set_args_s {
	struct xvm_sc_map_base_args_s base;
	const void *val;
	unsigned int val_size;
};

struct xvm_sc_bytes_hash_map_get_args_s {
	struct xvm_sc_map_base_args_s base;
	void *val;
	unsigned int *val_size;
};

struct xvm_sc_perfbuf_elem_update_args_s {
	unsigned int map_id;
	unsigned int cpu;
	int fd;
};

#endif
