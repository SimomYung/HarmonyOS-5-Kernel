/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: hmfs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 22 15:30:54 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#include <trace/fs_trace_templates.h>

TRACE_EVENT(hmdfs, hmdfs_balance_dirty_pages_ratelimited_begin,
		TP_PROTO(fscache_mapping_t *, mapping, unsigned int, retry_count),
		TP_ARGS(mapping, retry_count),
		TP_FIELD(
			__field(unsigned long, i_ino)
			__field(unsigned int, pages_dirty)
			__field(unsigned int, retry_count)
		),
		TP_ASSIGN(
			__entry->i_ino = 0;
			__entry->pages_dirty = 0;
			if (mapping != NULL) {
				__entry->pages_dirty = mapping->pages_dirty;
				if (mapping->host != NULL) {
					__entry->i_ino = mapping->host->index;
				}
			}
			__entry->retry_count = retry_count;
		),
		TP_PRINT("hmdfs: ino=0x%lx pages_dirty=%u, retry_count=%u",
			__entry->i_ino, __entry->pages_dirty, __entry->retry_count)
)

TRACE_EVENT(hmdfs, hmdfs_balance_dirty_pages_ratelimited_end,
		TP_PROTO(fscache_mapping_t *, mapping, unsigned int, retry_count),
		TP_ARGS(mapping, retry_count),
		TP_FIELD(
			__field(unsigned long, i_ino)
			__field(unsigned int, pages_dirty)
			__field(unsigned int, retry_count)
		),
		TP_ASSIGN(
			__entry->i_ino = 0;
			__entry->pages_dirty = 0;
			if (mapping != NULL) {
				__entry->pages_dirty = mapping->pages_dirty;
				if (mapping->host != NULL) {
					__entry->i_ino = mapping->host->index;
				}
			}
			__entry->retry_count = retry_count;
		),
		TP_PRINT("hmdfs: ino=0x%lx pages_dirty=%u, retry_count=%u",
			__entry->i_ino, __entry->pages_dirty, __entry->retry_count)
)

TRACE_EVENT(hmdfs, hmdfs_sync_dirty_pages,
		TP_PROTO(fscache_mapping_t *, mapping),
		TP_ARGS(mapping),
		TP_FIELD(
			__field(unsigned long, i_ino)
			__field(unsigned int, pages_dirty)
		),
		TP_ASSIGN(
			__entry->i_ino = 0;
			__entry->pages_dirty = 0;
			if (mapping != NULL) {
				__entry->pages_dirty = mapping->pages_dirty;
				if (mapping->host != NULL) {
					__entry->i_ino = mapping->host->index;
				}
			}
		),
		TP_PRINT("hmdfs: ino=0x%lx pages_dirty=%u",
			__entry->i_ino, __entry->pages_dirty)
)

TRACE_EVENT(hmdfs, hmdfs_tcp_send_message,
	TP_PROTO(struct hmdfs_head_cmd *, cmd_head),
	TP_ARGS(cmd_head),
	TP_FIELD(
		__field(__u32, id)
		__field(__u32, magic)
		__field(__u16, command)
		__field(__u16, flag)
		__field(__u32, len)
		__field(__u32, ret)
	),
	TP_ASSIGN(
		__entry->id = le32toh(cmd_head->msg_id);
		__entry->magic = cmd_head->magic;
		__entry->command = cmd_head->operations.command;
		__entry->flag = cmd_head->operations.cmd_flag;
		__entry->len = le32toh(cmd_head->data_len);
		__entry->ret = le32toh(cmd_head->ret_code);
	),
	TP_PRINT("msg_id:%u magic:%u command:%hu, cmd_flag:%hu, data_len:%u, ret_code:%u",
		__entry->id, __entry->magic, __entry->command,
		__entry->flag, __entry->len, __entry->ret)
)

TRACE_EVENT(hmdfs, hmdfs_recv_mesg_callback,
	TP_PROTO(struct hmdfs_head_cmd *, cmd_head),
	TP_ARGS(cmd_head),
	TP_FIELD(
		__field(__u32, id)
		__field(__u32, magic)
		__field(__u16, command)
		__field(__u16, flag)
		__field(__u32, len)
		__field(__u32, ret)
	),
	TP_ASSIGN(
		__entry->id = le32toh(cmd_head->msg_id);
		__entry->magic = cmd_head->magic;
		__entry->command = cmd_head->operations.command;
		__entry->flag = cmd_head->operations.cmd_flag;
		__entry->len = le32toh(cmd_head->data_len);
		__entry->ret = le32toh(cmd_head->ret_code);
	),
	TP_PRINT("msg_id:%u magic:%u command:%hu, cmd_flag:%hu, data_len:%u, ret_code:%u",
		__entry->id, __entry->magic, __entry->command,
		__entry->flag, __entry->len, __entry->ret)
)

