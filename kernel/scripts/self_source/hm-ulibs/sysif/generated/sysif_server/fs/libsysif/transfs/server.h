/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for fslt
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-filesystems/fs/libtransfs/libsysif/api.sysif -I ../kernel/hongmeng/hm-filesystems/fs/libtransfs/libsysif --api-dest ../image/usr/include/generated/sysif_client/fs/libsysif/transfs/api.h --server-dest ../image/usr/include/generated/sysif_server/fs/libsysif/transfs/server.h --legacy-mode
* Create: Fri Oct 31 10:38:41 2025
*/
#ifndef LIBSYSIF_FSLT_SERVER_H
#define LIBSYSIF_FSLT_SERVER_H
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
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);


extern int actvhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance);


extern int actvhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);


extern int actvhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);


extern int actvhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);


extern int actvhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


extern int actvhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


extern int actvhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


extern int actvhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);


extern int actvhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


extern int actvhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);


extern int actvhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);


extern int actvhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);


extern int actvhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance);


extern int actvhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);


#else /* aarch64 */
extern int actvhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);


extern int actvhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance);


extern int actvhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);


extern int actvhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);


extern int actvhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);


extern int actvhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


extern int actvhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


extern int actvhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);


extern int actvhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);


extern int actvhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);


extern int actvhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);


extern int actvhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);


extern int actvhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);


extern int actvhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);


extern int actvhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance);


extern int actvhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);


extern int actvhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);


extern int __actvshadowhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_fsltcall_transfs_get_tree* actvhdlr_fslthandler_transfs_get_tree_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_get_tree));
	return (struct __actvret_fsltcall_transfs_get_tree*)__ret;
}

static inline struct __actvret_fsltcall_transfs_kill_sb* actvhdlr_fslthandler_transfs_kill_sb_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_kill_sb));
	return (struct __actvret_fsltcall_transfs_kill_sb*)__ret;
}

static inline struct __actvret_fsltcall_transfs_getattr* actvhdlr_fslthandler_transfs_getattr_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_getattr));
	return (struct __actvret_fsltcall_transfs_getattr*)__ret;
}

static inline struct __actvret_fsltcall_transfs_setattr* actvhdlr_fslthandler_transfs_setattr_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_setattr));
	return (struct __actvret_fsltcall_transfs_setattr*)__ret;
}

static inline struct __actvret_fsltcall_transfs_readdir* actvhdlr_fslthandler_transfs_readdir_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_readdir));
	return (struct __actvret_fsltcall_transfs_readdir*)__ret;
}

static inline struct __actvret_fsltcall_transfs_lookup* actvhdlr_fslthandler_transfs_lookup_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_lookup));
	return (struct __actvret_fsltcall_transfs_lookup*)__ret;
}

static inline struct __actvret_fsltcall_transfs_create* actvhdlr_fslthandler_transfs_create_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_create));
	return (struct __actvret_fsltcall_transfs_create*)__ret;
}

static inline struct __actvret_fsltcall_transfs_unlink* actvhdlr_fslthandler_transfs_unlink_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_unlink));
	return (struct __actvret_fsltcall_transfs_unlink*)__ret;
}

static inline struct __actvret_fsltcall_transfs_write_iter* actvhdlr_fslthandler_transfs_write_iter_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_write_iter));
	return (struct __actvret_fsltcall_transfs_write_iter*)__ret;
}

static inline struct __actvret_fsltcall_transfs_read_iter* actvhdlr_fslthandler_transfs_read_iter_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_read_iter));
	return (struct __actvret_fsltcall_transfs_read_iter*)__ret;
}

static inline struct __actvret_fsltcall_transfs_ioctl* actvhdlr_fslthandler_transfs_ioctl_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_ioctl));
	return (struct __actvret_fsltcall_transfs_ioctl*)__ret;
}

static inline struct __actvret_fsltcall_transfs_mkdir* actvhdlr_fslthandler_transfs_mkdir_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_mkdir));
	return (struct __actvret_fsltcall_transfs_mkdir*)__ret;
}

static inline struct __actvret_fsltcall_transfs_rmdir* actvhdlr_fslthandler_transfs_rmdir_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_rmdir));
	return (struct __actvret_fsltcall_transfs_rmdir*)__ret;
}

static inline struct __actvret_fsltcall_transfs_rename* actvhdlr_fslthandler_transfs_rename_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_rename));
	return (struct __actvret_fsltcall_transfs_rename*)__ret;
}

static inline struct __actvret_fsltcall_transfs_statfs* actvhdlr_fslthandler_transfs_statfs_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_statfs));
	return (struct __actvret_fsltcall_transfs_statfs*)__ret;
}

static inline struct __actvret_fsltcall_transfs_show_options* actvhdlr_fslthandler_transfs_show_options_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_show_options));
	return (struct __actvret_fsltcall_transfs_show_options*)__ret;
}

static inline struct __actvret_fsltcall_transfs_listxattr* actvhdlr_fslthandler_transfs_listxattr_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_listxattr));
	return (struct __actvret_fsltcall_transfs_listxattr*)__ret;
}

static inline struct __actvret_fsltcall_transfs_getxattr* actvhdlr_fslthandler_transfs_getxattr_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_getxattr));
	return (struct __actvret_fsltcall_transfs_getxattr*)__ret;
}

static inline struct __actvret_fsltcall_transfs_setxattr* actvhdlr_fslthandler_transfs_setxattr_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_setxattr));
	return (struct __actvret_fsltcall_transfs_setxattr*)__ret;
}

static inline struct __actvret_fsltcall_transfs_destroy_node* actvhdlr_fslthandler_transfs_destroy_node_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_destroy_node));
	return (struct __actvret_fsltcall_transfs_destroy_node*)__ret;
}

static inline struct __actvret_fsltcall_transfs_sync* actvhdlr_fslthandler_transfs_sync_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_sync));
	return (struct __actvret_fsltcall_transfs_sync*)__ret;
}

static inline struct __actvret_fsltcall_transfs_fsync* actvhdlr_fslthandler_transfs_fsync_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_fsync));
	return (struct __actvret_fsltcall_transfs_fsync*)__ret;
}

static inline struct __actvret_fsltcall_transfs_flush* actvhdlr_fslthandler_transfs_flush_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fsltcall_transfs_flush));
	return (struct __actvret_fsltcall_transfs_flush*)__ret;
}

#ifdef __sysif_server_fslt_no_compat32_handlers__
#ifdef __HOST_LLT__
union fslt_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgehdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgehdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgehdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgehdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union fslt_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgehdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgehdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgehdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgehdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union fslt_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgehdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgehdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgehdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgehdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union fslt_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgehdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgehdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgehdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgehdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union fslt_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts);
	int (*p__actvbridgehdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr);
	int (*p__actvbridgehdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt);
	int (*p__actvbridgehdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg);
	int (*p__actvbridgehdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid);
	int (*p__actvbridgehdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf);
	int (*p__actvbridgehdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size);
	int (*p__actvbridgehdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags);
	int (*p__actvbridgehdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long long instance);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance);
	int (*p__actvbridgehdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino);
	int (*p__actvbridgehdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_create)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rename)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_sync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fslthandler_transfs_flush)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_fslt_no_compat32_handlers__) && defined(__sysif_server_fslt_no_ilp32_handlers__)
