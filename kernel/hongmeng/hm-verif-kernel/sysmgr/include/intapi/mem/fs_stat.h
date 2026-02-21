/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Memory statistics for VFS
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 21 12:05:01 2020
 */
#ifndef SYSMGR_MEM_FS_STAT_H
#define SYSMGR_MEM_FS_STAT_H

#include <lib/slist.h>
#include <libmem/mstat.h>
#include <intapi/mem/file.h>
#include <intapi/mem/umem.h>
#include <intapi/mem/vspace.h>
#include <intapi/process/service.h>

struct map_stat_s;

bool fs_stat_is_register(void);
int fs_stat_register(struct vspace_s *vspace, const void *stat_addr);
void fs_stat_deregister(const struct vspace_s *vspace);
void fs_stat_calculate(struct memstat_fs_stat_s *fs_mem_stat);
size_t fs_stat_pages_free(void);
size_t fs_stat_pages_free_active(void);
size_t fs_stat_free_cache(void);

int umem_attach_fsstat(struct vspace_s *vspace, const void *uptr, raw_vfs_stat_t **stat);
int umem_detach_fsstat(raw_vfs_stat_t *fs_stat);
#endif
