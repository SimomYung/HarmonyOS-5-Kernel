/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Procfs interface for slab dfx
 * Author: Huawei OS Kernel Lab
 * Create: Fri 01 March 2024 10:04
 */
#ifndef PROCFS_SLAB_DEBUG_H
#define PROCFS_SLAB_DEBUG_H

#ifdef CONFIG_VFS_FUSION
#include <intapi/procfs/seq_file.h>

enum slab_dump_content {
	SLAB_DUMP_AMOUNT,
	SLAB_DUMP_TRACE,
	SLAB_DUMP_STUB
};

int slab_usage_fill(struct seq_file_s *seq, enum slab_dump_content content);
int procfs_slab_usage_track_read(char *data, size_t data_len);
#endif
int procfs_slab_track_name_read(char *data, size_t data_len);
int procfs_slab_track_name_write(uint64_t pos, const char *src, size_t size, size_t *wsize);

#endif /* PROCFS_SLAB_DEBUG_H */