#define __fslt_method_TIMES 1
#elif !defined(__sysif_server_fslt_no_compat32_handlers__) && !defined(__sysif_server_fslt_no_ilp32_handlers__)
#define __fslt_method_TIMES 3
#else
#define __fslt_method_TIMES 2
#endif
extern union fslt_hdlrs_union sysif_fslt_actvhdlr_table[__fslt_method_MAX * __fslt_method_TIMES];

extern union fslt_hdlrs_union sysif_fslt_actvcapcallhdlr_table[__fslt_method_MAX * __fslt_method_TIMES];

extern void sysif_show_fslt_methods(void);


#ifdef SYSIF_GEN_IFACE
#include <stdint.h>
#include <hmasm/actv_rpc.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libsysif/base/server_arg_compat.h>
#ifdef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) __actvshadowhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, ...)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	return E_HM_NOSYS;
}

#else
int __attribute__((weak)) actvhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	return E_HM_NOSYS;
}

int __attribute__((weak)) actvhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	return E_HM_NOSYS;
}

#endif
#ifdef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_get_tree(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(dev_t, devno), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, mode), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, fs_type), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, opts));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_get_tree(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(dev_t, devno), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, mode), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, fs_type), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, opts));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_kill_sb(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_kill_sb(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, iattr_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, iattr_buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, iattr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, iattr));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_readdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_readdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_lookup(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_lookup(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_create(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_create(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_unlink(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_unlink(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_write_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_write_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_read_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_read_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_ioctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cmd), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_ioctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cmd), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_mkdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_mkdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rmdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rmdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rename(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, old_name_buf), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, new_name_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rename(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, old_name_buf), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, new_name_buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_statfs(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_statfs(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_show_options(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_show_options(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_listxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_listxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, value), __ARCH_ACTV_RPC_RECV_ARG(size_t, value_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, value), __ARCH_ACTV_RPC_RECV_ARG(size_t, value_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_destroy_node(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_destroy_node(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_sync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_sync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_fsync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_fsync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_flush(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_flush(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

#ifndef __sysif_server_fslt_no_compat32_handlers__
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
struct compat_x3 {
	unsigned int reserverd;
	unsigned int w3;
};

#else
struct compat_x3 {
	unsigned int w3;
	unsigned int reserverd;
};

