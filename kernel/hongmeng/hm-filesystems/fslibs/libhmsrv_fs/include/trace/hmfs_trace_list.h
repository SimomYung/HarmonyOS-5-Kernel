/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: hmfs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 22 15:30:54 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#include <trace/hmfs_trace_templates.h>

TRACE_EVENT(hmfs, hmfs_sync_file_exit,
	TP_PROTO(vfs_node_t *, vn, int, res),
	TP_ARGS(vn, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, size)
		__field(mode_t, mode)
		__field(int, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
			__entry->size = vn->size;
			__entry->mode = vn->mode;
		}
		__entry->res = res;
	),
	TP_PRINT("dev=%d,%d ino=%lu i_mode=0x%hx size=%llu res=%d",
		show_dev(__entry->dev), __entry->ino, __entry->mode, __entry->size, __entry->res)
)

/* NEGO: add DECLARE_EVENT_CLASS func, extract common parts */
TRACE_EVENT(hmfs, hmfs_write_enter,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, pid_t, pid, uint64_t, i_size, struct dentry *, dentry),
	TP_ARGS(vn, off, size, pid, i_size, dentry),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(pid_t, pid)
		__field(uint64_t, i_size)
		__string(name, dentry_name(dentry))
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->pid = pid;
		__entry->i_size = i_size;
		__assign_str(name, dentry_name(dentry));
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd pid=%d i_size=%llu entry_name=%s",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size,
		__entry->pid, __entry->i_size, __get_str(name))
)

TRACE_EVENT(hmfs, hmfs_write_exit,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, ssize_t, res),
	TP_ARGS(vn, off, size, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(ssize_t, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->res = res;
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd res=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size, __entry->res)
)

TRACE_EVENT(hmfs, hmfs_write_iter_enter,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size),
	TP_ARGS(vn, off, size),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size)
)

TRACE_EVENT(hmfs, hmfs_write_iter_exit,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, ssize_t, res),
	TP_ARGS(vn, off, size, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(ssize_t, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->res = res;
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd res=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size, __entry->res)
)

TRACE_EVENT(hmfs, hmfs_read_enter,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, pid_t, pid, uint64_t, i_size, struct dentry *, dentry),
	TP_ARGS(vn, off, size, pid, i_size, dentry),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(pid_t, pid)
		__field(uint64_t, i_size)
		__string(name, dentry_name(dentry))
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->pid = pid;
		__entry->i_size = i_size;
		__assign_str(name, dentry_name(dentry));
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd pid=%d i_size=%llu entry_name=%s",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size,
		__entry->pid, __entry->i_size, __get_str(name))
)

TRACE_EVENT(hmfs, hmfs_read_exit,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, ssize_t, res),
	TP_ARGS(vn, off, size, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(ssize_t, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->res = res;
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd res=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size, __entry->res)
)

TRACE_EVENT(hmfs, hmfs_read_iter_enter,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size),
	TP_ARGS(vn, off, size),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size)
)

TRACE_EVENT(hmfs, hmfs_read_iter_exit,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, ssize_t, res),
	TP_ARGS(vn, off, size, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(ssize_t, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->res = res;
	),
	TP_PRINT("dev=%d,%d ino=%lu offset=%llu size=%zd res=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->off, __entry->size, __entry->res)
)

TRACE_EVENT(hmfs, hmfs_new_vnode,
	TP_PROTO(vfs_node_t *, vn, int, res),
	TP_ARGS(vn, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(uint64_t, size)
		__field(mode_t, mode)
		__field(int, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
			__entry->size = vn->size;
			__entry->mode = vn->mode;
		}
		__entry->res = res;
	),
	TP_PRINT("dev=%d,%d ino=%lu i_mode=0x%hx size=%llu res=%d",
		show_dev(__entry->dev), __entry->ino, __entry->mode, __entry->size, __entry->res)
)

#define HMFS_TRACE_XATTR_NAME_LEN 64
#define HMFS_TRACE_XATTR_VAL_LEN 64

TRACE_EVENT(hmfs, hmfs_getxattr,
	TP_PROTO(const vfs_node_t *, vn, int, type, const char *, name, size_t, nsize, size_t, bufsize),
	TP_ARGS(vn, type, name, nsize, bufsize),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(int, type)
		__array(char, name, HMFS_TRACE_XATTR_NAME_LEN)
		__field(size_t, nsize)
		__field(size_t, bufsize)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->type = type;
		__entry->nsize = nsize;
		__entry->bufsize = bufsize;
		strncpy_s(__entry->name, HMFS_TRACE_XATTR_NAME_LEN, name, HMFS_TRACE_XATTR_NAME_LEN - 1);
	),
	TP_PRINT("dev=%d,%d ino=%lu type=%d name=%s nsize=%zd bufsize=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->type, __entry->name, __entry->nsize, __entry->bufsize)
)

TRACE_EVENT(hmfs, hmfs_getxattr_exit,
	TP_PROTO(const vfs_node_t *, vn, int, type, const char *, name, int, res),
	TP_ARGS(vn, type, name, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(int, type)
		__array(char, name, HMFS_TRACE_XATTR_NAME_LEN)
		__field(int, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->type = type;
		strncpy_s(__entry->name, HMFS_TRACE_XATTR_NAME_LEN, name, HMFS_TRACE_XATTR_NAME_LEN - 1);
		__entry->res = res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, type = %d, name = %s, res = %d",
		show_dev(__entry->dev), __entry->ino, __entry->type, __entry->name, __entry->res)
)

TRACE_EVENT(hmfs, hmfs_setxattr,
	TP_PROTO(const vfs_node_t *, vn, int, type, const char *, name, const char *, val, size_t, vsize),
	TP_ARGS(vn, type, name, val, vsize),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, ino)
		__field(int, type)
		__array(char, name, HMFS_TRACE_XATTR_NAME_LEN)
		__array(char, val, HMFS_TRACE_XATTR_VAL_LEN)
		__field(size_t, vsize)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->ino = vn->index;
		}
		__entry->type = type;
		__entry->vsize = vsize;
		strncpy_s(__entry->name, HMFS_TRACE_XATTR_NAME_LEN, name, HMFS_TRACE_XATTR_NAME_LEN - 1);
		strncpy_s(__entry->val, HMFS_TRACE_XATTR_VAL_LEN, val, HMFS_TRACE_XATTR_VAL_LEN - 1);
	),
	TP_PRINT("dev=%d,%d ino=%lu type=%d name=%s val=%s vsize=%zd",
		show_dev(__entry->dev), __entry->ino, __entry->type, __entry->name, __entry->val, __entry->vsize)
)

