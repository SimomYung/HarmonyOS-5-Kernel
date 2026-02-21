/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of table interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 1 16:28:08 2021
 */

#ifndef __TABLE_H__
#define __TABLE_H__

#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <devhost/transfs.h>
#include <stddef.h>

#ifdef __aarch64__
#define DATA_HOLDER ((void *)0x7fffffffffffffff)
#else
#define DATA_HOLDER ((void *)0x7fffffff)
#endif

#define TABLE_EXPAND_SIZE 512
#define MAX_FS_NAME_LEN 16

struct table_ctx {
	struct raw_rwlock table_lock;
	unsigned int table_size;
	unsigned int table_used_size;
	void **datas;
};

struct trans_sb_tables {
	struct super_block *sb;
	struct vfsmount *vmnt;
	size_t per_write_size;
	struct devhost_transfs_ops *ops;
	void *data;
	struct table_ctx *inode_table;
	struct table_ctx *dentry_table;
	struct table_ctx *file_table;
	/* support reverse query process */
	bool s_reverse_query;
};

struct trans_sb_info {
	struct raw_rwlock sb_tables_lock;
	unsigned int sb_tables_size;
	unsigned int sb_tables_used_size;
	struct trans_sb_tables *sb_tables;
};

#define  __SB_INFO_INITIALIZER(sb) \
		{ .sb_tables_lock = RAW_RWLOCK_INITIALIZER(sb.sb_tables_lock), \
		  .sb_tables_size = 0, \
		  .sb_tables_used_size = 0, \
		  .sb_tables = NULL }

struct lnxbase_ctx;
int dh_extension_init(struct lnxbase_ctx *ctx);

/* sb table ops */
int transfs_sb_table_init(void);
int alloc_sb_to_table(void);
int trans_del_sb_by_index(unsigned int sb_index, const struct super_block *sb,
			  const struct vfsmount *vmnt,
			  const struct devhost_transfs_ops *ops);
struct vfsmount *get_vmnt_by_index(unsigned int sb_index);
struct super_block *get_sb_by_index(unsigned int sb_index);
void *get_data_by_sb_index(unsigned int sb_index);
void set_sb_to_table(unsigned int sb_index, const struct transfs_mounted_ret *mount_ret,
		     size_t per_write_size, bool s_reverse_query);
void release_sb_to_table(unsigned int sb_index);
struct devhost_transfs_ops *get_ops_by_index(unsigned int sb_index);
size_t get_per_write_size_by_index(unsigned int sb_index);
int get_sb_index_and_d_index(struct super_block *sb, unsigned int *sb_index,
			      void *dentry, unsigned int *d_index);

#define declare_table_str_helpers(name)							\
int alloc_##name##_to_table(unsigned int sb_index);					\
int del_##name##_by_index(unsigned int sb_index, unsigned int table_index);		\
struct name *get_##name##_by_index(unsigned int sb_index, unsigned int table_index);	\
void release_##name##_to_table(unsigned int sb_index, unsigned int table_index);	\
void set_##name##_to_table(unsigned int sb_index, unsigned int trans_index,		\
			   struct name *item);

declare_table_str_helpers(inode)
declare_table_str_helpers(dentry)
declare_table_str_helpers(file)
struct dentry *get_dentry_by_index_ref(unsigned int sb_index, unsigned int trans_dentry_index,
				       transfs_dget_t transfs_dget);

#endif /* __TABLE_H__ */
