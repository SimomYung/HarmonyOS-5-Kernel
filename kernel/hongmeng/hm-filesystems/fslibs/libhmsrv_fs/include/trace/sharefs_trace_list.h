/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: sharefs tracepoint list
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 09 15:30:54 2023
 *
 * No header guard:
 * This file is a template, so it could be included at many times with
 * different macro definitions in the same file.
 */

#include <trace/fs_trace_templates.h>

TRACE_VN_DENTRY(sharefs, sharefs_getattr_enter)
TRACE_VN_DENTRY_RES(sharefs, sharefs_getattr_exit)
TRACE_VN_DENTRY(sharefs, sharefs_listxattr_enter)
TRACE_VN_DENTRY_SSIZE(sharefs, sharefs_listxattr_exit)
TRACE_VN_DENTRY_OFFSET_WHENCE_RES(sharefs, sharefs_file_llseek_exit)
TRACE_VN_DENTRY(sharefs, sharefs_open_enter)
TRACE_VN_DENTRY_RES(sharefs, sharefs_open_exit)
TRACE_VN_DENTRY(sharefs, sharefs_flush_enter)
TRACE_VN_DENTRY(sharefs, sharefs_flush_exit)
TRACE_VN_DENTRY(sharefs, sharefs_file_release_enter)
TRACE_VN_DENTRY(sharefs, sharefs_file_release_exit)
TRACE_VN_DENTRY(sharefs, sharefs_read_iter_enter)
TRACE_VN_DENTRY_SSIZE(sharefs, sharefs_read_iter_exit)
TRACE_VN_DENTRY(sharefs, sharefs_write_iter_enter)
TRACE_VN_DENTRY_SSIZE(sharefs, sharefs_write_iter_exit)
TRACE_VN_DENTRY(sharefs, sharefs_file_mmap_enter)
TRACE_VN_DENTRY_RES(sharefs, sharefs_file_mmap_exit)
TRACE_VN_DENTRY(sharefs, sharefs_readdir_enter)
TRACE_VN_DENTRY_SSIZE(sharefs, sharefs_readdir_exit)
