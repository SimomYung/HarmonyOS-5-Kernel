/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for fslt
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-filesystems/fs/libtransfs/libsysif/api.sysif -I ../kernel/hongmeng/hm-filesystems/fs/libtransfs/libsysif --api-dest ../image/usr/include/generated/sysif_client/fs/libsysif/transfs/api.h --server-dest ../image/usr/include/generated/sysif_server/fs/libsysif/transfs/server.h --legacy-mode
* Create: Fri Oct 31 10:38:41 2025
*/
#ifndef LIBSYSIF_FSLT_API_H
#define LIBSYSIF_FSLT_API_H
#ifndef LIBSYSIF_FSLT_COMMON_H
#define LIBSYSIF_FSLT_COMMON_H
#include <libsysif/utils_common.h>
#include <hongmeng/compiler.h>
#include <hongmeng/types.h>
#include <hmasm/actv_rpc.h>
#ifndef LIBSYSIF_BASE_API_COMMON_H
#define LIBSYSIF_BASE_API_COMMON_H
#include <stddef.h>
#include <stdint.h>
#include <hmkernel/capability.h>
struct sysif_reply_header {
	int32_t retval;
	uint32_t msgsz;
};

#define SYSIF_REPLY_LEN_MAX 256UL
extern void* hm_actvpool_prepare_ret(unsigned long long sender, unsigned long credential, unsigned long size);


extern struct arch_actv_local* hm_actv_current_arch_actv_local(void);


#endif
raw_static_assert(sizeof(dev_t) <= 96,
                    fsltcall_transfs_get_tree_arg_devno_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_get_tree_arg_fs_type_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_get_tree_arg_opts_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_getattr_arg_iattr_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_lookup_arg_name_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_create_arg_name_buf_is_too_large);
raw_static_assert(sizeof(mode_t) <= 96,
                    fsltcall_transfs_create_arg_mode_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_unlink_arg_name_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_mkdir_arg_name_buf_is_too_large);
raw_static_assert(sizeof(mode_t) <= 96,
                    fsltcall_transfs_mkdir_arg_mode_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_rmdir_arg_name_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_rename_arg_old_name_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    fsltcall_transfs_rename_arg_new_name_buf_is_too_large);
struct __actvret_fsltcall_transfs_get_tree {
	unsigned long long ino;
	unsigned long long instance;
};

struct __actvret_fsltcall_transfs_kill_sb {
};

struct __actvret_fsltcall_transfs_getattr {
};

struct __actvret_fsltcall_transfs_setattr {
};

struct __actvret_fsltcall_transfs_readdir {
};

struct __actvret_fsltcall_transfs_lookup {
	unsigned long long ino;
};

struct __actvret_fsltcall_transfs_create {
	unsigned long long ino;
};

struct __actvret_fsltcall_transfs_unlink {
};

struct __actvret_fsltcall_transfs_write_iter {
	unsigned long long new_pos;
};

struct __actvret_fsltcall_transfs_read_iter {
};

struct __actvret_fsltcall_transfs_ioctl {
};

struct __actvret_fsltcall_transfs_mkdir {
	unsigned long long ino;
};

struct __actvret_fsltcall_transfs_rmdir {
};

struct __actvret_fsltcall_transfs_rename {
};

struct __actvret_fsltcall_transfs_statfs {
};

struct __actvret_fsltcall_transfs_show_options {
};

struct __actvret_fsltcall_transfs_listxattr {
};

struct __actvret_fsltcall_transfs_getxattr {
};

struct __actvret_fsltcall_transfs_setxattr {
};

struct __actvret_fsltcall_transfs_destroy_node {
};

struct __actvret_fsltcall_transfs_sync {
};

struct __actvret_fsltcall_transfs_fsync {
};

struct __actvret_fsltcall_transfs_flush {
};

#ifdef __aarch64__
union __actvret_fsltcall {
	struct __actvret_fsltcall_transfs_get_tree transfs_get_tree;
	struct __actvret_fsltcall_transfs_kill_sb transfs_kill_sb;
	struct __actvret_fsltcall_transfs_getattr transfs_getattr;
	struct __actvret_fsltcall_transfs_setattr transfs_setattr;
	struct __actvret_fsltcall_transfs_readdir transfs_readdir;
	struct __actvret_fsltcall_transfs_lookup transfs_lookup;
	struct __actvret_fsltcall_transfs_create transfs_create;
	struct __actvret_fsltcall_transfs_unlink transfs_unlink;
	struct __actvret_fsltcall_transfs_write_iter transfs_write_iter;
	struct __actvret_fsltcall_transfs_read_iter transfs_read_iter;
	struct __actvret_fsltcall_transfs_ioctl transfs_ioctl;
	struct __actvret_fsltcall_transfs_mkdir transfs_mkdir;
	struct __actvret_fsltcall_transfs_rmdir transfs_rmdir;
	struct __actvret_fsltcall_transfs_rename transfs_rename;
	struct __actvret_fsltcall_transfs_statfs transfs_statfs;
	struct __actvret_fsltcall_transfs_show_options transfs_show_options;
	struct __actvret_fsltcall_transfs_listxattr transfs_listxattr;
	struct __actvret_fsltcall_transfs_getxattr transfs_getxattr;
	struct __actvret_fsltcall_transfs_setxattr transfs_setxattr;
	struct __actvret_fsltcall_transfs_destroy_node transfs_destroy_node;
	struct __actvret_fsltcall_transfs_sync transfs_sync;
	struct __actvret_fsltcall_transfs_fsync transfs_fsync;
	struct __actvret_fsltcall_transfs_flush transfs_flush;
};