#endif /* CONFIG_BIG_ENDIAN */
static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	dev_t __maybe_unused _devno;
	unsigned int __maybe_unused _mode;
	struct hm_actv_buf __maybe_unused _fs_type;
	struct hm_actv_buf __maybe_unused _opts;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(dev_t);
		type = __type_of(dev_t);
		__fetch_one_arg((void*)&_devno, data, 0, arg_sz, model, type, dev_t, _devno)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_mode, data, 1, arg_sz, model, type, unsigned int, _mode)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_fs_type, data, 2, arg_sz, model, type, struct hm_actv_buf, _fs_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_opts, data, 3, arg_sz, model, type, struct hm_actv_buf, _opts)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_get_tree(sender, credential, _devno, _mode, _fs_type, _opts);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	dev_t __maybe_unused _devno;
	unsigned int __maybe_unused _mode;
	struct hm_actv_buf __maybe_unused _fs_type;
	struct hm_actv_buf __maybe_unused _opts;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(dev_t);
		type = __type_of(dev_t);
		__fetch_one_arg((void*)&_devno, data, 0, arg_sz, model, type, dev_t, _devno)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_mode, data, 1, arg_sz, model, type, unsigned int, _mode)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_fs_type, data, 2, arg_sz, model, type, struct hm_actv_buf, _fs_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_opts, data, 3, arg_sz, model, type, struct hm_actv_buf, _opts)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_get_tree(sender, credential, _devno, _mode, _fs_type, _opts);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_kill_sb(sender, credential, _instance);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_kill_sb(sender, credential, _instance);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	struct hm_actv_buf __maybe_unused _iattr_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_iattr_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _iattr_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getattr(sender, credential, _instance, _ino, _iattr_buf);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	struct hm_actv_buf __maybe_unused _iattr_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_iattr_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _iattr_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getattr(sender, credential, _instance, _ino, _iattr_buf);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	__maybe_unused void *_iattr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_iattr, data, 2, arg_sz, model, type, void*, _iattr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setattr(sender, credential, _instance, _ino, _iattr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	__maybe_unused void *_iattr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_iattr, data, 2, arg_sz, model, type, void*, _iattr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setattr(sender, credential, _instance, _ino, _iattr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	long long __maybe_unused _pos;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long long);
		type = __type_of(long long);
		__fetch_one_arg((void*)&_pos, data, 2, arg_sz, model, type, long long, _pos)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 3, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 4, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_readdir(sender, credential, _instance, _ino, _pos, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	long long __maybe_unused _pos;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long long);
		type = __type_of(long long);
		__fetch_one_arg((void*)&_pos, data, 2, arg_sz, model, type, long long, _pos)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 3, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 4, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_readdir(sender, credential, _instance, _ino, _pos, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_lookup(sender, credential, _instance, _dir_ino, _name_buf);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_lookup(sender, credential, _instance, _dir_ino, _name_buf);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	mode_t __maybe_unused _mode;
	unsigned int __maybe_unused _uid;
	unsigned int __maybe_unused _gid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(mode_t);
		type = __type_of(mode_t);
		__fetch_one_arg((void*)&_mode, data, 3, arg_sz, model, type, mode_t, _mode)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_uid, data, 4, arg_sz, model, type, unsigned int, _uid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_gid, data, 5, arg_sz, model, type, unsigned int, _gid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_create(sender, credential, _instance, _dir_ino, _name_buf, _mode, _uid, _gid);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	mode_t __maybe_unused _mode;
	unsigned int __maybe_unused _uid;
	unsigned int __maybe_unused _gid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(mode_t);
		type = __type_of(mode_t);
		__fetch_one_arg((void*)&_mode, data, 3, arg_sz, model, type, mode_t, _mode)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_uid, data, 4, arg_sz, model, type, unsigned int, _uid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_gid, data, 5, arg_sz, model, type, unsigned int, _gid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_create(sender, credential, _instance, _dir_ino, _name_buf, _mode, _uid, _gid);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 3, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_unlink(sender, credential, _instance, _dir_ino, _name_buf, _ino);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 3, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_unlink(sender, credential, _instance, _dir_ino, _name_buf, _ino);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	long long __maybe_unused _pos;
	__maybe_unused void *_iov;
	int __maybe_unused _iovcnt;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long long);
		type = __type_of(long long);
		__fetch_one_arg((void*)&_pos, data, 2, arg_sz, model, type, long long, _pos)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_iov, data, 3, arg_sz, model, type, void*, _iov)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_iovcnt, data, 4, arg_sz, model, type, int, _iovcnt)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_write_iter(sender, credential, _instance, _ino, _pos, _iov, _iovcnt);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	long long __maybe_unused _pos;
	__maybe_unused void *_iov;
	int __maybe_unused _iovcnt;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long long);
		type = __type_of(long long);
		__fetch_one_arg((void*)&_pos, data, 2, arg_sz, model, type, long long, _pos)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_iov, data, 3, arg_sz, model, type, void*, _iov)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_iovcnt, data, 4, arg_sz, model, type, int, _iovcnt)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_write_iter(sender, credential, _instance, _ino, _pos, _iov, _iovcnt);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	long long __maybe_unused _pos;
	__maybe_unused void *_iov;
	int __maybe_unused _iovcnt;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long long);
		type = __type_of(long long);
		__fetch_one_arg((void*)&_pos, data, 2, arg_sz, model, type, long long, _pos)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_iov, data, 3, arg_sz, model, type, void*, _iov)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_iovcnt, data, 4, arg_sz, model, type, int, _iovcnt)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_read_iter(sender, credential, _instance, _ino, _pos, _iov, _iovcnt);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	long long __maybe_unused _pos;
	__maybe_unused void *_iov;
	int __maybe_unused _iovcnt;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(long long);
		type = __type_of(long long);
		__fetch_one_arg((void*)&_pos, data, 2, arg_sz, model, type, long long, _pos)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_iov, data, 3, arg_sz, model, type, void*, _iov)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_iovcnt, data, 4, arg_sz, model, type, int, _iovcnt)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_read_iter(sender, credential, _instance, _ino, _pos, _iov, _iovcnt);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	unsigned int __maybe_unused _cmd;
	unsigned long __maybe_unused _arg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cmd, data, 2, arg_sz, model, type, unsigned int, _cmd)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long);
		type = __type_of(unsigned long);
		__fetch_one_arg((void*)&_arg, data, 3, arg_sz, model, type, unsigned long, _arg)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_ioctl(sender, credential, _instance, _ino, _cmd, _arg);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	unsigned int __maybe_unused _cmd;
	unsigned long __maybe_unused _arg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cmd, data, 2, arg_sz, model, type, unsigned int, _cmd)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long);
		type = __type_of(unsigned long);
		__fetch_one_arg((void*)&_arg, data, 3, arg_sz, model, type, unsigned long, _arg)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_ioctl(sender, credential, _instance, _ino, _cmd, _arg);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	mode_t __maybe_unused _mode;
	unsigned int __maybe_unused _uid;
	unsigned int __maybe_unused _gid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(mode_t);
		type = __type_of(mode_t);
		__fetch_one_arg((void*)&_mode, data, 3, arg_sz, model, type, mode_t, _mode)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_uid, data, 4, arg_sz, model, type, unsigned int, _uid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_gid, data, 5, arg_sz, model, type, unsigned int, _gid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_mkdir(sender, credential, _instance, _dir_ino, _name_buf, _mode, _uid, _gid);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	mode_t __maybe_unused _mode;
	unsigned int __maybe_unused _uid;
	unsigned int __maybe_unused _gid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(mode_t);
		type = __type_of(mode_t);
		__fetch_one_arg((void*)&_mode, data, 3, arg_sz, model, type, mode_t, _mode)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_uid, data, 4, arg_sz, model, type, unsigned int, _uid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_gid, data, 5, arg_sz, model, type, unsigned int, _gid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_mkdir(sender, credential, _instance, _dir_ino, _name_buf, _mode, _uid, _gid);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 3, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rmdir(sender, credential, _instance, _dir_ino, _name_buf, _ino);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _dir_ino;
	struct hm_actv_buf __maybe_unused _name_buf;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_dir_ino, data, 1, arg_sz, model, type, unsigned long long, _dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_name_buf, data, 2, arg_sz, model, type, struct hm_actv_buf, _name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 3, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rmdir(sender, credential, _instance, _dir_ino, _name_buf, _ino);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _old_instance;
	unsigned long long __maybe_unused _new_instance;
	unsigned long long __maybe_unused _old_dir_ino;
	unsigned long long __maybe_unused _new_dir_ino;
	unsigned long long __maybe_unused _ino;
	struct hm_actv_buf __maybe_unused _old_name_buf;
	struct hm_actv_buf __maybe_unused _new_name_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_old_instance, data, 0, arg_sz, model, type, unsigned long long, _old_instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_new_instance, data, 1, arg_sz, model, type, unsigned long long, _new_instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_old_dir_ino, data, 2, arg_sz, model, type, unsigned long long, _old_dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_new_dir_ino, data, 3, arg_sz, model, type, unsigned long long, _new_dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 4, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_old_name_buf, data, 5, arg_sz, model, type, struct hm_actv_buf, _old_name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_new_name_buf, data, 6, arg_sz, model, type, struct hm_actv_buf, _new_name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rename(sender, credential, _old_instance, _new_instance, _old_dir_ino, _new_dir_ino, _ino, _old_name_buf, _new_name_buf);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _old_instance;
	unsigned long long __maybe_unused _new_instance;
	unsigned long long __maybe_unused _old_dir_ino;
	unsigned long long __maybe_unused _new_dir_ino;
	unsigned long long __maybe_unused _ino;
	struct hm_actv_buf __maybe_unused _old_name_buf;
	struct hm_actv_buf __maybe_unused _new_name_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_old_instance, data, 0, arg_sz, model, type, unsigned long long, _old_instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_new_instance, data, 1, arg_sz, model, type, unsigned long long, _new_instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_old_dir_ino, data, 2, arg_sz, model, type, unsigned long long, _old_dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_new_dir_ino, data, 3, arg_sz, model, type, unsigned long long, _new_dir_ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 4, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_old_name_buf, data, 5, arg_sz, model, type, struct hm_actv_buf, _old_name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hm_actv_buf);
		type = __type_of(struct hm_actv_buf);
		__fetch_one_arg((void*)&_new_name_buf, data, 6, arg_sz, model, type, struct hm_actv_buf, _new_name_buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_rename(sender, credential, _old_instance, _new_instance, _old_dir_ino, _new_dir_ino, _ino, _old_name_buf, _new_name_buf);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	__maybe_unused void *_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 1, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_statfs(sender, credential, _instance, _buf);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	__maybe_unused void *_buf;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 1, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_statfs(sender, credential, _instance, _buf);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 1, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 2, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_show_options(sender, credential, _instance, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 1, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 2, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_show_options(sender, credential, _instance, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 2, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 3, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_listxattr(sender, credential, _instance, _ino, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 2, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 3, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_listxattr(sender, credential, _instance, _ino, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	int __maybe_unused _prefix;
	__maybe_unused const void *_name;
	size_t __maybe_unused _name_len;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_prefix, data, 2, arg_sz, model, type, int, _prefix)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_name, data, 3, arg_sz, model, type, const void*, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_name_len, data, 4, arg_sz, model, type, size_t, _name_len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 5, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 6, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getxattr(sender, credential, _instance, _ino, _prefix, _name, _name_len, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	int __maybe_unused _prefix;
	__maybe_unused const void *_name;
	size_t __maybe_unused _name_len;
	__maybe_unused void *_buf;
	size_t __maybe_unused _buf_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_prefix, data, 2, arg_sz, model, type, int, _prefix)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_name, data, 3, arg_sz, model, type, const void*, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_name_len, data, 4, arg_sz, model, type, size_t, _name_len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 5, arg_sz, model, type, void*, _buf)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_buf_size, data, 6, arg_sz, model, type, size_t, _buf_size)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_getxattr(sender, credential, _instance, _ino, _prefix, _name, _name_len, _buf, _buf_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	int __maybe_unused _prefix;
	__maybe_unused const void *_name;
	size_t __maybe_unused _name_len;
	__maybe_unused const void *_value;
	size_t __maybe_unused _value_len;
	int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_prefix, data, 2, arg_sz, model, type, int, _prefix)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_name, data, 3, arg_sz, model, type, const void*, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_name_len, data, 4, arg_sz, model, type, size_t, _name_len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_value, data, 5, arg_sz, model, type, const void*, _value)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_value_len, data, 6, arg_sz, model, type, size_t, _value_len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_flags, data, 7, arg_sz, model, type, int, _flags)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setxattr(sender, credential, _instance, _ino, _prefix, _name, _name_len, _value, _value_len, _flags);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	int __maybe_unused _prefix;
	__maybe_unused const void *_name;
	size_t __maybe_unused _name_len;
	__maybe_unused const void *_value;
	size_t __maybe_unused _value_len;
	int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_prefix, data, 2, arg_sz, model, type, int, _prefix)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_name, data, 3, arg_sz, model, type, const void*, _name)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_name_len, data, 4, arg_sz, model, type, size_t, _name_len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_value, data, 5, arg_sz, model, type, const void*, _value)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_value_len, data, 6, arg_sz, model, type, size_t, _value_len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_flags, data, 7, arg_sz, model, type, int, _flags)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_setxattr(sender, credential, _instance, _ino, _prefix, _name, _name_len, _value, _value_len, _flags);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_destroy_node(sender, credential, _instance, _ino);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_destroy_node(sender, credential, _instance, _ino);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_sync(sender, credential, _instance);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_sync(sender, credential, _instance);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_fsync(sender, credential, _instance, _ino);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_fsync(sender, credential, _instance, _ino);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	unsigned int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_flags, data, 2, arg_sz, model, type, unsigned int, _flags)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_flush(sender, credential, _instance, _ino, _flags);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned long long __maybe_unused _instance;
	unsigned long long __maybe_unused _ino;
	unsigned int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_instance, data, 0, arg_sz, model, type, unsigned long long, _instance)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_ino, data, 1, arg_sz, model, type, unsigned long long, _ino)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_flags, data, 2, arg_sz, model, type, unsigned int, _flags)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fslthandler_transfs_flush(sender, credential, _instance, _ino, _flags);
}