TRACE_EVENT(hmdfs, hmdfs_setattr_local_enter,
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, struct vattr *, ia),
	TP_ARGS(vn, dentry, ia),
	TP_FIELD(
		__field(unsigned long, ino)
		__string(name, dentry_name(dentry))
		__field(mode_t, mode)
		__field(uid_t, uid)
		__field(gid_t, gid)
	),
	TP_ASSIGN(
		__entry->ino = 0;
		if (vn != NULL) {
			__entry->ino = vn->index;
		}
		__assign_str(name, dentry_name(dentry));
		__entry->mode = ia->mode;
		__entry->uid = ia->uid;
		__entry->gid = ia->gid;
	),
	TP_PRINT("ino:%lu name:%s mode:%u gid:%u uid:%u",
		__entry->ino, __get_str(name), __entry->mode, __entry->gid, __entry->uid)
)

TRACE_EVENT(hmdfs, hmdfs_setattr_local_exit,
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, struct vattr *, ia, int, err),
	TP_ARGS(vn, dentry, ia, err),
	TP_FIELD(
		__field(unsigned long, ino)
		__string(name, dentry_name(dentry))
		__field(mode_t, mode)
		__field(uid_t, uid)
		__field(gid_t, gid)
		__field(int, res)
	),
	TP_ASSIGN(
		__entry->ino = 0;
		if (vn != NULL) {
			__entry->ino = vn->index;
		}
		__assign_str(name, dentry_name(dentry));
		__entry->mode = ia->mode;
		__entry->uid = ia->uid;
		__entry->gid = ia->gid;
		__entry->res = err;
	),
	TP_PRINT("ino:%lu name:%s mode:%u gid:%u uid:%u res:%d",
		__entry->ino, __get_str(name), __entry->mode, __entry->gid, __entry->uid, __entry->res)
)

TRACE_VN_DENTRY(hmdfs, hmdfs_lookup_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_lookup_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_lookup_cloud_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_lookup_cloud_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_open_cloud_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_file_open_cloud_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_mmap_cloud_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_file_mmap_cloud_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_release_cloud_enter)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_release_cloud_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_flush_cloud_enter)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_flush_cloud_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_lookup_cloud_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_lookup_cloud_merge_exit)
TRACE_VN_DENTRY_FLAG(hmdfs, hmdfs_dir_open_cloud_enter)
TRACE_VN_DENTRY_FLAG_RES(hmdfs, hmdfs_dir_open_cloud_exit)
TRACE_VN_DENTRY_FLAG(hmdfs, hmdfs_dir_release_cloud_enter)
TRACE_VN_DENTRY_MODE(hmdfs, hmdfs_mkdir_cloud_merge_enter)
TRACE_VN_DENTRY_MODE_RES(hmdfs, hmdfs_mkdir_cloud_merge_exit)
TRACE_VN_DENTRY_MODE(hmdfs, hmdfs_create_cloud_merge_enter)
TRACE_VN_DENTRY_MODE_RES(hmdfs, hmdfs_create_cloud_merge_exit)
TRACE_OLD_NEW_VN_DENTRY_FLAG(hmdfs, hmdfs_rename_local_enter)
TRACE_OLD_NEW_VN_DENTRY_FLAG_RES(hmdfs, hmdfs_rename_local_exit)
TRACE_OLD_NEW_VN_DENTRY_FLAG(hmdfs, hmdfs_rename_cloud_merge_enter)
TRACE_OLD_NEW_VN_DENTRY_FLAG_RES(hmdfs, hmdfs_rename_cloud_merge_exit)
TRACE_VN_DENTRY_MODE(hmdfs, hmdfs_mkdir_merge_enter)
TRACE_VN_DENTRY_MODE_RES(hmdfs, hmdfs_mkdir_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_lookup_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_lookup_merge_exit)
TRACE_VN_DENTRY_MODE(hmdfs, hmdfs_create_merge_enter)
TRACE_VN_DENTRY_MODE_RES(hmdfs, hmdfs_create_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_rmdir_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_rmdir_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_unlink_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_unlink_merge_exit)
TRACE_OLD_NEW_VN_DENTRY_FLAG(hmdfs, hmdfs_rename_merge_enter)
TRACE_OLD_NEW_VN_DENTRY_FLAG_RES(hmdfs, hmdfs_rename_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_dir_open_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_dir_open_merge_exit)
TRACE_VN_DENTRY_SIZE(hmdfs, hmdfs_iterate_merge_enter)
TRACE_VN_DENTRY_SIZE_SSIZE(hmdfs, hmdfs_iterate_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_dir_release_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_dir_release_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_merge_read_iter_enter)
TRACE_VN_DENTRY_SSIZE(hmdfs, hmdfs_merge_read_iter_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_merge_write_iter_enter)
TRACE_VN_DENTRY_SSIZE(hmdfs, hmdfs_merge_write_iter_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_open_merge_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_file_open_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_flush_merge_enter)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_flush_merge_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_getattr_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_getattr_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_mkdir_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_mkdir_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_create_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_create_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_rmdir_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_rmdir_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_unlink_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_unlink_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_open_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_file_open_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_release_local_enter)
TRACE_VN_DENTRY(hmdfs, hmdfs_file_mmap_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_file_mmap_local_exit)
TRACE_VN_DENTRY_OFFSET_WHENCE_RES(hmdfs, hmdfs_file_llseek_local_exit)
TRACE_VN_DENTRY(hmdfs, hmdfs_symlink_local_enter)
TRACE_VN_DENTRY_RES(hmdfs, hmdfs_symlink_local_exit)
