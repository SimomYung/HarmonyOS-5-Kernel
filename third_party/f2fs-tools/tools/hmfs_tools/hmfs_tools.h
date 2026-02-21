/**
 * hmfs_tools.h
 *
 * Copyright (C) 2024 Huawei Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _HMFS_TOOLS_H_
#define _HMFS_TOOLS_H_

#include <stdbool.h>

#define HMFS_SUPER_MAGIC 0xFEF52024 /* HMFS Magic Number */

enum convert_type {
	CONVERT_TYPE_NONE,
	CONVERT_TYPE_F2FS,
	CONVERT_TYPE_HMFS,
};

enum compress_algorithm_type {
	COMPRESS_ALGO_LZO,
	COMPRESS_ALGO_LZ4,
	COMPRESS_ALGO_ZSTD,
	COMPRESS_ALGO_LZORLE,
	COMPRESS_ALGO_MAX,
};

bool is_valid_hmfs_magic(unsigned long hmfs_magic);

enum convert_type parse_convert_config(const char *optarg);

void mkfs_hmfs_msg(unsigned int fs_magic);

#endif /* _HMFS_TOOLS_H_ */