#endif /* compat handlers */
#ifndef __sysif_server_fslt_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(dev_t, devno)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, mode)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, fs_type)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, opts)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_get_tree(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(dev_t, devno), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, mode), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, fs_type), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, opts));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(dev_t, devno)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, mode)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, fs_type)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, opts)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_get_tree(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(dev_t, devno), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, mode), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, fs_type), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, opts));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_kill_sb(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_kill_sb(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, iattr_buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_getattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, iattr_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, iattr_buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_getattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, iattr_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(void*, iattr)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_setattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, iattr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(void*, iattr)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_setattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, iattr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(long long, pos)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_readdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(long long, pos)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_readdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_lookup(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_lookup(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(mode_t, mode)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, uid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, gid)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_create(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(mode_t, mode)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, uid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, gid)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_create(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_unlink(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_unlink(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(long long, pos)
	} while (0);
	do {
		__adjust_one_arg(void*, iov)
	} while (0);
	do {
		__adjust_one_arg(int, iovcnt)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_write_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(long long, pos)
	} while (0);
	do {
		__adjust_one_arg(void*, iov)
	} while (0);
	do {
		__adjust_one_arg(int, iovcnt)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_write_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(long long, pos)
	} while (0);
	do {
		__adjust_one_arg(void*, iov)
	} while (0);
	do {
		__adjust_one_arg(int, iovcnt)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_read_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(long long, pos)
	} while (0);
	do {
		__adjust_one_arg(void*, iov)
	} while (0);
	do {
		__adjust_one_arg(int, iovcnt)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_read_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, cmd)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_ioctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cmd), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, cmd)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_ioctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cmd), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(mode_t, mode)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, uid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, gid)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_mkdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(mode_t, mode)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, uid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, gid)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_mkdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_rmdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, dir_ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, name_buf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_rmdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, old_instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, new_instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, old_dir_ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, new_dir_ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, old_name_buf)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, new_name_buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_rename(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, old_name_buf), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, new_name_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, old_instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, new_instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, old_dir_ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, new_dir_ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, old_name_buf)
	} while (0);
	do {
		__adjust_one_arg(struct hm_actv_buf, new_name_buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_rename(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, old_name_buf), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, new_name_buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_statfs(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_statfs(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_show_options(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_show_options(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_listxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_listxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(int, prefix)
	} while (0);
	do {
		__adjust_one_arg(const void*, name)
	} while (0);
	do {
		__adjust_one_arg(size_t, name_len)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_getxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(int, prefix)
	} while (0);
	do {
		__adjust_one_arg(const void*, name)
	} while (0);
	do {
		__adjust_one_arg(size_t, name_len)
	} while (0);
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buf_size)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_getxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(int, prefix)
	} while (0);
	do {
		__adjust_one_arg(const void*, name)
	} while (0);
	do {
		__adjust_one_arg(size_t, name_len)
	} while (0);
	do {
		__adjust_one_arg(const void*, value)
	} while (0);
	do {
		__adjust_one_arg(size_t, value_len)
	} while (0);
	do {
		__adjust_one_arg(int, flags)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_setxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, value), __ARCH_ACTV_RPC_RECV_ARG(size_t, value_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
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
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(int, prefix)
	} while (0);
	do {
		__adjust_one_arg(const void*, name)
	} while (0);
	do {
		__adjust_one_arg(size_t, name_len)
	} while (0);
	do {
		__adjust_one_arg(const void*, value)
	} while (0);
	do {
		__adjust_one_arg(size_t, value_len)
	} while (0);
	do {
		__adjust_one_arg(int, flags)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_setxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, value), __ARCH_ACTV_RPC_RECV_ARG(size_t, value_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_destroy_node(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_destroy_node(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_sync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_sync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_fsync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_fsync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, flags)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_flush(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned long long, instance)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, ino)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, flags)
	} while (0);
	return __actvshadowhdlr_fslthandler_transfs_flush(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_get_tree(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(dev_t, devno), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, mode), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, fs_type), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, opts));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_get_tree(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, dev_t devno, unsigned int mode, struct hm_actv_buf fs_type, struct hm_actv_buf opts)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(dev_t), __arch_actv_rpc_stksz_alignment(dev_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_get_tree(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(dev_t, devno), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, mode), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, fs_type), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, opts));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_kill_sb(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_kill_sb(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_kill_sb(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_getattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, iattr_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_getattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, struct hm_actv_buf iattr_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_getattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, iattr_buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *iattr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_setattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, iattr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_setattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *iattr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_setattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, iattr));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_readdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_readdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_readdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_lookup(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_lookup(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_lookup(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_create(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_create(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_create(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_unlink(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_unlink(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_unlink(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_write_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_write_iter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_write_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_read_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_read_iter(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, long long pos, void *iov, int iovcnt)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_read_iter(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(long long, pos), __ARCH_ACTV_RPC_RECV_ARG(void*, iov), __ARCH_ACTV_RPC_RECV_ARG(int, iovcnt));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_ioctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cmd), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_ioctl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int cmd, unsigned long arg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_ioctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cmd), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_mkdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_mkdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, mode_t mode, unsigned int uid, unsigned int gid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_mkdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(mode_t, mode), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, uid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, gid));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_rmdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_rmdir(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long dir_ino, struct hm_actv_buf name_buf, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_rmdir(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, dir_ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, name_buf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_rename(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, old_name_buf), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, new_name_buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_rename(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long old_instance, unsigned long long new_instance, unsigned long long old_dir_ino, unsigned long long new_dir_ino, unsigned long long ino, struct hm_actv_buf old_name_buf, struct hm_actv_buf new_name_buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_rename(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, old_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, new_dir_ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, old_name_buf), __ARCH_ACTV_RPC_RECV_ARG(struct hm_actv_buf, new_name_buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_statfs(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_statfs(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_statfs(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, unsigned long long instance, void *buf, size_t buf_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_show_options(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_show_options(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_show_options(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_listxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_listxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_listxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_getxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_getxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, void *buf, size_t buf_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_getxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buf_size));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_setxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, value), __ARCH_ACTV_RPC_RECV_ARG(size_t, value_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_setxattr(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, int prefix, const void *name, size_t name_len, const void *value, size_t value_len, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
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
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_setxattr(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(int, prefix), __ARCH_ACTV_RPC_RECV_ARG(const void*, name), __ARCH_ACTV_RPC_RECV_ARG(size_t, name_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, value), __ARCH_ACTV_RPC_RECV_ARG(size_t, value_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_destroy_node(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_destroy_node(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_destroy_node(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, unsigned long long instance)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_sync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_sync(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_sync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_fsync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_fsync(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_fsync(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino));
}

static int __opt_nonoframe __actvbridgehdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_flush(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fslthandler_transfs_flush(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned long long instance, unsigned long long ino, unsigned int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fslthandler_transfs_flush(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, instance), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, ino), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_fslt_no_compat32_handlers__) && defined(__sysif_server_fslt_no_ilp32_handlers__)
__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_fslthandler_transfs_get_tree = (&__actvbridgehdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getattr = (&__actvbridgehdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setattr = (&__actvbridgehdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_readdir = (&__actvbridgehdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_fslthandler_transfs_lookup = (&__actvbridgehdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_fslthandler_transfs_create = (&__actvbridgehdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_fslthandler_transfs_unlink = (&__actvbridgehdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_write_iter = (&__actvbridgehdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_read_iter = (&__actvbridgehdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_fslthandler_transfs_ioctl = (&__actvbridgehdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_mkdir = (&__actvbridgehdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_rmdir = (&__actvbridgehdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_fslthandler_transfs_rename = (&__actvbridgehdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_fslthandler_transfs_statfs = (&__actvbridgehdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_fslthandler_transfs_show_options = (&__actvbridgehdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_listxattr = (&__actvbridgehdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getxattr = (&__actvbridgehdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setxattr = (&__actvbridgehdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_fslthandler_transfs_sync = (&__actvbridgehdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_fslthandler_transfs_fsync = (&__actvbridgehdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_fslthandler_transfs_flush = (&__actvbridgehdlr_fslthandler_transfs_flush)
	}
};

__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvcapcallhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_fslthandler_transfs_flush)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_fslt_no_compat32_handlers__) && defined(__sysif_server_fslt_no_ilp32_handlers__)
__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_fslthandler_transfs_get_tree = (&__actvbridgehdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getattr = (&__actvbridgehdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setattr = (&__actvbridgehdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_readdir = (&__actvbridgehdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_fslthandler_transfs_lookup = (&__actvbridgehdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_fslthandler_transfs_create = (&__actvbridgehdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_fslthandler_transfs_unlink = (&__actvbridgehdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_write_iter = (&__actvbridgehdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_read_iter = (&__actvbridgehdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_fslthandler_transfs_ioctl = (&__actvbridgehdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_mkdir = (&__actvbridgehdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_rmdir = (&__actvbridgehdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_fslthandler_transfs_rename = (&__actvbridgehdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_fslthandler_transfs_statfs = (&__actvbridgehdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_fslthandler_transfs_show_options = (&__actvbridgehdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_listxattr = (&__actvbridgehdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getxattr = (&__actvbridgehdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setxattr = (&__actvbridgehdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_fslthandler_transfs_sync = (&__actvbridgehdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_fslthandler_transfs_fsync = (&__actvbridgehdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_fslthandler_transfs_flush = (&__actvbridgehdlr_fslthandler_transfs_flush)
	},
	[__fslt_method_MAX ... __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_get_tree = (&__actvbridgehdlr_compat_fslthandler_transfs_get_tree)
	},
	[__fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_compat_fslthandler_transfs_kill_sb)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_getattr = (&__actvbridgehdlr_compat_fslthandler_transfs_getattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_setattr = (&__actvbridgehdlr_compat_fslthandler_transfs_setattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_readdir = (&__actvbridgehdlr_compat_fslthandler_transfs_readdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_lookup = (&__actvbridgehdlr_compat_fslthandler_transfs_lookup)
	},
	[__fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_create = (&__actvbridgehdlr_compat_fslthandler_transfs_create)
	},
	[__fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_unlink = (&__actvbridgehdlr_compat_fslthandler_transfs_unlink)
	},
	[__fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_write_iter = (&__actvbridgehdlr_compat_fslthandler_transfs_write_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_read_iter = (&__actvbridgehdlr_compat_fslthandler_transfs_read_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_ioctl = (&__actvbridgehdlr_compat_fslthandler_transfs_ioctl)
	},
	[__fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_mkdir = (&__actvbridgehdlr_compat_fslthandler_transfs_mkdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_rmdir = (&__actvbridgehdlr_compat_fslthandler_transfs_rmdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_rename = (&__actvbridgehdlr_compat_fslthandler_transfs_rename)
	},
	[__fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_statfs = (&__actvbridgehdlr_compat_fslthandler_transfs_statfs)
	},
	[__fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_show_options = (&__actvbridgehdlr_compat_fslthandler_transfs_show_options)
	},
	[__fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_listxattr = (&__actvbridgehdlr_compat_fslthandler_transfs_listxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_getxattr = (&__actvbridgehdlr_compat_fslthandler_transfs_getxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_setxattr = (&__actvbridgehdlr_compat_fslthandler_transfs_setxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_compat_fslthandler_transfs_destroy_node)
	},
	[__fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_sync = (&__actvbridgehdlr_compat_fslthandler_transfs_sync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_fsync = (&__actvbridgehdlr_compat_fslthandler_transfs_fsync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_flush = (&__actvbridgehdlr_compat_fslthandler_transfs_flush)
	}
};

__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvcapcallhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_fslthandler_transfs_flush)
	},
	[__fslt_method_MAX ... __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree)
	},
	[__fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup)
	},
	[__fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_create)
	},
	[__fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink)
	},
	[__fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl)
	},
	[__fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_rename)
	},
	[__fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs)
	},
	[__fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options)
	},
	[__fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node)
	},
	[__fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_sync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_flush)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_fslt_no_compat32_handlers__) && !defined(__sysif_server_fslt_no_ilp32_handlers__)
__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_fslthandler_transfs_get_tree = (&__actvbridgehdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getattr = (&__actvbridgehdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setattr = (&__actvbridgehdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_readdir = (&__actvbridgehdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_fslthandler_transfs_lookup = (&__actvbridgehdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_fslthandler_transfs_create = (&__actvbridgehdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_fslthandler_transfs_unlink = (&__actvbridgehdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_write_iter = (&__actvbridgehdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_read_iter = (&__actvbridgehdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_fslthandler_transfs_ioctl = (&__actvbridgehdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_mkdir = (&__actvbridgehdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_rmdir = (&__actvbridgehdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_fslthandler_transfs_rename = (&__actvbridgehdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_fslthandler_transfs_statfs = (&__actvbridgehdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_fslthandler_transfs_show_options = (&__actvbridgehdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_listxattr = (&__actvbridgehdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getxattr = (&__actvbridgehdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setxattr = (&__actvbridgehdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_fslthandler_transfs_sync = (&__actvbridgehdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_fslthandler_transfs_fsync = (&__actvbridgehdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_fslthandler_transfs_flush = (&__actvbridgehdlr_fslthandler_transfs_flush)
	},
	[__fslt_method_MAX ... __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_fslthandler_transfs_get_tree = (&__actvbridgehdlr_ilp32_fslthandler_transfs_get_tree)
	},
	[__fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_ilp32_fslthandler_transfs_kill_sb)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_getattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_setattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_readdir = (&__actvbridgehdlr_ilp32_fslthandler_transfs_readdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_fslthandler_transfs_lookup = (&__actvbridgehdlr_ilp32_fslthandler_transfs_lookup)
	},
	[__fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_fslthandler_transfs_create = (&__actvbridgehdlr_ilp32_fslthandler_transfs_create)
	},
	[__fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_fslthandler_transfs_unlink = (&__actvbridgehdlr_ilp32_fslthandler_transfs_unlink)
	},
	[__fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_write_iter = (&__actvbridgehdlr_ilp32_fslthandler_transfs_write_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_read_iter = (&__actvbridgehdlr_ilp32_fslthandler_transfs_read_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_fslthandler_transfs_ioctl = (&__actvbridgehdlr_ilp32_fslthandler_transfs_ioctl)
	},
	[__fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_mkdir = (&__actvbridgehdlr_ilp32_fslthandler_transfs_mkdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_rmdir = (&__actvbridgehdlr_ilp32_fslthandler_transfs_rmdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_fslthandler_transfs_rename = (&__actvbridgehdlr_ilp32_fslthandler_transfs_rename)
	},
	[__fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_fslthandler_transfs_statfs = (&__actvbridgehdlr_ilp32_fslthandler_transfs_statfs)
	},
	[__fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_fslthandler_transfs_show_options = (&__actvbridgehdlr_ilp32_fslthandler_transfs_show_options)
	},
	[__fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_listxattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_listxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getxattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_getxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setxattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_setxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_ilp32_fslthandler_transfs_destroy_node)
	},
	[__fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_fslthandler_transfs_sync = (&__actvbridgehdlr_ilp32_fslthandler_transfs_sync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_fslthandler_transfs_fsync = (&__actvbridgehdlr_ilp32_fslthandler_transfs_fsync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_fslthandler_transfs_flush = (&__actvbridgehdlr_ilp32_fslthandler_transfs_flush)
	}
};

__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvcapcallhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_fslthandler_transfs_flush)
	},
	[__fslt_method_MAX ... __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_get_tree)
	},
	[__fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_kill_sb)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_getattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_setattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_readdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_lookup)
	},
	[__fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_create)
	},
	[__fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_unlink)
	},
	[__fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_write_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_read_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_ioctl)
	},
	[__fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_mkdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_rmdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_rename)
	},
	[__fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_statfs)
	},
	[__fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_show_options)
	},
	[__fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_listxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_getxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_setxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_destroy_node)
	},
	[__fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_sync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_fsync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_flush)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_fslt_no_compat32_handlers__) && !defined(__sysif_server_fslt_no_ilp32_handlers__)
__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_fslthandler_transfs_get_tree = (&__actvbridgehdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getattr = (&__actvbridgehdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setattr = (&__actvbridgehdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_readdir = (&__actvbridgehdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_fslthandler_transfs_lookup = (&__actvbridgehdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_fslthandler_transfs_create = (&__actvbridgehdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_fslthandler_transfs_unlink = (&__actvbridgehdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_write_iter = (&__actvbridgehdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_read_iter = (&__actvbridgehdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_fslthandler_transfs_ioctl = (&__actvbridgehdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_mkdir = (&__actvbridgehdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_rmdir = (&__actvbridgehdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_fslthandler_transfs_rename = (&__actvbridgehdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_fslthandler_transfs_statfs = (&__actvbridgehdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_fslthandler_transfs_show_options = (&__actvbridgehdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_listxattr = (&__actvbridgehdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getxattr = (&__actvbridgehdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setxattr = (&__actvbridgehdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_fslthandler_transfs_sync = (&__actvbridgehdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_fslthandler_transfs_fsync = (&__actvbridgehdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_fslthandler_transfs_flush = (&__actvbridgehdlr_fslthandler_transfs_flush)
	},
	[__fslt_method_MAX ... __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_get_tree = (&__actvbridgehdlr_compat_fslthandler_transfs_get_tree)
	},
	[__fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_compat_fslthandler_transfs_kill_sb)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_getattr = (&__actvbridgehdlr_compat_fslthandler_transfs_getattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_setattr = (&__actvbridgehdlr_compat_fslthandler_transfs_setattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_readdir = (&__actvbridgehdlr_compat_fslthandler_transfs_readdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_lookup = (&__actvbridgehdlr_compat_fslthandler_transfs_lookup)
	},
	[__fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_create = (&__actvbridgehdlr_compat_fslthandler_transfs_create)
	},
	[__fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_unlink = (&__actvbridgehdlr_compat_fslthandler_transfs_unlink)
	},
	[__fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_write_iter = (&__actvbridgehdlr_compat_fslthandler_transfs_write_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_read_iter = (&__actvbridgehdlr_compat_fslthandler_transfs_read_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_ioctl = (&__actvbridgehdlr_compat_fslthandler_transfs_ioctl)
	},
	[__fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_mkdir = (&__actvbridgehdlr_compat_fslthandler_transfs_mkdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_rmdir = (&__actvbridgehdlr_compat_fslthandler_transfs_rmdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_rename = (&__actvbridgehdlr_compat_fslthandler_transfs_rename)
	},
	[__fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_statfs = (&__actvbridgehdlr_compat_fslthandler_transfs_statfs)
	},
	[__fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_show_options = (&__actvbridgehdlr_compat_fslthandler_transfs_show_options)
	},
	[__fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_listxattr = (&__actvbridgehdlr_compat_fslthandler_transfs_listxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_getxattr = (&__actvbridgehdlr_compat_fslthandler_transfs_getxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_setxattr = (&__actvbridgehdlr_compat_fslthandler_transfs_setxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_compat_fslthandler_transfs_destroy_node)
	},
	[__fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_sync = (&__actvbridgehdlr_compat_fslthandler_transfs_sync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_fsync = (&__actvbridgehdlr_compat_fslthandler_transfs_fsync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_compat_fslthandler_transfs_flush = (&__actvbridgehdlr_compat_fslthandler_transfs_flush)
	},
	[2 * __fslt_method_MAX ... 2 * __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgehdlr_fslthandler_transfs_get_tree = (&__actvbridgehdlr_ilp32_fslthandler_transfs_get_tree)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgehdlr_fslthandler_transfs_kill_sb = (&__actvbridgehdlr_ilp32_fslthandler_transfs_kill_sb)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_getattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_setattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_readdir = (&__actvbridgehdlr_ilp32_fslthandler_transfs_readdir)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgehdlr_fslthandler_transfs_lookup = (&__actvbridgehdlr_ilp32_fslthandler_transfs_lookup)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgehdlr_fslthandler_transfs_create = (&__actvbridgehdlr_ilp32_fslthandler_transfs_create)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgehdlr_fslthandler_transfs_unlink = (&__actvbridgehdlr_ilp32_fslthandler_transfs_unlink)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_write_iter = (&__actvbridgehdlr_ilp32_fslthandler_transfs_write_iter)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgehdlr_fslthandler_transfs_read_iter = (&__actvbridgehdlr_ilp32_fslthandler_transfs_read_iter)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgehdlr_fslthandler_transfs_ioctl = (&__actvbridgehdlr_ilp32_fslthandler_transfs_ioctl)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_mkdir = (&__actvbridgehdlr_ilp32_fslthandler_transfs_mkdir)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgehdlr_fslthandler_transfs_rmdir = (&__actvbridgehdlr_ilp32_fslthandler_transfs_rmdir)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgehdlr_fslthandler_transfs_rename = (&__actvbridgehdlr_ilp32_fslthandler_transfs_rename)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgehdlr_fslthandler_transfs_statfs = (&__actvbridgehdlr_ilp32_fslthandler_transfs_statfs)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgehdlr_fslthandler_transfs_show_options = (&__actvbridgehdlr_ilp32_fslthandler_transfs_show_options)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_listxattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_listxattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_getxattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_getxattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgehdlr_fslthandler_transfs_setxattr = (&__actvbridgehdlr_ilp32_fslthandler_transfs_setxattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgehdlr_fslthandler_transfs_destroy_node = (&__actvbridgehdlr_ilp32_fslthandler_transfs_destroy_node)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgehdlr_fslthandler_transfs_sync = (&__actvbridgehdlr_ilp32_fslthandler_transfs_sync)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgehdlr_fslthandler_transfs_fsync = (&__actvbridgehdlr_ilp32_fslthandler_transfs_fsync)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgehdlr_fslthandler_transfs_flush = (&__actvbridgehdlr_ilp32_fslthandler_transfs_flush)
	}
};

__ro_after_init_hdlr union fslt_hdlrs_union sysif_fslt_actvcapcallhdlr_table[__fslt_method_MAX * __fslt_method_TIMES] = {
	[0 ... __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_fslthandler_transfs_get_tree)
	},
	[ __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb)
	},
	[ __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getattr)
	},
	[ __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setattr)
	},
	[ __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_readdir)
	},
	[ __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_fslthandler_transfs_lookup)
	},
	[ __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_fslthandler_transfs_create)
	},
	[ __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_fslthandler_transfs_unlink)
	},
	[ __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_write_iter)
	},
	[ __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_fslthandler_transfs_read_iter)
	},
	[ __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_fslthandler_transfs_ioctl)
	},
	[ __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_mkdir)
	},
	[ __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_fslthandler_transfs_rmdir)
	},
	[ __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_fslthandler_transfs_rename)
	},
	[ __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_fslthandler_transfs_statfs)
	},
	[ __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_fslthandler_transfs_show_options)
	},
	[ __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_listxattr)
	},
	[ __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_getxattr)
	},
	[ __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_fslthandler_transfs_setxattr)
	},
	[ __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node)
	},
	[ __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_fslthandler_transfs_sync)
	},
	[ __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_fslthandler_transfs_fsync)
	},
	[ __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_fslthandler_transfs_flush)
	},
	[__fslt_method_MAX ... __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_get_tree)
	},
	[__fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_kill_sb)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_getattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_setattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_readdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_lookup)
	},
	[__fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_create)
	},
	[__fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_unlink)
	},
	[__fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_write_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_read_iter)
	},
	[__fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_ioctl)
	},
	[__fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_mkdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_rmdir)
	},
	[__fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_rename)
	},
	[__fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_statfs)
	},
	[__fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_show_options)
	},
	[__fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_listxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_getxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_setxattr)
	},
	[__fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_destroy_node)
	},
	[__fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_sync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_fsync)
	},
	[__fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_compat_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_compat_fslthandler_transfs_flush)
	},
	[2 * __fslt_method_MAX ... 2 * __fslt_method_MAX + __fslt_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_get_tree] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_get_tree = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_get_tree)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_kill_sb] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_kill_sb = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_kill_sb)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_getattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_getattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_setattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_setattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_readdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_readdir = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_readdir)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_lookup] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_lookup = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_lookup)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_create] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_create = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_create)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_unlink] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_unlink = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_unlink)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_write_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_write_iter = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_write_iter)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_read_iter] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_read_iter = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_read_iter)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_ioctl] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_ioctl = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_ioctl)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_mkdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_mkdir = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_mkdir)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_rmdir] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rmdir = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_rmdir)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_rename] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_rename = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_rename)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_statfs] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_statfs = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_statfs)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_show_options] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_show_options = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_show_options)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_listxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_listxattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_listxattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_getxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_getxattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_getxattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_setxattr] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_setxattr = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_setxattr)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_destroy_node] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_destroy_node = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_destroy_node)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_sync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_sync = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_sync)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_fsync] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_fsync = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_fsync)
	},
	[2 * __fslt_method_MAX + __fslt_method_transfs_flush] = {
		.p__actvbridgecapcallhdlr_fslthandler_transfs_flush = (&__actvbridgecapcallhdlr_ilp32_fslthandler_transfs_flush)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_fslt_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "get_tree", __fslt_method_transfs_get_tree);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "get_tree", __fslt_method_transfs_get_tree);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "kill_sb", __fslt_method_transfs_kill_sb);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "kill_sb", __fslt_method_transfs_kill_sb);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "getattr", __fslt_method_transfs_getattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "getattr", __fslt_method_transfs_getattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "setattr", __fslt_method_transfs_setattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "setattr", __fslt_method_transfs_setattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "readdir", __fslt_method_transfs_readdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "readdir", __fslt_method_transfs_readdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "lookup", __fslt_method_transfs_lookup);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "lookup", __fslt_method_transfs_lookup);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "create", __fslt_method_transfs_create);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "create", __fslt_method_transfs_create);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "unlink", __fslt_method_transfs_unlink);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "unlink", __fslt_method_transfs_unlink);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "write_iter", __fslt_method_transfs_write_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "write_iter", __fslt_method_transfs_write_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "read_iter", __fslt_method_transfs_read_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "read_iter", __fslt_method_transfs_read_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "ioctl", __fslt_method_transfs_ioctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "ioctl", __fslt_method_transfs_ioctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "mkdir", __fslt_method_transfs_mkdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "mkdir", __fslt_method_transfs_mkdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "rmdir", __fslt_method_transfs_rmdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "rmdir", __fslt_method_transfs_rmdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "rename", __fslt_method_transfs_rename);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "rename", __fslt_method_transfs_rename);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "statfs", __fslt_method_transfs_statfs);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "statfs", __fslt_method_transfs_statfs);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "show_options", __fslt_method_transfs_show_options);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "show_options", __fslt_method_transfs_show_options);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "listxattr", __fslt_method_transfs_listxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "listxattr", __fslt_method_transfs_listxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "getxattr", __fslt_method_transfs_getxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "getxattr", __fslt_method_transfs_getxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "setxattr", __fslt_method_transfs_setxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "setxattr", __fslt_method_transfs_setxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "destroy_node", __fslt_method_transfs_destroy_node);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "destroy_node", __fslt_method_transfs_destroy_node);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "sync", __fslt_method_transfs_sync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "sync", __fslt_method_transfs_sync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "fsync", __fslt_method_transfs_fsync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "fsync", __fslt_method_transfs_fsync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "flush", __fslt_method_transfs_flush);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "flush", __fslt_method_transfs_flush);
	hm_info("%s_%s, max method num: %d\n", "fslt", "method", __fslt_method_MAX);
}

#elif defined(__arm__)
void sysif_show_fslt_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "get_tree", __fslt_method_transfs_get_tree);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "get_tree", __fslt_method_transfs_get_tree);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "kill_sb", __fslt_method_transfs_kill_sb);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "kill_sb", __fslt_method_transfs_kill_sb);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "getattr", __fslt_method_transfs_getattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "getattr", __fslt_method_transfs_getattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "setattr", __fslt_method_transfs_setattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "setattr", __fslt_method_transfs_setattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "readdir", __fslt_method_transfs_readdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "readdir", __fslt_method_transfs_readdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "lookup", __fslt_method_transfs_lookup);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "lookup", __fslt_method_transfs_lookup);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "create", __fslt_method_transfs_create);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "create", __fslt_method_transfs_create);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "unlink", __fslt_method_transfs_unlink);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "unlink", __fslt_method_transfs_unlink);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "write_iter", __fslt_method_transfs_write_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "write_iter", __fslt_method_transfs_write_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "read_iter", __fslt_method_transfs_read_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "read_iter", __fslt_method_transfs_read_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "ioctl", __fslt_method_transfs_ioctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "ioctl", __fslt_method_transfs_ioctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "mkdir", __fslt_method_transfs_mkdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "mkdir", __fslt_method_transfs_mkdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "rmdir", __fslt_method_transfs_rmdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "rmdir", __fslt_method_transfs_rmdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "rename", __fslt_method_transfs_rename);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "rename", __fslt_method_transfs_rename);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "statfs", __fslt_method_transfs_statfs);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "statfs", __fslt_method_transfs_statfs);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "show_options", __fslt_method_transfs_show_options);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "show_options", __fslt_method_transfs_show_options);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "listxattr", __fslt_method_transfs_listxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "listxattr", __fslt_method_transfs_listxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "getxattr", __fslt_method_transfs_getxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "getxattr", __fslt_method_transfs_getxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "setxattr", __fslt_method_transfs_setxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "setxattr", __fslt_method_transfs_setxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "destroy_node", __fslt_method_transfs_destroy_node);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "destroy_node", __fslt_method_transfs_destroy_node);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "sync", __fslt_method_transfs_sync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "sync", __fslt_method_transfs_sync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "fsync", __fslt_method_transfs_fsync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "fsync", __fslt_method_transfs_fsync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "flush", __fslt_method_transfs_flush);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "flush", __fslt_method_transfs_flush);
	hm_info("%s_%s, max method num: %d\n", "fslt", "method", __fslt_method_MAX);
}