#elif defined(__arm__)
union __actvret_fsltcall {
	struct __actvret_fsltcall_transfs_get_tree transfs_get_tree;
	struct __actvret_fsltcall_transfs_kill_sb transfs_kill_sb;
	struct __actvret_fsltcall_transfs_getattr transfs_getattr;
	struct __actvret_fsltcall_transfs_setattr transfs_setattr;
	struct __actvret_fsltcall_transfs_readdir transfs_readdir;
	struct __actvret_fsltcall_transfs_lookup transfs_lookup;
	struct __actvret_fsltcall_transfs_create transfs_create;
	struct __actvret_fsltcall_transfs_unlink transfs_unlink;
	struct __actvret_fsltcall_transfs_write_iter transfs_write_iter;
	struct __actvret_fsltcall_transfs_read_iter transfs_read_iter;
	struct __actvret_fsltcall_transfs_ioctl transfs_ioctl;
	struct __actvret_fsltcall_transfs_mkdir transfs_mkdir;
	struct __actvret_fsltcall_transfs_rmdir transfs_rmdir;
	struct __actvret_fsltcall_transfs_rename transfs_rename;
	struct __actvret_fsltcall_transfs_statfs transfs_statfs;
	struct __actvret_fsltcall_transfs_show_options transfs_show_options;
	struct __actvret_fsltcall_transfs_listxattr transfs_listxattr;
	struct __actvret_fsltcall_transfs_getxattr transfs_getxattr;
	struct __actvret_fsltcall_transfs_setxattr transfs_setxattr;
	struct __actvret_fsltcall_transfs_destroy_node transfs_destroy_node;
	struct __actvret_fsltcall_transfs_sync transfs_sync;
	struct __actvret_fsltcall_transfs_fsync transfs_fsync;
	struct __actvret_fsltcall_transfs_flush transfs_flush;
};

#else
union __actvret_fsltcall {
	struct __actvret_fsltcall_transfs_get_tree transfs_get_tree;
	struct __actvret_fsltcall_transfs_kill_sb transfs_kill_sb;
	struct __actvret_fsltcall_transfs_getattr transfs_getattr;
	struct __actvret_fsltcall_transfs_setattr transfs_setattr;
	struct __actvret_fsltcall_transfs_readdir transfs_readdir;
	struct __actvret_fsltcall_transfs_lookup transfs_lookup;
	struct __actvret_fsltcall_transfs_create transfs_create;
	struct __actvret_fsltcall_transfs_unlink transfs_unlink;
	struct __actvret_fsltcall_transfs_write_iter transfs_write_iter;
	struct __actvret_fsltcall_transfs_read_iter transfs_read_iter;
	struct __actvret_fsltcall_transfs_ioctl transfs_ioctl;
	struct __actvret_fsltcall_transfs_mkdir transfs_mkdir;
	struct __actvret_fsltcall_transfs_rmdir transfs_rmdir;
	struct __actvret_fsltcall_transfs_rename transfs_rename;
	struct __actvret_fsltcall_transfs_statfs transfs_statfs;
	struct __actvret_fsltcall_transfs_show_options transfs_show_options;
	struct __actvret_fsltcall_transfs_listxattr transfs_listxattr;
	struct __actvret_fsltcall_transfs_getxattr transfs_getxattr;
	struct __actvret_fsltcall_transfs_setxattr transfs_setxattr;
	struct __actvret_fsltcall_transfs_destroy_node transfs_destroy_node;
	struct __actvret_fsltcall_transfs_sync transfs_sync;
	struct __actvret_fsltcall_transfs_fsync transfs_fsync;
	struct __actvret_fsltcall_transfs_flush transfs_flush;
};

#endif
enum __fslt_mgr {
	__fslt_INVAL_mgr=0,
	__fslt_transfs_mgr,
	__fslt_MAX_mgr,
};

