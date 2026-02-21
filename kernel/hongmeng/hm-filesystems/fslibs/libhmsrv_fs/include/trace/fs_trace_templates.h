/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: fs tracepoint templates
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 9 11:26:54 2024
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#ifndef TRACE_FS_TRACE_TEMPLATES_H
#define TRACE_FS_TRACE_TEMPLATES_H

#define TRACE_VN_DENTRY(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry), \
	TP_ARGS(vn, dentry), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
	), \
	TP_PRINT("ino:%lu name:%s", __entry->ino, __get_str(name)) \
)

#define TRACE_VN_DENTRY_RES(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, int , err), \
	TP_ARGS(vn, dentry, err), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(int, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->res = err; \
	), \
	TP_PRINT("ino:%lu name:%s res:%d", __entry->ino, __get_str(name), __entry->res) \
)

#define TRACE_VN_DENTRY_FLAG(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, unsigned int, flag), \
	TP_ARGS(vn, dentry, flag), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(unsigned int, flag) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->flag = flag; \
	), \
	TP_PRINT("ino:%lu name:%s flag:%u", __entry->ino, __get_str(name), __entry->flag) \
)

#define TRACE_VN_DENTRY_FLAG_RES(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, unsigned int, flag, int, err), \
	TP_ARGS(vn, dentry, flag, err), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(unsigned int, flag) \
		__field(int, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->flag = flag; \
		__entry->res = err; \
	), \
	TP_PRINT("ino:%lu name:%s flag:%u res:%d", __entry->ino, __get_str(name), __entry->flag, __entry->res) \
)

#define TRACE_VN_DENTRY_MODE(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, mode_t, mode), \
	TP_ARGS(vn, dentry, mode), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(mode_t, mode) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->mode = mode; \
	), \
	TP_PRINT("ino:%lu name:%s mode:%u", __entry->ino, __get_str(name), __entry->mode) \
)

#define TRACE_VN_DENTRY_MODE_RES(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, mode_t, mode, int, res), \
	TP_ARGS(vn, dentry, mode, res), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(mode_t, mode) \
		__field(int, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->mode = mode; \
		__entry->res = res; \
	), \
	TP_PRINT("ino:%lu name:%s mode:%u res:%d", __entry->ino, __get_str(name), __entry->mode, __entry->res) \
)

#define TRACE_OLD_NEW_VN_DENTRY_FLAG(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, oldvn, struct dentry *, olddentry, vfs_node_t *, newvn, struct dentry *, newdentry, \
		unsigned int , flags), \
	TP_ARGS(oldvn, olddentry, newvn, newdentry, flags), \
	TP_FIELD( \
		__field(unsigned long, old_ino) \
		__string(oldname, dentry_name(olddentry)) \
		__field(unsigned long, new_ino) \
		__string(newname, dentry_name(newdentry)) \
		__field(unsigned int, flags) \
	), \
	TP_ASSIGN( \
		__entry->old_ino = 0; \
		__entry->new_ino = 0; \
		if (oldvn != NULL) { \
			__entry->old_ino= oldvn->index; \
		} \
		if (newvn != NULL) { \
			__entry->new_ino= newvn->index; \
		} \
		__assign_str(oldname, dentry_name(olddentry)); \
		__assign_str(newname, dentry_name(newdentry)); \
		__entry->flags = flags; \
	), \
	TP_PRINT("old_ino:%lu oldname:%s new_ino:%lu newname:%s, flags:%u", __entry->old_ino, __get_str(oldname), \
		__entry->new_ino, __get_str(newname), __entry->flags) \
)