TRACE_EVENT(hmfs, hmfs_iget,
	TP_PROTO(vfs_node_t *, vn, unsigned long, pino, unsigned char, advise, bool, is_cache_hit),
	TP_ARGS(vn, pino, advise, is_cache_hit),
	TP_FIELD(
		__field(dev_t,	dev)
		__field(unsigned long,	index)
		__field(unsigned long,	pino)
		__field(mode_t, mode)
		__field(long long,	size)
		__field(unsigned int, nlink)
		__field(unsigned long long, blocks)
		__field(unsigned char,	advise)
		__string(cache_status, "MISS")
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->pino	= pino;
		__entry->mode	= vn->mode;
		__entry->nlink	= vn->lnkcnt;
		__entry->size	= vn->size;
		__entry->blocks	= vn->blocks;
		__entry->advise	= advise;
		if (is_cache_hit) {
			__assign_str(cache_status, "HIT");
		} else {
			__assign_str(cache_status, "MISS");
		}
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, pino = %lu, mode = 0x%hx, "
		"size = %lld, linkcnt = %u, blocks = %llu, advise = 0x%x, "
		"cache %s",
		show_dev_index(__entry),
		(unsigned long)__entry->pino,
		__entry->mode,
		__entry->size,
		(unsigned int)__entry->nlink,
		(unsigned long long)__entry->blocks,
		(unsigned char)__entry->advise,
		__get_str(cache_status))
)

TRACE_EVENT(hmfs, hmfs_lookup_start,

	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, bool, inline_dots),

	TP_ARGS(vn, dentry, inline_dots),

	TP_FIELD(
		__field(dev_t,	dev)
		__field(unsigned long,	index)
		__string(name,	dentry_name(dentry))
		__string(inline_status, "FALSE")
	),

	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__assign_str(name, dentry_name(dentry));
		if (inline_dots) {
			__assign_str(inline_status, "TRUE");
		} else {
			__assign_str(inline_status, "FALSE");
		}
	),

	TP_PRINT("dev = (%d,%d), ino = %lu, name = %s, has_inline_dots = %s",
		show_dev_index(__entry),
		__get_str(name),
		__get_str(inline_status))
)

TRACE_EVENT(hmfs, hmfs_lookup_end,

	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, unsigned long, ino, int, res),

	TP_ARGS(vn, dentry, ino, res),

	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(unsigned long,	cino)
		__field(int,		res)
		__string(name,		dentry_name(dentry))
	),

	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->cino = ino;
		__entry->res = res;
		__assign_str(name, dentry_name(dentry));
	),

	TP_PRINT("dev = (%d,%d), pino = %lu, name:%s, ino:%lu, err:%d",
		show_dev_index(__entry),
		__get_str(name),
		__entry->cino,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_readpages,

	TP_PROTO(vfs_node_t *, vn, unsigned long, start, unsigned int, nrpage),

	TP_ARGS(vn, start, nrpage),

	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(unsigned long,	start)
		__field(unsigned int,	nrpage)
	),

	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->start	= start;
		__entry->nrpage	= nrpage;
	),

	TP_PRINT("dev = (%d,%d), ino = %lu, start = %lu nrpage = %u",
		show_dev_index(__entry),
		__entry->start,
		__entry->nrpage)
)

TRACE_EVENT(hmfs, hmfs_writepages,
	TP_PROTO(vfs_node_t *, vn, struct writeback_control *, wbc, bool, skipped, int, type),
	TP_ARGS(vn, wbc, skipped, type),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__string(pgtype, 	MAX_BLOCK_TYPE_TEMPLATE)
		__field(long long,	range_start)
		__field(long long,	range_end)
		__field(long,		nr_to_write)
		__string(wbctype,	MAX_FSCACHE_TYPE_TEMPLATE)
		__string(dir, 		MAX_FILE_MODE_TEMPLATE)
		__string(skip_status, "FALSE")
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
			ASIGN_FILE_MODE(dir, S_ISDIR(vn->mode));
		}
		ASIGN_BLOCK_TYPE(pgtype, type);
		__entry->range_start	= wbc->start_idx;
		__entry->range_end	= wbc->end_idx;
		__entry->nr_to_write	= wbc->nr_to_write;
		ASIGN_FSCACHE_TYPE_TEMP(wbctype, wbc->type);
		if (skipped) {
			__assign_str(skip_status, "TRUE");
		} else {
			__assign_str(skip_status, "FALSE");
		}
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, type = %s, %s, nr_to_write %ld, "
		"skipped %s, start %lld, end %lld, wbc_type %s",
		show_dev_index(__entry),
		__get_str(pgtype),
		__get_str(dir),
		__entry->nr_to_write,
		__get_str(skip_status),
		__entry->range_start,
		__entry->range_end,
		__get_str(wbctype))
)

TRACE_EVENT(hmfs, hmfs_direct_IO_enter,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, unsigned char, rw),
	TP_ARGS(vn, off, size, rw),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, index)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(unsigned char, rw)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->rw = rw;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu offset = %lld size = %lu rw = %hhu",
		show_dev_index(__entry),
		__entry->off,
		__entry->size,
		__entry->rw)
)