#define __fslt_method_MIN __ACTV_HIGHTABLE_START
#define __fslt_method_transfs_get_tree (__ACTV_HIGHTABLE_START + 1)
#define __fslt_method_transfs_kill_sb (__ACTV_HIGHTABLE_START + 2)
#define __fslt_method_transfs_getattr (__ACTV_HIGHTABLE_START + 3)
#define __fslt_method_transfs_setattr (__ACTV_HIGHTABLE_START + 4)
#define __fslt_method_transfs_readdir (__ACTV_HIGHTABLE_START + 5)
#define __fslt_method_transfs_lookup (__ACTV_HIGHTABLE_START + 6)
#define __fslt_method_transfs_create (__ACTV_HIGHTABLE_START + 7)
#define __fslt_method_transfs_unlink (__ACTV_HIGHTABLE_START + 8)
#define __fslt_method_transfs_write_iter (__ACTV_HIGHTABLE_START + 9)
#define __fslt_method_transfs_read_iter (__ACTV_HIGHTABLE_START + 10)
#define __fslt_method_transfs_ioctl (__ACTV_HIGHTABLE_START + 11)
#define __fslt_method_transfs_mkdir (__ACTV_HIGHTABLE_START + 12)
#define __fslt_method_transfs_rmdir (__ACTV_HIGHTABLE_START + 13)
#define __fslt_method_transfs_rename (__ACTV_HIGHTABLE_START + 14)
#define __fslt_method_transfs_statfs (__ACTV_HIGHTABLE_START + 15)
#define __fslt_method_transfs_show_options (__ACTV_HIGHTABLE_START + 16)
#define __fslt_method_transfs_listxattr (__ACTV_HIGHTABLE_START + 17)
#define __fslt_method_transfs_getxattr (__ACTV_HIGHTABLE_START + 18)
#define __fslt_method_transfs_setxattr (__ACTV_HIGHTABLE_START + 19)
#define __fslt_method_transfs_destroy_node (__ACTV_HIGHTABLE_START + 20)
#define __fslt_method_transfs_sync (__ACTV_HIGHTABLE_START + 21)
#define __fslt_method_transfs_fsync (__ACTV_HIGHTABLE_START + 22)
#define __fslt_method_transfs_flush (__ACTV_HIGHTABLE_START + 23)
#define __fslt_method_MAX (__ACTV_HIGHTABLE_START + 24)
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_get_tree) < 512,
            fsltcall_transfs_get_tree_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_kill_sb) < 512,
            fsltcall_transfs_kill_sb_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_getattr) < 512,
            fsltcall_transfs_getattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_setattr) < 512,
            fsltcall_transfs_setattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_readdir) < 512,
            fsltcall_transfs_readdir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_lookup) < 512,
            fsltcall_transfs_lookup_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_create) < 512,
            fsltcall_transfs_create_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_unlink) < 512,
            fsltcall_transfs_unlink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_write_iter) < 512,
            fsltcall_transfs_write_iter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_read_iter) < 512,
            fsltcall_transfs_read_iter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_ioctl) < 512,
            fsltcall_transfs_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_mkdir) < 512,
            fsltcall_transfs_mkdir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_rmdir) < 512,
            fsltcall_transfs_rmdir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_rename) < 512,
            fsltcall_transfs_rename_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_statfs) < 512,
            fsltcall_transfs_statfs_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_show_options) < 512,
            fsltcall_transfs_show_options_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_listxattr) < 512,
            fsltcall_transfs_listxattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_getxattr) < 512,
            fsltcall_transfs_getxattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_setxattr) < 512,
            fsltcall_transfs_setxattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_destroy_node) < 512,
            fsltcall_transfs_destroy_node_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_sync) < 512,
            fsltcall_transfs_sync_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_fsync) < 512,
            fsltcall_transfs_fsync_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fsltcall_transfs_flush) < 512,
            fsltcall_transfs_flush_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_fsltcall) < 512,
        fsltcall_too_many_actvret_data);
extern int __actvcall_fsltcall_transfs_get_tree(unsigned long long __rpc_info, void *__buf, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);


