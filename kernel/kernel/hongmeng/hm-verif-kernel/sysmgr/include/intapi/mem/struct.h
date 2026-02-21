/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Useful structs for memory system
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 03 20:04:48 2021
 */
#ifndef INTAPI_MEM_STRUCT_H
#define INTAPI_MEM_STRUCT_H

#include <hmkernel/capability.h>
/*
 * System server that provides IPC calls for user processes
 * memory system depends, such as VFS, devhost, etc.
 */
struct sys_srv_s {
	cref_t srv_cref; /* cref of the sys server */
	rref_t ipc_rref; /* rref of IPC to the server */
};

#endif /* INTAPI_MEM_STRUCT_H */