TRACE_EVENT(hmfs, hmfs_direct_IO_exit,
	TP_PROTO(vfs_node_t *, vn, uint64_t, off, size_t, size, unsigned char, rw, ssize_t, res),
	TP_ARGS(vn, off, size, rw, res),
	TP_FIELD(
		__field(dev_t, dev)
		__field(unsigned long, index)
		__field(uint64_t, off)
		__field(size_t, size)
		__field(unsigned char, rw)
		__field(ssize_t, res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->off = off;
		__entry->size = size;
		__entry->rw = rw;
		__entry->res = res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu offset = %lld size = %lu rw = %hhu res = %zd",
		show_dev_index(__entry),
		__entry->off,
		__entry->size,
		__entry->rw,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_map_blocks,
	TP_PROTO(vfs_node_t *, vn, struct hmfs_map_info *, map, int, res),
	TP_ARGS(vn, map, res),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(unsigned long,		index)
		__field(unsigned long long, 	pblk)
		__field(unsigned long long,	lblk)
		__field(unsigned int,		len)
		__field(unsigned int,		flag)
		__field(int,			chu_type)
		__field(bool, 			create)
		__field(long long, 		res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->pblk 		= map->pblk;
		__entry->lblk 		= map->lblk;
		__entry->len 		= map->len;
		__entry->flag 		= map->flag;
		__entry->chu_type 	= map->chu_type;
		__entry->create 	= map->create;
		__entry->res 		= res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, file offset = %llu, "
		"start blkaddr = 0x%llx, len = 0x%llx, flag = %u,"
		"seg_type = %d, may_create = %d, err = %lld",
		show_dev_index(__entry),
		(unsigned long long)__entry->lblk,
		(unsigned long long)__entry->pblk,
		(unsigned long long)__entry->len,
		__entry->flag,
		__entry->chu_type,
		__entry->create,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_submit_buffers_write,
	TP_PROTO(fscache_page_t *, page, struct hmfs_io_info *, hio),
	TP_ARGS(page, hio),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(ino_t,			index)
		__field(unsigned long,		page_index)
		__field(unsigned long long,	old_blkaddr)
		__field(unsigned long long,	new_blkaddr)
		__string(hop,			MAX_FS_IO_OP_TEMPLATE)
		__string(tptype,		MAX_TEMP_TYPE_TEMPLATE)
		__string(pgtype, 		MAX_BLOCK_TYPE_TEMPLATE)
	),
	TP_ASSIGN(
		if (page != NULL) {
			if (FSCACHE_NODE(PAGE_MAPPING(page)) != NULL) {
				__entry->dev = FSCACHE_NODE(PAGE_MAPPING(page))->i_sb != NULL ?
							FSCACHE_NODE(PAGE_MAPPING(page))->i_sb->s_dev_id : 0;
				__entry->index = FSCACHE_NODE(PAGE_MAPPING(page))->index;
			}
			__entry->page_index = PAGE_INDEX(page);
		}
		ASIGN_BLOCK_TYPE(pgtype, hio->type);
		ASIGN_TEMP_TYPE(tptype, hio->temp);
		ASIGN_FS_IO_OP_TYPE(hop, show_bio_op(hio->op));
		__entry->old_blkaddr = hio->old_blkaddr;
		__entry->new_blkaddr = hio->new_blkaddr;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, page_index = 0x%lx, oldaddr = 0x%llx,"
		" newaddr = 0x%llx, rw = %s, type = %s_%s",
		show_dev_index(__entry),
		(unsigned long)__entry->page_index,
		(unsigned long long)__entry->old_blkaddr,
		(unsigned long long)__entry->new_blkaddr,
		__get_str(hop),
		__get_str(tptype),
		__get_str(pgtype))
)

#define HMFS_TRACE_COMMIT_MSG_LEN 64

TRACE_EVENT(hmfs, hmfs_start_commit,
	TP_PROTO(struct super_block *, sb, int, reason, const char *, msg),
	TP_ARGS(sb, reason, msg),
	TP_FIELD(
		__field(dev_t,	dev)
		__field(int,	reason)
		__array(char, 	msg, HMFS_TRACE_COMMIT_MSG_LEN)
	),
	TP_ASSIGN(
		__entry->dev	= sb->s_dev_id;
		__entry->reason	= reason;
		strncpy_s(__entry->msg, HMFS_TRACE_COMMIT_MSG_LEN, msg, HMFS_TRACE_COMMIT_MSG_LEN - 1);
	),
	TP_PRINT("dev = (%d,%d), checkpoint for 0x%lx, state = %s",
		show_dev(__entry->dev),
		__entry->reason,
		__entry->msg)
)

TRACE_EVENT(hmfs, hmfs_gc_begin,
	TP_PROTO(struct super_block *, sb, bool, sync, bool, background,
		long long, dirty_nodes, long long, dirty_dents, long long, dirty_imeta,
		unsigned int, free_sec, unsigned int, free_chu,
		int, reserved_chu, unsigned int, prefree_chu),
	TP_ARGS(sb, sync, background, dirty_nodes, dirty_dents, dirty_imeta,
		free_sec, free_chu, reserved_chu, prefree_chu),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(bool,		sync)
		__field(bool,		background)
		__field(long long,	dirty_nodes)
		__field(long long,	dirty_dents)
		__field(long long,	dirty_imeta)
		__field(unsigned int,	free_sec)
		__field(unsigned int,	free_chu)
		__field(int,		reserved_chu)
		__field(unsigned int,	prefree_chu)
	),
	TP_ASSIGN(
		__entry->dev		= sb->s_dev_id;
		__entry->sync		= sync;
		__entry->background	= background;
		__entry->dirty_nodes	= dirty_nodes;
		__entry->dirty_dents	= dirty_dents;
		__entry->dirty_imeta	= dirty_imeta;
		__entry->free_sec	= free_sec;
		__entry->free_chu	= free_chu;
		__entry->reserved_chu	= reserved_chu;
		__entry->prefree_chu	= prefree_chu;
	),
	TP_PRINT("dev = (%d,%d), sync = %d, background = %d, nodes = %lld, "
		"dents = %lld, imeta = %lld, free_sec:%u, free_chu:%u, "
		"rsv_chu:%d, prefree_chu:%u",
		show_dev(__entry->dev),
		__entry->sync,
		__entry->background,
		__entry->dirty_nodes,
		__entry->dirty_dents,
		__entry->dirty_imeta,
		__entry->free_sec,
		__entry->free_chu,
		__entry->reserved_chu,
		__entry->prefree_chu)

)

TRACE_EVENT(hmfs, hmfs_gc_end,
	TP_PROTO(struct super_block *, sb, int, res, int, chu_freed, int, sec_freed,
		long long, dirty_nodes, long long, dirty_dents, long long, dirty_imeta,
		unsigned int, free_sec, unsigned int, free_chu,
		int, reserved_chu, unsigned int, prefree_chu),
	TP_ARGS(sb, res, chu_freed, sec_freed, dirty_nodes, dirty_dents, dirty_imeta,
		free_sec, free_chu, reserved_chu, prefree_chu),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(int,		res)
		__field(int,		sec_freed)
		__field(int,		chu_freed)
		__field(long long,	dirty_nodes)
		__field(long long,	dirty_dents)
		__field(long long,	dirty_imeta)
		__field(unsigned int,	free_sec)
		__field(unsigned int,	free_chu)
		__field(int,		reserved_chu)
		__field(unsigned int,	prefree_chu)
	),
	TP_ASSIGN(
		__entry->dev		= sb->s_dev_id;
		__entry->res		= res;
		__entry->sec_freed	= sec_freed;
		__entry->chu_freed	= chu_freed;
		__entry->dirty_nodes	= dirty_nodes;
		__entry->dirty_dents	= dirty_dents;
		__entry->dirty_imeta	= dirty_imeta;
		__entry->free_sec	= free_sec;
		__entry->free_chu	= free_chu;
		__entry->reserved_chu	= reserved_chu;
		__entry->prefree_chu	= prefree_chu;
	),
	TP_PRINT("dev = (%d,%d), res = %d, sec_freed = %d, chu_freed = %d, nodes = %lld, "
		"dents = %lld, imeta = %lld, free_sec:%u, free_chu:%u, "
		"rsv_chu:%d, prefree_chu:%u",
		show_dev(__entry->dev),
		__entry->res,
		__entry->sec_freed,
		__entry->chu_freed,
		__entry->dirty_nodes,
		__entry->dirty_dents,
		__entry->dirty_imeta,
		__entry->free_sec,
		__entry->free_chu,
		__entry->reserved_chu,
		__entry->prefree_chu)
)