extern int __actvcapcall_fsltcall_transfs_get_tree(rref_t __rref, unsigned long long __rpc_info, void *__buf, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_get_tree(bool is_xact_fwd, bool is_xact_allow_refwd, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts, struct __actvret_fsltcall_transfs_get_tree *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_get_tree;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_get_tree(rpc_info, __ret, devno, mode, fs_type, opts);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_get_tree(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts, struct __actvret_fsltcall_transfs_get_tree *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_get_tree;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_get_tree(__rref, rpc_info, __ret, devno, mode, fs_type, opts);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_get_tree(dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts, struct __actvret_fsltcall_transfs_get_tree *__ret)
{
	return actvxactcall_fsltcall_transfs_get_tree(false, false, devno, mode, fs_type, opts, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_get_tree(rref_t __rref, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts, struct __actvret_fsltcall_transfs_get_tree *__ret)
{
	return actvxactcapcall_fsltcall_transfs_get_tree(false, false, __rref, devno, mode, fs_type, opts, __ret);
}

extern int __actvcall_fsltcall_transfs_kill_sb(unsigned long long __rpc_info, void *__buf, unsigned long long instance);


extern int __actvcapcall_fsltcall_transfs_kill_sb(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_kill_sb(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance)
{
	const unsigned long __callno = __fslt_method_transfs_kill_sb;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_kill_sb(rpc_info, NULL, instance);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_kill_sb(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance)
{
	const unsigned long __callno = __fslt_method_transfs_kill_sb;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_kill_sb(__rref, rpc_info, NULL, instance);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_kill_sb(unsigned long long instance)
{
	return actvxactcall_fsltcall_transfs_kill_sb(false, false, instance);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_kill_sb(rref_t __rref, unsigned long long instance)
{
	return actvxactcapcall_fsltcall_transfs_kill_sb(false, false, __rref, instance);
}

extern int __actvcall_fsltcall_transfs_getattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);


extern int __actvcapcall_fsltcall_transfs_getattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_getattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	const unsigned long __callno = __fslt_method_transfs_getattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_getattr(rpc_info, NULL, instance, ino, iattr_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_getattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	const unsigned long __callno = __fslt_method_transfs_getattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_getattr(__rref, rpc_info, NULL, instance, ino, iattr_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_getattr(unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	return actvxactcall_fsltcall_transfs_getattr(false, false, instance, ino, iattr_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_getattr(rref_t __rref, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	return actvxactcapcall_fsltcall_transfs_getattr(false, false, __rref, instance, ino, iattr_buf);
}

extern int __actvcall_fsltcall_transfs_setattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *iattr);


extern int __actvcapcall_fsltcall_transfs_setattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *iattr);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_setattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, void *iattr)
{
	const unsigned long __callno = __fslt_method_transfs_setattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_setattr(rpc_info, NULL, instance, ino, iattr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_setattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, void *iattr)
{
	const unsigned long __callno = __fslt_method_transfs_setattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_setattr(__rref, rpc_info, NULL, instance, ino, iattr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_setattr(unsigned long long instance, unsigned long long ino, void *iattr)
{
	return actvxactcall_fsltcall_transfs_setattr(false, false, instance, ino, iattr);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_setattr(rref_t __rref, unsigned long long instance, unsigned long long ino, void *iattr)
{
	return actvxactcapcall_fsltcall_transfs_setattr(false, false, __rref, instance, ino, iattr);
}

extern int __actvcall_fsltcall_transfs_readdir(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);


extern int __actvcapcall_fsltcall_transfs_readdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_readdir(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_readdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_readdir(rpc_info, NULL, instance, ino, pos, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_readdir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_readdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_readdir(__rref, rpc_info, NULL, instance, ino, pos, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_readdir(unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	return actvxactcall_fsltcall_transfs_readdir(false, false, instance, ino, pos, buf, buf_size);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_readdir(rref_t __rref, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	return actvxactcapcall_fsltcall_transfs_readdir(false, false, __rref, instance, ino, pos, buf, buf_size);
}

extern int __actvcall_fsltcall_transfs_lookup(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);


extern int __actvcapcall_fsltcall_transfs_lookup(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_lookup(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, struct __actvret_fsltcall_transfs_lookup *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_lookup;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_lookup(rpc_info, __ret, instance, dir_ino, name_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_lookup(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, struct __actvret_fsltcall_transfs_lookup *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_lookup;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_lookup(__rref, rpc_info, __ret, instance, dir_ino, name_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_lookup(unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, struct __actvret_fsltcall_transfs_lookup *__ret)
{
	return actvxactcall_fsltcall_transfs_lookup(false, false, instance, dir_ino, name_buf, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_lookup(rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, struct __actvret_fsltcall_transfs_lookup *__ret)
{
	return actvxactcapcall_fsltcall_transfs_lookup(false, false, __rref, instance, dir_ino, name_buf, __ret);
}

extern int __actvcall_fsltcall_transfs_create(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


extern int __actvcapcall_fsltcall_transfs_create(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_create(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_create *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_create;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_create(rpc_info, __ret, instance, dir_ino, name_buf, mode, uid, gid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_create(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_create *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_create;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_create(__rref, rpc_info, __ret, instance, dir_ino, name_buf, mode, uid, gid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_create(unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_create *__ret)
{
	return actvxactcall_fsltcall_transfs_create(false, false, instance, dir_ino, name_buf, mode, uid, gid, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_create(rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_create *__ret)
{
	return actvxactcapcall_fsltcall_transfs_create(false, false, __rref, instance, dir_ino, name_buf, mode, uid, gid, __ret);
}

extern int __actvcall_fsltcall_transfs_unlink(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


extern int __actvcapcall_fsltcall_transfs_unlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_unlink(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_unlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_unlink(rpc_info, NULL, instance, dir_ino, name_buf, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_unlink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_unlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_unlink(__rref, rpc_info, NULL, instance, dir_ino, name_buf, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_unlink(unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return actvxactcall_fsltcall_transfs_unlink(false, false, instance, dir_ino, name_buf, ino);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_unlink(rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return actvxactcapcall_fsltcall_transfs_unlink(false, false, __rref, instance, dir_ino, name_buf, ino);
}

extern int __actvcall_fsltcall_transfs_write_iter(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


extern int __actvcapcall_fsltcall_transfs_write_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_write_iter(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt, struct __actvret_fsltcall_transfs_write_iter *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_write_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_write_iter(rpc_info, __ret, instance, ino, pos, iov, iovcnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_write_iter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt, struct __actvret_fsltcall_transfs_write_iter *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_write_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_write_iter(__rref, rpc_info, __ret, instance, ino, pos, iov, iovcnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_write_iter(unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt, struct __actvret_fsltcall_transfs_write_iter *__ret)
{
	return actvxactcall_fsltcall_transfs_write_iter(false, false, instance, ino, pos, iov, iovcnt, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_write_iter(rref_t __rref, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt, struct __actvret_fsltcall_transfs_write_iter *__ret)
{
	return actvxactcapcall_fsltcall_transfs_write_iter(false, false, __rref, instance, ino, pos, iov, iovcnt, __ret);
}

extern int __actvcall_fsltcall_transfs_read_iter(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


extern int __actvcapcall_fsltcall_transfs_read_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_read_iter(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	const unsigned long __callno = __fslt_method_transfs_read_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_read_iter(rpc_info, NULL, instance, ino, pos, iov, iovcnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_read_iter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	const unsigned long __callno = __fslt_method_transfs_read_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_read_iter(__rref, rpc_info, NULL, instance, ino, pos, iov, iovcnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_read_iter(unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	return actvxactcall_fsltcall_transfs_read_iter(false, false, instance, ino, pos, iov, iovcnt);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_read_iter(rref_t __rref, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	return actvxactcapcall_fsltcall_transfs_read_iter(false, false, __rref, instance, ino, pos, iov, iovcnt);
}

extern int __actvcall_fsltcall_transfs_ioctl(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);


extern int __actvcapcall_fsltcall_transfs_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	const unsigned long __callno = __fslt_method_transfs_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_ioctl(rpc_info, NULL, instance, ino, cmd, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	const unsigned long __callno = __fslt_method_transfs_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_ioctl(__rref, rpc_info, NULL, instance, ino, cmd, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_ioctl(unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	return actvxactcall_fsltcall_transfs_ioctl(false, false, instance, ino, cmd, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_ioctl(rref_t __rref, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	return actvxactcapcall_fsltcall_transfs_ioctl(false, false, __rref, instance, ino, cmd, arg);
}

extern int __actvcall_fsltcall_transfs_mkdir(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


extern int __actvcapcall_fsltcall_transfs_mkdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_mkdir(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_mkdir *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_mkdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_mkdir(rpc_info, __ret, instance, dir_ino, name_buf, mode, uid, gid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_mkdir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_mkdir *__ret)
{
	const unsigned long __callno = __fslt_method_transfs_mkdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_mkdir(__rref, rpc_info, __ret, instance, dir_ino, name_buf, mode, uid, gid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_mkdir(unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_mkdir *__ret)
{
	return actvxactcall_fsltcall_transfs_mkdir(false, false, instance, dir_ino, name_buf, mode, uid, gid, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_mkdir(rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid, struct __actvret_fsltcall_transfs_mkdir *__ret)
{
	return actvxactcapcall_fsltcall_transfs_mkdir(false, false, __rref, instance, dir_ino, name_buf, mode, uid, gid, __ret);
}

extern int __actvcall_fsltcall_transfs_rmdir(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


extern int __actvcapcall_fsltcall_transfs_rmdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_rmdir(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_rmdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_rmdir(rpc_info, NULL, instance, dir_ino, name_buf, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_rmdir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_rmdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_rmdir(__rref, rpc_info, NULL, instance, dir_ino, name_buf, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_rmdir(unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return actvxactcall_fsltcall_transfs_rmdir(false, false, instance, dir_ino, name_buf, ino);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_rmdir(rref_t __rref, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return actvxactcapcall_fsltcall_transfs_rmdir(false, false, __rref, instance, dir_ino, name_buf, ino);
}

extern int __actvcall_fsltcall_transfs_rename(unsigned long long __rpc_info, void *__buf, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);


extern int __actvcapcall_fsltcall_transfs_rename(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_rename(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	const unsigned long __callno = __fslt_method_transfs_rename;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_rename(rpc_info, NULL, old_instance, new_instance, old_dir_ino, new_dir_ino, ino, old_name_buf, new_name_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_rename(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	const unsigned long __callno = __fslt_method_transfs_rename;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_rename(__rref, rpc_info, NULL, old_instance, new_instance, old_dir_ino, new_dir_ino, ino, old_name_buf, new_name_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_rename(unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	return actvxactcall_fsltcall_transfs_rename(false, false, old_instance, new_instance, old_dir_ino, new_dir_ino, ino, old_name_buf, new_name_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_rename(rref_t __rref, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	return actvxactcapcall_fsltcall_transfs_rename(false, false, __rref, old_instance, new_instance, old_dir_ino, new_dir_ino, ino, old_name_buf, new_name_buf);
}

extern int __actvcall_fsltcall_transfs_statfs(unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf);


extern int __actvcapcall_fsltcall_transfs_statfs(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_statfs(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, void *buf)
{
	const unsigned long __callno = __fslt_method_transfs_statfs;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_statfs(rpc_info, NULL, instance, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_statfs(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, void *buf)
{
	const unsigned long __callno = __fslt_method_transfs_statfs;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_statfs(__rref, rpc_info, NULL, instance, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_statfs(unsigned long long instance, void *buf)
{
	return actvxactcall_fsltcall_transfs_statfs(false, false, instance, buf);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_statfs(rref_t __rref, unsigned long long instance, void *buf)
{
	return actvxactcapcall_fsltcall_transfs_statfs(false, false, __rref, instance, buf);
}

extern int __actvcall_fsltcall_transfs_show_options(unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf, size_t buf_size);


extern int __actvcapcall_fsltcall_transfs_show_options(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf, size_t buf_size);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_show_options(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_show_options;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_show_options(rpc_info, NULL, instance, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_show_options(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_show_options;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_show_options(__rref, rpc_info, NULL, instance, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_show_options(unsigned long long instance, void *buf, size_t buf_size)
{
	return actvxactcall_fsltcall_transfs_show_options(false, false, instance, buf, buf_size);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_show_options(rref_t __rref, unsigned long long instance, void *buf, size_t buf_size)
{
	return actvxactcapcall_fsltcall_transfs_show_options(false, false, __rref, instance, buf, buf_size);
}

extern int __actvcall_fsltcall_transfs_listxattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);


extern int __actvcapcall_fsltcall_transfs_listxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_listxattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_listxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_listxattr(rpc_info, NULL, instance, ino, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_listxattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_listxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_listxattr(__rref, rpc_info, NULL, instance, ino, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_listxattr(unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	return actvxactcall_fsltcall_transfs_listxattr(false, false, instance, ino, buf, buf_size);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_listxattr(rref_t __rref, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	return actvxactcapcall_fsltcall_transfs_listxattr(false, false, __rref, instance, ino, buf, buf_size);
}

extern int __actvcall_fsltcall_transfs_getxattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);


extern int __actvcapcall_fsltcall_transfs_getxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_getxattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_getxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_getxattr(rpc_info, NULL, instance, ino, prefix, name, name_len, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_getxattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	const unsigned long __callno = __fslt_method_transfs_getxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_getxattr(__rref, rpc_info, NULL, instance, ino, prefix, name, name_len, buf, buf_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_getxattr(unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	return actvxactcall_fsltcall_transfs_getxattr(false, false, instance, ino, prefix, name, name_len, buf, buf_size);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_getxattr(rref_t __rref, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	return actvxactcapcall_fsltcall_transfs_getxattr(false, false, __rref, instance, ino, prefix, name, name_len, buf, buf_size);
}

extern int __actvcall_fsltcall_transfs_setxattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);


extern int __actvcapcall_fsltcall_transfs_setxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_setxattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	const unsigned long __callno = __fslt_method_transfs_setxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_setxattr(rpc_info, NULL, instance, ino, prefix, name, name_len, value, value_len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_setxattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	const unsigned long __callno = __fslt_method_transfs_setxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_setxattr(__rref, rpc_info, NULL, instance, ino, prefix, name, name_len, value, value_len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_setxattr(unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	return actvxactcall_fsltcall_transfs_setxattr(false, false, instance, ino, prefix, name, name_len, value, value_len, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_setxattr(rref_t __rref, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	return actvxactcapcall_fsltcall_transfs_setxattr(false, false, __rref, instance, ino, prefix, name, name_len, value, value_len, flags);
}

extern int __actvcall_fsltcall_transfs_destroy_node(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino);


extern int __actvcapcall_fsltcall_transfs_destroy_node(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_destroy_node(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_destroy_node;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_destroy_node(rpc_info, NULL, instance, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_destroy_node(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_destroy_node;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_destroy_node(__rref, rpc_info, NULL, instance, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_destroy_node(unsigned long long instance, unsigned long long ino)
{
	return actvxactcall_fsltcall_transfs_destroy_node(false, false, instance, ino);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_destroy_node(rref_t __rref, unsigned long long instance, unsigned long long ino)
{
	return actvxactcapcall_fsltcall_transfs_destroy_node(false, false, __rref, instance, ino);
}

extern int __actvcall_fsltcall_transfs_sync(unsigned long long __rpc_info, void *__buf, unsigned long long instance);


extern int __actvcapcall_fsltcall_transfs_sync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_sync(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance)
{
	const unsigned long __callno = __fslt_method_transfs_sync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_sync(rpc_info, NULL, instance);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_sync(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance)
{
	const unsigned long __callno = __fslt_method_transfs_sync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_sync(__rref, rpc_info, NULL, instance);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_sync(unsigned long long instance)
{
	return actvxactcall_fsltcall_transfs_sync(false, false, instance);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_sync(rref_t __rref, unsigned long long instance)
{
	return actvxactcapcall_fsltcall_transfs_sync(false, false, __rref, instance);
}

extern int __actvcall_fsltcall_transfs_fsync(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino);


extern int __actvcapcall_fsltcall_transfs_fsync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_fsync(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_fsync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_fsync(rpc_info, NULL, instance, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_fsync(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino)
{
	const unsigned long __callno = __fslt_method_transfs_fsync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_fsync(__rref, rpc_info, NULL, instance, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_fsync(unsigned long long instance, unsigned long long ino)
{
	return actvxactcall_fsltcall_transfs_fsync(false, false, instance, ino);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_fsync(rref_t __rref, unsigned long long instance, unsigned long long ino)
{
	return actvxactcapcall_fsltcall_transfs_fsync(false, false, __rref, instance, ino);
}

extern int __actvcall_fsltcall_transfs_flush(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int flags);


extern int __actvcapcall_fsltcall_transfs_flush(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int flags);


static inline int __attribute__((always_inline)) actvxactcall_fsltcall_transfs_flush(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	const unsigned long __callno = __fslt_method_transfs_flush;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fsltcall_transfs_flush(rpc_info, NULL, instance, ino, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fsltcall_transfs_flush(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	const unsigned long __callno = __fslt_method_transfs_flush;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fsltcall_transfs_flush(__rref, rpc_info, NULL, instance, ino, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fsltcall_transfs_flush(unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	return actvxactcall_fsltcall_transfs_flush(false, false, instance, ino, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_fsltcall_transfs_flush(rref_t __rref, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	return actvxactcapcall_fsltcall_transfs_flush(false, false, __rref, instance, ino, flags);
}

#endif
#ifdef SYSIF_GEN_IFACE
#include <hmkernel/compiler.h>
#include <hmasm/actv_rpc.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <libstrict/strict.h>
#include <hmasm/lsyscall.h>
static int __used __noinline __opt_noframe __internal_rpccall_nobuf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_buf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_retbuf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_fallback(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_nobuf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(NoBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_buf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(SendBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_retbuf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(RetBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_fallback(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(, ret);
	return ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_get_tree(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_get_tree(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_get_tree(unsigned long long __rpc_info, void *__buf, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	int __ret;
	dev_t __maybe_unused __actv_local_var_devno = devno;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_fs_type = fs_type;
	struct hm_actv_buf __maybe_unused __actv_local_var_opts = opts;
	__ret = ____actvcall_fsltcall_transfs_get_tree(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(fs_type, __actv_local_var_fs_type), __ARCH_ACTV_RPC_PASS_ARG(opts, __actv_local_var_opts));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_get_tree(rref_t __rref, unsigned long long __rpc_info, void *__buf, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	int __ret;
	dev_t __maybe_unused __actv_local_var_devno = devno;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_fs_type = fs_type;
	struct hm_actv_buf __maybe_unused __actv_local_var_opts = opts;
	__ret = ____actvcapcall_fsltcall_transfs_get_tree(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(fs_type, __actv_local_var_fs_type), __ARCH_ACTV_RPC_PASS_ARG(opts, __actv_local_var_opts));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_kill_sb(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_kill_sb(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_kill_sb(unsigned long long __rpc_info, void *__buf, unsigned long long instance)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__ret = ____actvcall_fsltcall_transfs_kill_sb(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_kill_sb(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__ret = ____actvcapcall_fsltcall_transfs_kill_sb(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_getattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_getattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_getattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_iattr_buf = iattr_buf;
	__ret = ____actvcall_fsltcall_transfs_getattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(iattr_buf, __actv_local_var_iattr_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_getattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_iattr_buf = iattr_buf;
	__ret = ____actvcapcall_fsltcall_transfs_getattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(iattr_buf, __actv_local_var_iattr_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_setattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_setattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_setattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *iattr)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__maybe_unused void *__actv_local_var_iattr = iattr;
	__ret = ____actvcall_fsltcall_transfs_setattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(iattr, __actv_local_var_iattr));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_setattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *iattr)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__maybe_unused void *__actv_local_var_iattr = iattr;
	__ret = ____actvcapcall_fsltcall_transfs_setattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(iattr, __actv_local_var_iattr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_readdir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_readdir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_readdir(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	long long __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcall_fsltcall_transfs_readdir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_readdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	long long __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcapcall_fsltcall_transfs_readdir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_lookup(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_lookup(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_lookup(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	__ret = ____actvcall_fsltcall_transfs_lookup(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_lookup(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	__ret = ____actvcapcall_fsltcall_transfs_lookup(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_create(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_create(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_create(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	unsigned int __maybe_unused __actv_local_var_uid = uid;
	unsigned int __maybe_unused __actv_local_var_gid = gid;
	__ret = ____actvcall_fsltcall_transfs_create(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid), __ARCH_ACTV_RPC_PASS_ARG(gid, __actv_local_var_gid));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_create(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	unsigned int __maybe_unused __actv_local_var_uid = uid;
	unsigned int __maybe_unused __actv_local_var_gid = gid;
	__ret = ____actvcapcall_fsltcall_transfs_create(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid), __ARCH_ACTV_RPC_PASS_ARG(gid, __actv_local_var_gid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_unlink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_unlink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_unlink(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcall_fsltcall_transfs_unlink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_unlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcapcall_fsltcall_transfs_unlink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_write_iter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_write_iter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_write_iter(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	long long __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	__ret = ____actvcall_fsltcall_transfs_write_iter(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_write_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	long long __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	__ret = ____actvcapcall_fsltcall_transfs_write_iter(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_read_iter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_read_iter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_read_iter(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	long long __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	__ret = ____actvcall_fsltcall_transfs_read_iter(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_read_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	long long __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	__ret = ____actvcapcall_fsltcall_transfs_read_iter(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_ioctl(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	unsigned int __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_fsltcall_transfs_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	unsigned int __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_fsltcall_transfs_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_mkdir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_mkdir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_mkdir(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	unsigned int __maybe_unused __actv_local_var_uid = uid;
	unsigned int __maybe_unused __actv_local_var_gid = gid;
	__ret = ____actvcall_fsltcall_transfs_mkdir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid), __ARCH_ACTV_RPC_PASS_ARG(gid, __actv_local_var_gid));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_mkdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	unsigned int __maybe_unused __actv_local_var_uid = uid;
	unsigned int __maybe_unused __actv_local_var_gid = gid;
	__ret = ____actvcapcall_fsltcall_transfs_mkdir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid), __ARCH_ACTV_RPC_PASS_ARG(gid, __actv_local_var_gid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_rmdir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_rmdir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_rmdir(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcall_fsltcall_transfs_rmdir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_rmdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_dir_ino = dir_ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_name_buf = name_buf;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcapcall_fsltcall_transfs_rmdir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(dir_ino, __actv_local_var_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(name_buf, __actv_local_var_name_buf), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_rename(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_rename(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_rename(unsigned long long __rpc_info, void *__buf, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_old_instance = old_instance;
	unsigned long long __maybe_unused __actv_local_var_new_instance = new_instance;
	unsigned long long __maybe_unused __actv_local_var_old_dir_ino = old_dir_ino;
	unsigned long long __maybe_unused __actv_local_var_new_dir_ino = new_dir_ino;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_old_name_buf = old_name_buf;
	struct hm_actv_buf __maybe_unused __actv_local_var_new_name_buf = new_name_buf;
	__ret = ____actvcall_fsltcall_transfs_rename(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(old_instance, __actv_local_var_old_instance), __ARCH_ACTV_RPC_PASS_ARG(new_instance, __actv_local_var_new_instance), __ARCH_ACTV_RPC_PASS_ARG(old_dir_ino, __actv_local_var_old_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(new_dir_ino, __actv_local_var_new_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(old_name_buf, __actv_local_var_old_name_buf), __ARCH_ACTV_RPC_PASS_ARG(new_name_buf, __actv_local_var_new_name_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_rename(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_old_instance = old_instance;
	unsigned long long __maybe_unused __actv_local_var_new_instance = new_instance;
	unsigned long long __maybe_unused __actv_local_var_old_dir_ino = old_dir_ino;
	unsigned long long __maybe_unused __actv_local_var_new_dir_ino = new_dir_ino;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	struct hm_actv_buf __maybe_unused __actv_local_var_old_name_buf = old_name_buf;
	struct hm_actv_buf __maybe_unused __actv_local_var_new_name_buf = new_name_buf;
	__ret = ____actvcapcall_fsltcall_transfs_rename(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(old_instance, __actv_local_var_old_instance), __ARCH_ACTV_RPC_PASS_ARG(new_instance, __actv_local_var_new_instance), __ARCH_ACTV_RPC_PASS_ARG(old_dir_ino, __actv_local_var_old_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(new_dir_ino, __actv_local_var_new_dir_ino), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(old_name_buf, __actv_local_var_old_name_buf), __ARCH_ACTV_RPC_PASS_ARG(new_name_buf, __actv_local_var_new_name_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_statfs(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_statfs(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_statfs(unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__maybe_unused void *__actv_local_var_buf = buf;
	__ret = ____actvcall_fsltcall_transfs_statfs(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_statfs(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__maybe_unused void *__actv_local_var_buf = buf;
	__ret = ____actvcapcall_fsltcall_transfs_statfs(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_show_options(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_show_options(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_show_options(unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcall_fsltcall_transfs_show_options(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_show_options(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcapcall_fsltcall_transfs_show_options(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_listxattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_listxattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_listxattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcall_fsltcall_transfs_listxattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_listxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcapcall_fsltcall_transfs_listxattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_getxattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_getxattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_getxattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	int __maybe_unused __actv_local_var_prefix = prefix;
	__maybe_unused const void *__actv_local_var_name = name;
	size_t __maybe_unused __actv_local_var_name_len = name_len;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcall_fsltcall_transfs_getxattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(name_len, __actv_local_var_name_len), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_getxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	int __maybe_unused __actv_local_var_prefix = prefix;
	__maybe_unused const void *__actv_local_var_name = name;
	size_t __maybe_unused __actv_local_var_name_len = name_len;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_buf_size = buf_size;
	__ret = ____actvcapcall_fsltcall_transfs_getxattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(name_len, __actv_local_var_name_len), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(buf_size, __actv_local_var_buf_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_setxattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_setxattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_setxattr(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	int __maybe_unused __actv_local_var_prefix = prefix;
	__maybe_unused const void *__actv_local_var_name = name;
	size_t __maybe_unused __actv_local_var_name_len = name_len;
	__maybe_unused const void *__actv_local_var_value = value;
	size_t __maybe_unused __actv_local_var_value_len = value_len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_fsltcall_transfs_setxattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(name_len, __actv_local_var_name_len), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value), __ARCH_ACTV_RPC_PASS_ARG(value_len, __actv_local_var_value_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_setxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	int __maybe_unused __actv_local_var_prefix = prefix;
	__maybe_unused const void *__actv_local_var_name = name;
	size_t __maybe_unused __actv_local_var_name_len = name_len;
	__maybe_unused const void *__actv_local_var_value = value;
	size_t __maybe_unused __actv_local_var_value_len = value_len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_fsltcall_transfs_setxattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(name_len, __actv_local_var_name_len), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value), __ARCH_ACTV_RPC_PASS_ARG(value_len, __actv_local_var_value_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_destroy_node(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_destroy_node(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_destroy_node(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcall_fsltcall_transfs_destroy_node(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_destroy_node(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcapcall_fsltcall_transfs_destroy_node(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_sync(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_sync(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_sync(unsigned long long __rpc_info, void *__buf, unsigned long long instance)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__ret = ____actvcall_fsltcall_transfs_sync(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_sync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	__ret = ____actvcapcall_fsltcall_transfs_sync(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_fsync(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_fsync(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_fsync(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcall_fsltcall_transfs_fsync(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_fsync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcapcall_fsltcall_transfs_fsync(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fsltcall_transfs_flush(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fsltcall_transfs_flush(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fsltcall_transfs_flush(unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_fsltcall_transfs_flush(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_fsltcall_transfs_flush(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_instance = instance;
	unsigned long long __maybe_unused __actv_local_var_ino = ino;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_fsltcall_transfs_flush(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(instance, __actv_local_var_instance), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

#endif
#endif