#define TRACE_OLD_NEW_VN_DENTRY_FLAG_RES(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, oldvn, struct dentry *, olddentry, vfs_node_t *, newvn, struct dentry *, newdentry, \
		unsigned int , flags, int, res), \
	TP_ARGS(oldvn, olddentry, newvn, newdentry, flags, res), \
	TP_FIELD( \
		__field(unsigned long, old_ino) \
		__string(oldname, dentry_name(olddentry)) \
		__field(unsigned long, new_ino) \
		__string(newname, dentry_name(newdentry)) \
		__field(unsigned int, flags) \
		__field(int, res) \
	), \
	TP_ASSIGN( \
		__entry->old_ino = 0; \
		__entry->new_ino = 0; \
		if (oldvn != NULL) { \
			__entry->old_ino= oldvn->index; \
		} \
		if (newvn != NULL) { \
			__entry->new_ino= newvn->index; \
		} \
		__assign_str(oldname, dentry_name(olddentry)); \
		__assign_str(newname, dentry_name(newdentry)); \
		__entry->flags = flags; \
		__entry->res = res; \
	), \
	TP_PRINT("old_ino:%lu oldname:%s new_ino:%lu newname:%s, flags:%u res:%d", __entry->old_ino, __get_str(oldname), \
		__entry->new_ino, __get_str(newname), __entry->flags, __entry->res) \
)

#define TRACE_VN_DENTRY_SIZE(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, size_t, size), \
	TP_ARGS(vn, dentry, size), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(size_t, size) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->size = size; \
	), \
	TP_PRINT("ino:%lu name:%s size:%d", __entry->ino, __get_str(name), __entry->size) \
)

#define TRACE_VN_DENTRY_SIZE_SSIZE(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, size_t, size, ssize_t, res), \
	TP_ARGS(vn, dentry, size, res), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(size_t, size) \
		__field(ssize_t, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->size = size; \
		__entry->res = res; \
	), \
	TP_PRINT("ino:%lu name:%s size:%d res:%ld", __entry->ino, __get_str(name), __entry->size, __entry->res) \
)

#define TRACE_VN_DENTRY_CMD_ARG(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, unsigned int, cmd, unsigned long, arg), \
	TP_ARGS(vn, dentry, cmd, arg), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(unsigned int, cmd) \
		__field(unsigned long, arg) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->cmd = cmd; \
		__entry->arg = arg; \
	), \
	TP_PRINT("ino:%lu name:%s cmd:%u arg:%lu", \
		__entry->ino, __get_str(name), __entry->cmd, __entry->arg) \
)

#define TRACE_VN_DENTRY_CMD_ARG_RES(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, unsigned int, cmd, unsigned long, arg, int, res), \
	TP_ARGS(vn, dentry, cmd, arg, res), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(unsigned int, cmd) \
		__field(unsigned long, arg) \
		__field(int, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->cmd = cmd; \
		__entry->arg = arg; \
		__entry->res = res; \
	), \
	TP_PRINT("ino:%lu name:%s cmd:%u arg:%lu res:%d", \
		__entry->ino, __get_str(name), __entry->cmd, __entry->arg, __entry->res) \
)

#define TRACE_VN_DENTRY_SSIZE(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, ssize_t, res), \
	TP_ARGS(vn, dentry, res), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(ssize_t, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->res = res; \
	), \
	TP_PRINT("ino:%lu name:%s res:%ld", \
		__entry->ino, __get_str(name), __entry->res) \
)

#define TRACE_VN_DENTRY_OFFSET_WHENCE_RES(trace_class_, trace_name_) \
TRACE_EVENT(trace_class_, trace_name_, \
	TP_PROTO(vfs_node_t *, vn, struct dentry *, dentry, off_t, offset, int, whence, off_t, res), \
	TP_ARGS(vn, dentry, offset, whence, res), \
	TP_FIELD( \
		__field(unsigned long, ino) \
		__string(name, dentry_name(dentry)) \
		__field(off_t, offset) \
		__field(int, whence) \
		__field(off_t, res) \
	), \
	TP_ASSIGN( \
		__entry->ino = 0; \
		if (vn != NULL) { \
			__entry->ino = vn->index; \
		} \
		__assign_str(name, dentry_name(dentry)); \
		__entry->offset = offset; \
		__entry->whence = whence; \
		__entry->res = res; \
	), \
	TP_PRINT("ino:%lu name:%s offset:%ld whence:%d ret:%ld", \
		__entry->ino, __get_str(name), __entry->offset, __entry->whence, __entry->res) \
)

#endif