TRACE_EVENT(hmfs, hmfs_grading_ssr_allocate,
	TP_PROTO(struct super_block *, sb, uint64_t, blk_cnt, unsigned int, free_chunks, int, contig),
	TP_ARGS(sb, blk_cnt, free_chunks, contig),
	TP_FIELD(
		__field(dev_t, dev)
		__field(uint64_t, blk_cnt)
		__field(unsigned int, free_chunks)
		__field(int, contig)
	),
	TP_ASSIGN(
		__entry->dev = sb->s_dev_id;
		__entry->blk_cnt = blk_cnt;
		__entry->free_chunks = free_chunks;
		__entry->contig = contig;
	),
	TP_PRINT("dev = (%d,%d), ssr: left_space = %lu free_chunks = %u is_seq = %d",
		show_dev(__entry->dev),
		__entry->blk_cnt,
		__entry->free_chunks,
		__entry->contig)
)

TRACE_EVENT_VNODE_TEMPLATE(hmfs, hmfs_evict_vnode)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_readpage)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_writepage)

TRACE_EVENT_BIO_TEMPLATE(hmfs, hmfs_submit_read_io)

TRACE_EVENT_BIO_TEMPLATE(hmfs, hmfs_submit_write_io)

TRACE_EVENT_BIO_TEMPLATE(hmfs, hmfs_bread)

TRACE_EVENT_BIO_TEMPLATE(hmfs, hmfs_bread_async)

TRACE_EVENT_DISCARD_TEMPLATE(hmfs, hmfs_issue_discard)

TRACE_EVENT_DISCARD_TEMPLATE(hmfs, hmfs_queue_discard)

TRACE_EVENT_DISCARD_TEMPLATE(hmfs, hmfs_remove_discard)

TRACE_EVENT(hmfs, shrink_zombie_list,
	TP_PROTO(struct super_block *, sb, unsigned int, node_cnt,
						unsigned int, tree_cnt),
	TP_ARGS(sb, node_cnt, tree_cnt),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned int,	node_cnt)
		__field(unsigned int,	tree_cnt)
	),
	TP_ASSIGN(
		__entry->dev		= sb->s_dev_id;
		__entry->node_cnt	= node_cnt;
		__entry->tree_cnt	= tree_cnt;
	),
	TP_PRINT("dev = (%d,%d), shrunk: node_cnt = %u, tree_cnt = %u",
		show_dev(__entry->dev),
		__entry->node_cnt,
		__entry->tree_cnt)
)

TRACE_EVENT(hmfs, hmfs_lookup_extent_cache_start,
	TP_PROTO(vfs_node_t *, vn, unsigned int, pgofs),
	TP_ARGS(vn, pgofs),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(unsigned int,	pgofs)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->pgofs		= pgofs;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, pgofs = %u",
		show_dev_index(__entry),
		__entry->pgofs)
)

TRACE_EVENT(hmfs, hmfs_lookup_extent_cache_end,
	TP_PROTO(vfs_node_t *, vn, unsigned int, fofs, __u32, blk,
		unsigned int, ei_len, unsigned int, pgofs),
	TP_ARGS(vn, fofs, blk, ei_len, pgofs),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(unsigned int,	fofs)
		__field(__u32,		blk)
		__field(unsigned int,	ei_len)
		__field(unsigned int,	pgofs)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->fofs		= fofs;
		__entry->blk		= blk;
		__entry->ei_len		= ei_len;
		__entry->pgofs		= pgofs;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, ext_info(fofs: %u, "
		"blk: %u, len: %u), pgofs = %u",
		show_dev_index(__entry),
		__entry->fofs,
		__entry->blk,
		__entry->ei_len,
		__entry->pgofs)
)

TRACE_EVENT(hmfs, hmfs_mmap_exit,
	TP_PROTO(vfs_node_t *, vn, uint64_t, ext_flags, int, res),
	TP_ARGS(vn, ext_flags, res),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(mode_t, 	mode)
		__field(uint64_t,	ext_flags)
		__field(int,		res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->mode		= vn->mode;
		__entry->ext_flags	= ext_flags;
		__entry->res		= res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, mode = 0x%hx, ext_flags = 0x%hx, ret = %d",
		show_dev_index(__entry),
		__entry->mode,
		__entry->ext_flags,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_update_extent_tree_range,
	TP_PROTO(vfs_node_t *, vn, unsigned int, pgofs, block_t, blkaddr,
							unsigned int, et_len),
	TP_ARGS(vn, pgofs, blkaddr, et_len),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(unsigned int,	pgofs)
		__field(__u32,		blk)
		__field(unsigned int,	et_len)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->pgofs		= pgofs;
		__entry->blk		= blkaddr;
		__entry->et_len		= et_len;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, pgofs = %u, "
					"blkaddr = %u, len = %u",
		show_dev_index(__entry),
		__entry->pgofs,
		__entry->blk,
		__entry->et_len)
)

