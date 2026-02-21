/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_GARBAGE_DUMP_TYPES_H
#define VSYNC_GARBAGE_DUMP_TYPES_H

#include <vsync/smr/internal/gdump_list.h>
#include <vsync/smr/internal/smr_nodes_list.h>
#include <vsync/smr/internal/smr_lock.h>

#ifndef VGDUMP_MAX_SYNCERS
#define VGDUMP_MAX_SYNCERS 5U
#endif

/* gd toolkit object */
typedef struct gdump_s {
	gdump_list_t threads; /* list of registered threads */
	vatomicptr_t sync_cache[VGDUMP_MAX_SYNCERS]; /* a channel to communicate
                                                with sync(ers) */
	smr_nodes_list_t del_lst; /* list of retired nodes */
	smr_rwlock_lib_t lock;
} VSYNC_CACHEALIGN gdump_t;

typedef struct gdump_thread_s {
	gdump_list_node_t lst_node; /* keep this field as the first field DO NOT
                         MOVE AROUND! The address of the field must
                         equal the address of the container object */
#if defined(SMR_GDUMPV4)
	vatomic8_t status;
	vatomic64_t timestamp;
#else
	vatomic64_t ts_status;
#endif

#if defined(VGDUMP_ENABLE_BUFF)
	smr_node_buff_t buff;
#endif
	vsize_t enter_count;
} gdump_thread_t;

#endif
