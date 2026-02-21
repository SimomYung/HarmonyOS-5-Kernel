/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ext4 tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 7 15:30:54 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

TRACE_EVENT(ext4, ext4_sync_file_enter,
	TP_PROTO(struct dentry *, dentry),
	TP_ARGS(dentry),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(unsigned long, parent)
		__field(int, datasync)
	),
	TP_ASSIGN(
		if (dentry == NULL) {
			__entry->dev = 0;
			__entry->ino = 0;
			__entry->parent = 0;
		} else {
			__entry->dev = dentry->d_sb != NULL ? dentry->d_sb->s_dev_id : 0;
			__entry->ino = dentry->d_inode != NULL ? dentry->d_inode->index : 0;
			__entry->parent = (dentry->d_parent != NULL && dentry->d_parent->d_inode != NULL) ?
					  dentry->d_parent->d_inode->index : 0;
		}
		__entry->datasync = 0;
	),
	TP_PRINT("dev %d,%d ino %lu parent %lu datasync %d ",
		((unsigned int) ((__entry->dev) >> 20)), ((unsigned int) ((__entry->dev) & ((1U << 20) - 1))),
		(unsigned long) __entry->ino, (unsigned long) __entry->parent, __entry->datasync)
)

TRACE_EVENT(ext4, ext4_sync_file_exit,
	TP_PROTO(vfs_node_t *, vn, int, err),
	TP_ARGS(vn, err),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(int, ret)
	),
	TP_ASSIGN(
		if (vn == NULL) {
			__entry->dev = 0;
			__entry->ino = 0;
		} else {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->ret = err;
	),
	TP_PRINT("dev %d,%d ino %lu ret %d",
		((unsigned int) ((__entry->dev) >> 20)), ((unsigned int) ((__entry->dev) & ((1U << 20) - 1))),
		(unsigned long) __entry->ino, __entry->ret)
)

TRACE_EVENT(ext4, ext4_da_write_begin,
	TP_PROTO(vfs_node_t *, vn, uint64_t, posi, size_t, size),
	TP_ARGS(vn, posi, size),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(long long, pos)
		__field(unsigned int, len)
		__field(unsigned int, flags)
	),
	TP_ASSIGN(
		if (vn == NULL) {
			__entry->dev = 0;
			__entry->ino = 0;
		} else {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->pos = (long long)posi;
		__entry->len = (unsigned int)size;
		__entry->flags = 0;
	),
	TP_PRINT("dev %d,%d ino %lu pos %lld len %u flags %u",
		((unsigned int) ((__entry->dev) >> 20)), ((unsigned int) ((__entry->dev) & ((1U << 20) - 1))),
		(unsigned long) __entry->ino, __entry->pos, __entry->len, __entry->flags)
)

TRACE_EVENT(ext4, ext4_da_write_end,
	TP_PROTO(vfs_node_t *, vn, uint64_t, posi, size_t, size),
	TP_ARGS(vn, posi, size),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(long long, pos)
		__field(unsigned int, len)
		__field(unsigned int, copied)
	),
	TP_ASSIGN(
		if (vn == NULL) {
			__entry->dev = 0;
			__entry->ino = 0;
		} else {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->pos = (long long)posi;
		__entry->len = (unsigned int)size;
		__entry->copied = 0;
	),
	TP_PRINT("dev %d,%d ino %lu pos %lld len %u copied %u",
		((unsigned int) ((__entry->dev) >> 20)), ((unsigned int) ((__entry->dev) & ((1U << 20) - 1))),
		(unsigned long) __entry->ino, __entry->pos, __entry->len, __entry->copied)
)