TRACE_EVENT_SHRINK_EXTENT_TREE_TEMPLATE(hmfs, hmfs_shrink_read_extent_tree)

TRACE_EVENT_DESTROY_EXTENT_TREE_TEMPLATE(hmfs, hmfs_destroy_extent_tree)

TRACE_EVENT(hmfs, hmfs_background_gc,
	TP_PROTO(struct super_block *, sb, unsigned int, wait_ms,
					unsigned int, prefree, unsigned int, free),
	TP_ARGS(sb, wait_ms, prefree, free),
	TP_FIELD(
		__field(dev_t,	dev)
		__field(unsigned int,	wait_ms)
		__field(unsigned int,	prefree)
		__field(unsigned int,	free)
	),
	TP_ASSIGN(
		__entry->dev		= sb->s_dev_id;
		__entry->wait_ms	= wait_ms;
		__entry->prefree	= prefree;
		__entry->free		= free;
 	),
	TP_PRINT("dev = (%d,%d), wait_ms = %u, prefree = %u, free = %u",
		show_dev(__entry->dev),
		__entry->wait_ms,
		__entry->prefree,
		__entry->free)
)

TRACE_EVENT(hmfs, hmfs_link_enter,
	TP_PROTO(vfs_node_t *, vn, struct dentry *, old_dentry, struct dentry *, new_dentry),
	TP_ARGS(vn, old_dentry, new_dentry),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(unsigned long,		index)
		__field(uint64_t, 		size)
		__field(unsigned long long, 	blocks)
		__string(old_name,		dentry_name(old_dentry))
		__string(new_name,		dentry_name(new_dentry))
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->size 	= vn->size;
		__entry->blocks = vn->blocks;
		__assign_str(old_name, dentry_name(old_dentry));
		__assign_str(new_name, dentry_name(new_dentry));
	),
	TP_PRINT("dev = (%d,%d), dir ino = %lu, size = %lld, blocks = %llu,"
		" old_name = %s, new_name = %s",
		show_dev_index(__entry),
		__entry->size,
		__entry->blocks,
		__get_str(old_name),
		__get_str(new_name))
)

TRACE_EVENT(hmfs, hmfs_symlink_enter,
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry),
	TP_ARGS(vn, dentry),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(unsigned long,		index)
		__string(d_name,		dentry_name(dentry))
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__assign_str(d_name, dentry_name(dentry));
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, dentry_name = %s",
		show_dev_index(__entry),
		__get_str(d_name))
)

#define HMFS_TRACE_SYMLINK_NAME_LEN 256

TRACE_EVENT(hmfs, hmfs_symlink_exit,
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, const char *, symlink_name,
		int, res),
	TP_ARGS(vn, dentry, symlink_name, res),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__string(d_name,	dentry_name(dentry))
		__array(char, s_name, 	HMFS_TRACE_SYMLINK_NAME_LEN)
		__field(int,		res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__assign_str(d_name, dentry_name(dentry));
		strncpy_s(__entry->s_name, HMFS_TRACE_SYMLINK_NAME_LEN,
			symlink_name, HMFS_TRACE_SYMLINK_NAME_LEN-1);
		__entry->res = res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, dentry_name = %s, symlink_name = %s, ret = %d",
		show_dev_index(__entry),
		__get_str(d_name),
		__entry->s_name,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_fiemap,
	TP_PROTO(vfs_node_t *, vn, unsigned long long, lblock, unsigned long long,
		pblock, unsigned long long, size, unsigned int, flags, int, res),
	TP_ARGS(vn, lblock, pblock, size, flags, res),
	TP_FIELD(
		__field(dev_t, 			dev)
		__field(unsigned long, 		index)
		__field(unsigned long long, 	lblock)
		__field(unsigned long long, 	pblock)
		__field(unsigned long long, 	size)
		__field(unsigned int, 		flags)
		__field(int, 			res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->lblock	= lblock;
		__entry->pblock	= pblock;
		__entry->size	= size;
		__entry->flags	= flags;
		__entry->res	= res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, lblock:%lld, pblock:%lld,"
		" len:%llu, flags:0x%hx, ret:%d",
		show_dev_index(__entry),
		(unsigned long long)__entry->lblock,
		(unsigned long long)__entry->pblock,
		__entry->size,
		__entry->flags,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_truncate_enter,
	TP_PROTO(vfs_node_t *, vn, unsigned long, pino, unsigned char, advise),
	TP_ARGS(vn, pino, advise),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(unsigned long,		index)
		__field(unsigned long,		pino)
		__field(mode_t, 		mode)
		__field(uint64_t,		size)
		__field(unsigned int, 		nlink)
		__field(unsigned long long, 	blocks)
		__field(unsigned char,		advise)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->pino	= pino;
		__entry->mode	= vn->mode;
		__entry->nlink	= vn->lnkcnt;
		__entry->size	= vn->size;
		__entry->blocks	= vn->blocks;
		__entry->advise	= advise;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, pino = %lu, mode = 0x%hx, "
		"size = %lld, linkcnt = %u, blocks = %llu, advise = 0x%x",
		show_dev_index(__entry),
		(unsigned long)__entry->pino,
		__entry->mode,
		__entry->size,
		(unsigned int)__entry->nlink,
		(unsigned long long)__entry->blocks,
		(unsigned char)__entry->advise)
)

TRACE_EVENT(hmfs, hmfs_readdir,
	TP_PROTO(vfs_node_t *, vn, uint64_t, start_pos, uint64_t, end_pos, int, res),
	TP_ARGS(vn, start_pos, end_pos, res),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(ino_t,		index)
		__field(uint64_t,	start_pos)
		__field(uint64_t,	end_pos)
		__field(int,		res)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->start_pos 	= start_pos;
		__entry->end_pos 	= end_pos;
		__entry->res 		= res;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, start_pos:%llu, end_pos:%llu, err:%d",
		show_dev_index(__entry),
		__entry->start_pos,
		__entry->end_pos,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_getattr_enter,
	TP_PROTO(vfs_node_t *, vn, uint64_t, ext_flags),
	TP_ARGS(vn, ext_flags),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(unsigned long,		index)
		__field(mode_t, 		mode)
		__field(uint64_t,		size)
		__field(unsigned long long, 	blocks)
		__field(unsigned int, 		nlink)
		__field(uint64_t,		ext_flags)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->mode		= vn->mode;
		__entry->size		= vn->size;
		__entry->blocks		= vn->blocks;
		__entry->nlink		= vn->lnkcnt;
		__entry->ext_flags	= ext_flags;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, mode = 0x%hx, "
		"size = %lld, blocks = %llu, linkcnt = %u, ext_flags = 0x%hx",
		show_dev_index(__entry),
		__entry->mode,
		__entry->size,
		(unsigned long long)__entry->blocks,
		__entry->nlink,
		__entry->ext_flags)
)

