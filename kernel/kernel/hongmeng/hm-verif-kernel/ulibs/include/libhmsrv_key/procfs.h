/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: keymgr procfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue July 25 12:10:24 2023
 */
#ifndef LIBHMSRV_PROCFS_H
#define LIBHMSRV_PROCFS_H

typedef enum {
	KEY_PROCFS_GC_DELAY = 0,
	KEY_PROCFS_MAXBYTES,
	KEY_PROCFS_MAXKEYS,
	KEY_PROCFS_ROOT_MAXBYTES,
	KEY_PROCFS_ROOT_MAXKEYS,
	KEY_PROCFS_INVAL
} key_procfs_file_id;

int hm_key_procfs_read(rref_t keymgr_rref, int file_id, char *buf, size_t len);
int hm_key_procfs_write(rref_t keymgr_rref, int file_id, const char *buf, size_t len);
int hm_key_procfs_read_by_name(rref_t keymgr_rref, const char *filename, char *buf, size_t len);
int hm_key_procfs_write_by_name(rref_t keymgr_rref, const char *filename, const char *buf, size_t len);

#endif