#else
void sysif_show_fslt_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "get_tree", __fslt_method_transfs_get_tree);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "get_tree", __fslt_method_transfs_get_tree);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "kill_sb", __fslt_method_transfs_kill_sb);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "kill_sb", __fslt_method_transfs_kill_sb);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "getattr", __fslt_method_transfs_getattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "getattr", __fslt_method_transfs_getattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "setattr", __fslt_method_transfs_setattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "setattr", __fslt_method_transfs_setattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "readdir", __fslt_method_transfs_readdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "readdir", __fslt_method_transfs_readdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "lookup", __fslt_method_transfs_lookup);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "lookup", __fslt_method_transfs_lookup);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "create", __fslt_method_transfs_create);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "create", __fslt_method_transfs_create);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "unlink", __fslt_method_transfs_unlink);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "unlink", __fslt_method_transfs_unlink);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "write_iter", __fslt_method_transfs_write_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "write_iter", __fslt_method_transfs_write_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "read_iter", __fslt_method_transfs_read_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "read_iter", __fslt_method_transfs_read_iter);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "ioctl", __fslt_method_transfs_ioctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "ioctl", __fslt_method_transfs_ioctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "mkdir", __fslt_method_transfs_mkdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "mkdir", __fslt_method_transfs_mkdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "rmdir", __fslt_method_transfs_rmdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "rmdir", __fslt_method_transfs_rmdir);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "rename", __fslt_method_transfs_rename);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "rename", __fslt_method_transfs_rename);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "statfs", __fslt_method_transfs_statfs);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "statfs", __fslt_method_transfs_statfs);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "show_options", __fslt_method_transfs_show_options);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "show_options", __fslt_method_transfs_show_options);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "listxattr", __fslt_method_transfs_listxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "listxattr", __fslt_method_transfs_listxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "getxattr", __fslt_method_transfs_getxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "getxattr", __fslt_method_transfs_getxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "setxattr", __fslt_method_transfs_setxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "setxattr", __fslt_method_transfs_setxattr);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "destroy_node", __fslt_method_transfs_destroy_node);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "destroy_node", __fslt_method_transfs_destroy_node);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "sync", __fslt_method_transfs_sync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "sync", __fslt_method_transfs_sync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "fsync", __fslt_method_transfs_fsync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "fsync", __fslt_method_transfs_fsync);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fsltcall", "transfs", "flush", __fslt_method_transfs_flush);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fslthandler", "transfs", "flush", __fslt_method_transfs_flush);
	hm_info("%s_%s, max method num: %d\n", "fslt", "method", __fslt_method_MAX);
}

#endif
#endif
#endif