TRACE_EVENT(hmfs, hmfs_setattr_enter,
	TP_PROTO(vfs_node_t *, vn, struct vattr *, attr),
	TP_ARGS(vn, attr),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(mode_t, 	mode)
		__field(mode_t, 	attr_mode)
		__field(uint64_t,	attr_size)
		__field(uid_t, 		attr_uid)
		__field(gid_t, 		attr_gid)
		__field(unsigned int,	attr_valid)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
			__entry->mode = vn->mode;
		}
		__entry->attr_mode	= attr->mode;
		__entry->attr_size	= attr->size;
		__entry->attr_uid	= attr->uid;
		__entry->attr_gid	= attr->gid;
		__entry->attr_valid	= attr->valid;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, mode = 0x%hx, attr_mode = 0x%hx,"
		" attr_size = %lld, attr_uid = %u, attr_gid = %u, attr_valid = %u",
		show_dev_index(__entry),
		__entry->mode,
		__entry->attr_mode,
		__entry->attr_size,
		__entry->attr_uid,
		__entry->attr_gid,
		__entry->attr_valid)
)

TRACE_EVENT(hmfs, hmfs_open_enter,
	TP_PROTO(vfs_node_t *, vn, unsigned int, f_modes, uint64_t, ext_flags),
	TP_ARGS(vn, f_modes, ext_flags),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(mode_t, 	mode)
		__field(unsigned int, 	v_flags)
		__field(unsigned int,	f_modes)
		__field(uint64_t, 	ext_flags)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->mode		= vn->mode;
		__entry->v_flags	= vn->v_flags;
		__entry->f_modes	= f_modes;
		__entry->ext_flags	= ext_flags;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, mode = 0x%hx, v_flags = 0x%hx,"
		" f_modes = 0x%hx, ext_flags = 0x%hx",
		show_dev_index(__entry),
		__entry->mode,
		__entry->v_flags,
		__entry->f_modes,
		__entry->ext_flags)
)

#define HMFS_TRACE_RD_INFO 32

TRACE_EVENT(hmfs, hmfs_get_node_block,
	TP_PROTO(struct super_block *, sb, uint32_t, nid, uint32_t, footer_ino,
		uint32_t, footer_nid, const char *, rd_info, const char *, ra_info),
	TP_ARGS(sb, nid, footer_ino, footer_nid, rd_info, ra_info),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(uint32_t,	nid)
		__field(uint32_t,	footer_ino)
		__field(uint32_t,	footer_nid)
		__array(char,		rd_info, HMFS_TRACE_RD_INFO)
		__array(char,		ra_info, HMFS_TRACE_RD_INFO)
	),
	TP_ASSIGN(
		__entry->dev 		= sb->s_dev_id;
		__entry->nid 		= nid;
		__entry->footer_ino 	= footer_ino;
		__entry->footer_nid 	= footer_nid;
		strncpy_s(__entry->rd_info, HMFS_TRACE_RD_INFO, rd_info, HMFS_TRACE_RD_INFO - 1);
		strncpy_s(__entry->ra_info, HMFS_TRACE_RD_INFO, ra_info, HMFS_TRACE_RD_INFO - 1);
	),
	TP_PRINT("dev = (%d,%d), nid = %u, footer_ino = %u,"
		" footer_nid = %u, rd_info = %s, ra_info = %s",
		show_dev(__entry->dev),
		__entry->nid,
		__entry->footer_ino,
		__entry->footer_nid,
		__entry->rd_info,
		__entry->ra_info)
)

TRACE_EVENT(hmfs, hmfs_rename_enter,
	TP_PROTO(vfs_node_t *, old_vn, struct dentry *, old_dentry,
		vfs_node_t *, new_vn, struct dentry *, new_dentry),
	TP_ARGS(old_vn, old_dentry, new_vn, new_dentry),
	TP_FIELD(
		__field(dev_t,			old_dev)
		__field(unsigned long,		old_index)
		__string(old_dname,		dentry_name(old_dentry))
		__field(dev_t,			new_dev)
		__field(unsigned long,		new_index)
		__string(new_dname,		dentry_name(new_dentry))
	),
	TP_ASSIGN(
		if (old_vn != NULL) {
			__entry->old_dev = old_vn->i_sb != NULL ? old_vn->i_sb->s_dev_id : 0;
			__entry->old_index = old_vn->index;
		}
		__assign_str(old_dname, dentry_name(old_dentry));
		if (new_vn != NULL) {
			__entry->new_dev = new_vn->i_sb != NULL ? new_vn->i_sb->s_dev_id : 0;
			__entry->new_index = new_vn->index;
		}
		__assign_str(new_dname, dentry_name(new_dentry));
	),
	TP_PRINT("old_dev = (%d,%d), old_ino = %lu, old_dentry_name = %s,"
		" new_dev = (%d,%d), new_ino = %lu, new_dentry_name = %s",
		show_dev(__entry->old_dev),
		(unsigned long)__entry->old_index,
		__get_str(old_dname),
		show_dev(__entry->new_dev),
		(unsigned long)__entry->new_index,
		__get_str(new_dname))
)

TRACE_EVENT(hmfs, hmfs_rename_exit,
	TP_PROTO(vfs_node_t *, old_vn, struct dentry *, old_dentry,
		vfs_node_t *, new_vn, struct dentry *, new_dentry, int, res),
	TP_ARGS(old_vn, old_dentry, new_vn, new_dentry, res),
	TP_FIELD(
		__field(dev_t,			old_dev)
		__field(unsigned long,		old_index)
		__string(old_dname,		dentry_name(old_dentry))
		__field(dev_t,			new_dev)
		__field(unsigned long,		new_index)
		__string(new_dname,		dentry_name(new_dentry))
		__field(int, 			res)
	),
	TP_ASSIGN(
		if (old_vn != NULL) {
			__entry->old_dev = old_vn->i_sb != NULL ? old_vn->i_sb->s_dev_id : 0;
			__entry->old_index = old_vn->index;
		}
		__assign_str(old_dname, dentry_name(old_dentry));
		if (new_vn != NULL) {
			__entry->new_dev = new_vn->i_sb != NULL ? new_vn->i_sb->s_dev_id : 0;
			__entry->new_index = new_vn->index;
		}
		__assign_str(new_dname, dentry_name(new_dentry));
		__entry->res = res;
	),
	TP_PRINT("old_dev = (%d,%d), old_ino = %lu, old_dentry_name = %s,"
		" new_dev = (%d,%d), new_ino = %lu, new_dentry_name = %s, ret = %d",
		show_dev(__entry->old_dev),
		(unsigned long)__entry->old_index,
		__get_str(old_dname),
		show_dev(__entry->new_dev),
		(unsigned long)__entry->new_index,
		__get_str(new_dname),
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_get_node_info_enter,
	TP_PROTO(struct super_block *, sb, uint32_t, nid),
	TP_ARGS(sb, nid),
	TP_FIELD(
		__field(dev_t, 		dev)
		__field(uint32_t, 	nid)
	),
	TP_ASSIGN(
		__entry->dev = sb->s_dev_id;
		__entry->nid = nid;
	),
	TP_PRINT("dev = (%d,%d), nid = %u",
		show_dev(__entry->dev),
		__entry->nid)
)

TRACE_EVENT(hmfs, hmfs_get_node_info_exit,
	TP_PROTO(struct super_block *, sb, uint32_t, nid, int, res),
	TP_ARGS(sb, nid, res),
	TP_FIELD(
		__field(dev_t, 		dev)
		__field(uint32_t, 	nid)
		__field(int, 		res)
	),
	TP_ASSIGN(
		__entry->dev = sb->s_dev_id;
		__entry->nid = nid;
		__entry->res = res;
	),
	TP_PRINT("dev = (%d,%d), nid = %u, ret = %d",
		show_dev(__entry->dev),
		__entry->nid,
		__entry->res)
)

TRACE_EVENT(hmfs, hmfs_listxattr_enter,
	TP_PROTO(vfs_node_t *, vn, uint32_t, xattr_nid, uint64_t, size),
	TP_ARGS(vn, xattr_nid, size),
	TP_FIELD(
		__field(dev_t,			dev)
		__field(unsigned long,		index)
		__field(mode_t, 		mode)
		__field(uint32_t, 		xattr_nid)
		__field(uint64_t,		size)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
			__entry->mode = vn->mode;
		}
		__entry->xattr_nid	= xattr_nid;
		__entry->size		= size;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, mode = 0x%hx, xattr_nid = %d, size = %lld",
		show_dev_index(__entry),
		__entry->mode,
		__entry->xattr_nid,
		__entry->size)
)

TRACE_EVENT(hmfs, hmfs_mmap_enter,
	TP_PROTO(vfs_node_t *, vn, const struct vfs_mmap_create_info *, info, uint64_t, ext_flags,
		unsigned int, f_flags, unsigned int, f_modes),
	TP_ARGS(vn, info, ext_flags, f_flags, f_modes),
	TP_FIELD(
		__field(dev_t,		dev)
		__field(unsigned long,	index)
		__field(mode_t, 	mode)
		__field(unsigned long,	mmap_fd_file)
		__field(int64_t,	mmap_offset)
		__field(int64_t,	mmap_len)
		__field(uint32_t,	mmap_cnode_idx)
		__field(uint32_t,	mmap_prot)
		__field(uint32_t,	mmap_flags)
		__field(uint64_t,	ext_flags)
		__field(unsigned int,	f_flags)
		__field(unsigned int,	f_modes)
	),
	TP_ASSIGN(
		if (vn != NULL) {
			__entry->dev = vn->i_sb != NULL ? vn->i_sb->s_dev_id : 0;
			__entry->index = vn->index;
		}
		__entry->mode		= vn->mode;
		__entry->mmap_fd_file	= info->fd_file;
		__entry->mmap_offset	= info->offset;
		__entry->mmap_len	= info->len;
		__entry->mmap_cnode_idx	= info->cnode_idx;
		__entry->mmap_prot	= info->prot;
		__entry->mmap_flags	= info->flags;
		__entry->ext_flags	= ext_flags;
		__entry->f_flags	= f_flags;
		__entry->f_modes	= f_modes;
	),
	TP_PRINT("dev = (%d,%d), ino = %lu, mode = 0x%hx, mmap_fd_file = %lu,"
		"mmap_offset = %lld, mmap_len = %lld, mmap_cnode_idx = %ld,"
		" mmap_prot = 0x%hx, mmap_flags = 0x%hx, ext_flags = 0x%hx,"
		" f_flags = 0x%hx, f_modes = 0x%hx",
		show_dev_index(__entry),
		__entry->mode,
		__entry->mmap_fd_file,
		__entry->mmap_offset,
		__entry->mmap_len,
		__entry->mmap_cnode_idx,
		__entry->mmap_prot,
		__entry->mmap_flags,
		__entry->ext_flags)
)

TRACE_EVENT(hmfs, hmfs_sync,
	TP_PROTO(struct super_block *, sb),
	TP_ARGS(sb),
	TP_FIELD(
		__field(dev_t,	dev)
		__field(int,	dirty)
	),
	TP_ASSIGN(
		__entry->dev	= sb->s_dev_id;
		__entry->dirty	= is_sbi_flag_set(HMFS_SB(sb), SBI_IS_DIRTY);
	),
	TP_PRINT("dev = (%d,%d), superblock is %s",
		show_dev(__entry->dev),
		__entry->dirty ? "dirty" : "not dirty")
)

TRACE_EVENT_VFS_NODE_T_TEMPLATE(hmfs, hmfs_sync_file_enter)

TRACE_EVENT_LINK_TEMPLATE(hmfs, hmfs_unlink_enter)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_unlink_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_link_exit)

TRACE_EVENT_LINK_TEMPLATE(hmfs, hmfs_rmdir_enter)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_rmdir_exit)

TRACE_EVENT_LINK_TEMPLATE(hmfs, hmfs_mkdir_enter)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_mkdir_exit)

TRACE_EVENT_LINK_TEMPLATE(hmfs, hmfs_create_enter)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_create_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_getattr_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_setattr_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_open_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_listxattr_exit)

TRACE_EVENT_VFS_NODE_T_TEMPLATE(hmfs, hmfs_getlink_enter)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_getlink_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_mark_dirty)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_truncate_exit)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_mmap_dirtypage)

TRACE_EVENT(hmfs, hmfs_get_victim,

	TP_PROTO(struct super_block *, sb, int, type, int, gc_type,
			int, alloc_mode, int, gc_mode, unsigned int, min_segno, unsigned int, min_cost,
			unsigned int, ofs_unit, unsigned int, pre_victim,
			unsigned int, prefree, unsigned int, free_num),
 
	TP_ARGS(sb, type, gc_type, alloc_mode, gc_mode, min_segno, min_cost, ofs_unit,
			pre_victim, prefree, free_num),

	TP_FIELD(
		__field(dev_t,	dev)
		__string(fd_type,	LONGEST_FILE_DATA_TYPE_TEMPLATE)
		__string(fd_gc_type,	LONGEST_FILE_GC_TYPE_TEMPLATE)
		__string(fd_alloc_mode,	LONGEST_FILE_ALLOC_MODE_TEMPLATE)
		__string(fd_gc_mode,	LONGEST_FILE_VICTIM_POLICY_TEMPLATE)
		__field(unsigned int,	victim)
		__field(unsigned int,	cost)
		__field(unsigned int,	ofs_unit)
		__field(unsigned int,	pre_victim)
		__field(unsigned int,	prefree)
		__field(unsigned int,	free_num)
	),

	TP_ASSIGN(
		__entry->dev		= sb->s_dev_id;
		assign_data_type(fd_type, type);
		assign_gc_type(fd_gc_type, gc_type);
		assign_alloc_mode(fd_alloc_mode, alloc_mode);
		assign_victim_policy(fd_gc_mode, gc_mode);
		__entry->victim		= min_segno;
		__entry->cost		= min_cost;
		__entry->ofs_unit	= ofs_unit;
		__entry->pre_victim	= pre_victim;
		__entry->prefree	= prefree;
		__entry->free_num	= free_num;
	),

	TP_PRINT("dev = (%d,%d), type = %s, policy = (%s, %s, %s), "
		"victim = %u, cost = %u, ofs_unit = %u, "
		"pre_victim_secno = %d, prefree = %u, free = %u",
		show_dev(__entry->dev),
		__get_str(fd_type),
		__get_str(fd_gc_type),
		__get_str(fd_alloc_mode),
		__get_str(fd_gc_mode),
		__entry->victim,
		__entry->cost,
		__entry->ofs_unit,
		(int)__entry->pre_victim,
		__entry->prefree,
		__entry->free_num)
)

TRACE_EVENT(hmfs, hmfs_reserve_new_blocks,

	TP_PROTO(vfs_node_t *, vn, uint32_t, nid, unsigned int, ofs_in_node,
							blkcnt_t, count),
 
	TP_ARGS(vn, nid, ofs_in_node, count),

	TP_FIELD(
		__field(dev_t,		dev)
		__field(uint32_t,	nid)
		__field(unsigned int, 	ofs_in_node)
		__field(blkcnt_t, 	count)
	),

	TP_ASSIGN(
		__entry->dev	= vn->i_sb->s_dev_id;
		__entry->nid	= nid;
		__entry->ofs_in_node = ofs_in_node;
		__entry->count = count;
	),

	TP_PRINT("dev = (%d,%d), nid = %u, ofs_in_node = %u, count = %llu",
		show_dev(__entry->dev),
		(unsigned int)__entry->nid,
		__entry->ofs_in_node,
		(unsigned long long)__entry->count)
)

TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE(hmfs, hmfs_sync_dirty_vnodes_enter)

TRACE_EVENT_SYNC_DIRTY_VN_TEMPLATE(hmfs, hmfs_sync_dirty_vnodes_exit)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_iget_exit)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_do_write_data_page)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_set_page_dirty)

TRACE_EVENT_MAPPING_TEMPLATE(hmfs, hmfs_balance_fs)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_releasepage)

TRACE_EVENT_PAGE_TEMPLATE(hmfs, hmfs_cleanpage)

TRACE_EVENT_END_IO_TEMPLATE(hmfs, hmfs_writepages_end_io)

TRACE_EVENT_END_IO_TEMPLATE(hmfs, hmfs_submit_discard_endio)

TRACE_EVENT_END_IO_TEMPLATE(hmfs, hmfs_decompress_and_end_io)

TRACE_EVENT_VNODE_EXIT_TEMPLATE(hmfs, hmfs_sync_exit)

TRACE_EVENT(hmfs, hmfs_fscache_end_pages_io,

	TP_PROTO(dev_t, dev_id, unsigned int, fi_flags, int, fi_status),

	TP_ARGS(dev_id, fi_flags, fi_status),

	TP_FIELD(
		__field(dev_t,		dev_id)
		__field(unsigned int, 	fi_flags)
		__field(int, 		fi_status)
	),

	TP_ASSIGN(
		__entry->dev_id		= dev_id;
		__entry->fi_flags 	= fi_flags;
		__entry->fi_status 	= fi_status;
	),

	TP_PRINT("dev = (%d,%d), fi_flags = %lu, fi_status = %d",
		show_dev(__entry->dev_id),
		__entry->fi_flags,
		__entry->fi_status)
)
