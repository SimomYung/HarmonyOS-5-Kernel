/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for keymgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/keymgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/keymgr/libsysif/keymgr/api.h --server-dest ../image/usr/include/generated/sysif_server/keymgr/libsysif/keymgr/server.h --dump-callno ../image/usr/include/generated/dump_info/keymgr/libsysif/keymgr/l2h_map.h --legacy-mode
* Create: Mon Sep 22 11:44:32 2025
*/
#ifndef LIBSYSIF_KEYMGR_API_H
#define LIBSYSIF_KEYMGR_API_H
#ifndef LIBSYSIF_KEYMGR_COMMON_H
#define LIBSYSIF_KEYMGR_COMMON_H
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
#include <libhmsrv_key/keymgr.h>

#include <sys/types.h>
#include <libhmsrv_key/key.h>

struct __actvret_keycall_keymgr_fscrypt_ctrl {
};

struct __actvret_keycall_keymgr_fsverity_ctrl {
};

struct __actvret_keycall_keymgr_procfs_read {
};

struct __actvret_keycall_keymgr_procfs_write {
};

struct __actvret_keycall_keymgr_add_key {
};

struct __actvret_keycall_keymgr_request_key {
};

struct __actvret_keycall_keymgr_keyctl {
};

#ifdef __aarch64__
union __actvret_keycall {
	struct __actvret_keycall_keymgr_fscrypt_ctrl keymgr_fscrypt_ctrl;
	struct __actvret_keycall_keymgr_fsverity_ctrl keymgr_fsverity_ctrl;
	struct __actvret_keycall_keymgr_procfs_read keymgr_procfs_read;
	struct __actvret_keycall_keymgr_procfs_write keymgr_procfs_write;
	struct __actvret_keycall_keymgr_add_key keymgr_add_key;
	struct __actvret_keycall_keymgr_request_key keymgr_request_key;
	struct __actvret_keycall_keymgr_keyctl keymgr_keyctl;
};

#elif defined(__arm__)
union __actvret_keycall {
	struct __actvret_keycall_keymgr_fscrypt_ctrl keymgr_fscrypt_ctrl;
	struct __actvret_keycall_keymgr_fsverity_ctrl keymgr_fsverity_ctrl;
	struct __actvret_keycall_keymgr_procfs_read keymgr_procfs_read;
	struct __actvret_keycall_keymgr_procfs_write keymgr_procfs_write;
	struct __actvret_keycall_keymgr_add_key keymgr_add_key;
	struct __actvret_keycall_keymgr_request_key keymgr_request_key;
	struct __actvret_keycall_keymgr_keyctl keymgr_keyctl;
};

#else
union __actvret_keycall {
	struct __actvret_keycall_keymgr_fscrypt_ctrl keymgr_fscrypt_ctrl;
	struct __actvret_keycall_keymgr_fsverity_ctrl keymgr_fsverity_ctrl;
	struct __actvret_keycall_keymgr_procfs_read keymgr_procfs_read;
	struct __actvret_keycall_keymgr_procfs_write keymgr_procfs_write;
	struct __actvret_keycall_keymgr_add_key keymgr_add_key;
	struct __actvret_keycall_keymgr_request_key keymgr_request_key;
	struct __actvret_keycall_keymgr_keyctl keymgr_keyctl;
};

#endif
enum __keymgr_mgr {
	__keymgr_INVAL_mgr=0,
	__keymgr_keymgr_mgr,
	__keymgr_MAX_mgr,
};

#define __keymgr_method_MIN __ACTV_HIGHTABLE_START
#define __keymgr_method_keymgr_add_key (__ACTV_HIGHTABLE_START + 1)
#define __keymgr_method_keymgr_request_key (__ACTV_HIGHTABLE_START + 2)
#define __keymgr_method_keymgr_keyctl (__ACTV_HIGHTABLE_START + 3)
#define __keymgr_method_keymgr_fscrypt_ctrl (__ACTV_HIGHTABLE_START + 4)
#define __keymgr_method_keymgr_fsverity_ctrl (__ACTV_HIGHTABLE_START + 5)
#define __keymgr_method_keymgr_procfs_read (__ACTV_HIGHTABLE_START + 6)
#define __keymgr_method_keymgr_procfs_write (__ACTV_HIGHTABLE_START + 7)
#define __keymgr_method_MAX (__ACTV_HIGHTABLE_START + 8)
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_fscrypt_ctrl) < 512,
            keycall_keymgr_fscrypt_ctrl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_fsverity_ctrl) < 512,
            keycall_keymgr_fsverity_ctrl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_procfs_read) < 512,
            keycall_keymgr_procfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_procfs_write) < 512,
            keycall_keymgr_procfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_add_key) < 512,
            keycall_keymgr_add_key_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_request_key) < 512,
            keycall_keymgr_request_key_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_keyctl) < 512,
            keycall_keymgr_keyctl_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_keycall) < 512,
        keycall_too_many_actvret_data);
extern int __actvcall_keycall_keymgr_fscrypt_ctrl(unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size);


extern int __actvcapcall_keycall_keymgr_fscrypt_ctrl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size);


static inline int __attribute__((always_inline)) actvxactcall_keycall_keymgr_fscrypt_ctrl(bool is_xact_fwd, bool is_xact_allow_refwd, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	const unsigned long __callno = __keymgr_method_keymgr_fscrypt_ctrl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_keycall_keymgr_fscrypt_ctrl(rpc_info, NULL, operation, arg_addr, arg_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_keycall_keymgr_fscrypt_ctrl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	const unsigned long __callno = __keymgr_method_keymgr_fscrypt_ctrl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_keycall_keymgr_fscrypt_ctrl(__rref, rpc_info, NULL, operation, arg_addr, arg_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_keycall_keymgr_fscrypt_ctrl(int operation, unsigned long arg_addr, unsigned long arg_size)
{
	return actvxactcall_keycall_keymgr_fscrypt_ctrl(false, false, operation, arg_addr, arg_size);
}

static inline int __attribute__((always_inline)) actvcapcall_keycall_keymgr_fscrypt_ctrl(rref_t __rref, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	return actvxactcapcall_keycall_keymgr_fscrypt_ctrl(false, false, __rref, operation, arg_addr, arg_size);
}

extern int __actvcall_keycall_keymgr_fsverity_ctrl(unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size);


extern int __actvcapcall_keycall_keymgr_fsverity_ctrl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size);


static inline int __attribute__((always_inline)) actvxactcall_keycall_keymgr_fsverity_ctrl(bool is_xact_fwd, bool is_xact_allow_refwd, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	const unsigned long __callno = __keymgr_method_keymgr_fsverity_ctrl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_keycall_keymgr_fsverity_ctrl(rpc_info, NULL, operation, arg_addr, arg_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_keycall_keymgr_fsverity_ctrl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	const unsigned long __callno = __keymgr_method_keymgr_fsverity_ctrl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_keycall_keymgr_fsverity_ctrl(__rref, rpc_info, NULL, operation, arg_addr, arg_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_keycall_keymgr_fsverity_ctrl(int operation, unsigned long arg_addr, unsigned long arg_size)
{
	return actvxactcall_keycall_keymgr_fsverity_ctrl(false, false, operation, arg_addr, arg_size);
}

static inline int __attribute__((always_inline)) actvcapcall_keycall_keymgr_fsverity_ctrl(rref_t __rref, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	return actvxactcapcall_keycall_keymgr_fsverity_ctrl(false, false, __rref, operation, arg_addr, arg_size);
}

extern int __actvcall_keycall_keymgr_procfs_read(unsigned long long __rpc_info, void *__buf, int file_id, char *ubuf, unsigned long len);


extern int __actvcapcall_keycall_keymgr_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, int file_id, char *ubuf, unsigned long len);


static inline int __attribute__((always_inline)) actvxactcall_keycall_keymgr_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, int file_id, char *ubuf, unsigned long len)
{
	const unsigned long __callno = __keymgr_method_keymgr_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_keycall_keymgr_procfs_read(rpc_info, NULL, file_id, ubuf, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_keycall_keymgr_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int file_id, char *ubuf, unsigned long len)
{
	const unsigned long __callno = __keymgr_method_keymgr_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_keycall_keymgr_procfs_read(__rref, rpc_info, NULL, file_id, ubuf, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_keycall_keymgr_procfs_read(int file_id, char *ubuf, unsigned long len)
{
	return actvxactcall_keycall_keymgr_procfs_read(false, false, file_id, ubuf, len);
}

static inline int __attribute__((always_inline)) actvcapcall_keycall_keymgr_procfs_read(rref_t __rref, int file_id, char *ubuf, unsigned long len)
{
	return actvxactcapcall_keycall_keymgr_procfs_read(false, false, __rref, file_id, ubuf, len);
}

extern int __actvcall_keycall_keymgr_procfs_write(unsigned long long __rpc_info, void *__buf, int file_id, const char *ubuf, unsigned long len);


extern int __actvcapcall_keycall_keymgr_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, int file_id, const char *ubuf, unsigned long len);


static inline int __attribute__((always_inline)) actvxactcall_keycall_keymgr_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, int file_id, const char *ubuf, unsigned long len)
{
	const unsigned long __callno = __keymgr_method_keymgr_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_keycall_keymgr_procfs_write(rpc_info, NULL, file_id, ubuf, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_keycall_keymgr_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int file_id, const char *ubuf, unsigned long len)
{
	const unsigned long __callno = __keymgr_method_keymgr_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_keycall_keymgr_procfs_write(__rref, rpc_info, NULL, file_id, ubuf, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_keycall_keymgr_procfs_write(int file_id, const char *ubuf, unsigned long len)
{
	return actvxactcall_keycall_keymgr_procfs_write(false, false, file_id, ubuf, len);
}

static inline int __attribute__((always_inline)) actvcapcall_keycall_keymgr_procfs_write(rref_t __rref, int file_id, const char *ubuf, unsigned long len)
{
	return actvxactcapcall_keycall_keymgr_procfs_write(false, false, __rref, file_id, ubuf, len);
}

extern int __lsyscall_keymgr_add_key(long syscall_num, const char *type, const char *description, const void *payload, size_t plen, int keyring);


static inline int __attribute__((always_inline)) lsyscall_keymgr_add_key(const char *type, const char *description, const void *payload, size_t plen, int keyring)
{
	return __lsyscall_keymgr_add_key(SYS_add_key, type, description, payload, plen, keyring);
}

extern int __lsyscall_keymgr_request_key(long syscall_num, const char *type, const char *description, const char *callout_info, int keyring);


static inline int __attribute__((always_inline)) lsyscall_keymgr_request_key(const char *type, const char *description, const char *callout_info, int keyring)
{
	return __lsyscall_keymgr_request_key(SYS_request_key, type, description, callout_info, keyring);
}

extern int __lsyscall_keymgr_keyctl(long syscall_num, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);


static inline int __attribute__((always_inline)) lsyscall_keymgr_keyctl(int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5)
{
	return __lsyscall_keymgr_keyctl(SYS_keyctl, operation, arg2, arg3, arg4, arg5);
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

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_keycall_keymgr_fscrypt_ctrl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_keycall_keymgr_fscrypt_ctrl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_keycall_keymgr_fscrypt_ctrl(unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	int __ret;
	int __maybe_unused __actv_local_var_operation = operation;
	unsigned long __maybe_unused __actv_local_var_arg_addr = arg_addr;
	unsigned long __maybe_unused __actv_local_var_arg_size = arg_size;
	__ret = ____actvcall_keycall_keymgr_fscrypt_ctrl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(operation, __actv_local_var_operation), __ARCH_ACTV_RPC_PASS_ARG(arg_addr, __actv_local_var_arg_addr), __ARCH_ACTV_RPC_PASS_ARG(arg_size, __actv_local_var_arg_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_keycall_keymgr_fscrypt_ctrl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	int __ret;
	int __maybe_unused __actv_local_var_operation = operation;
	unsigned long __maybe_unused __actv_local_var_arg_addr = arg_addr;
	unsigned long __maybe_unused __actv_local_var_arg_size = arg_size;
	__ret = ____actvcapcall_keycall_keymgr_fscrypt_ctrl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(operation, __actv_local_var_operation), __ARCH_ACTV_RPC_PASS_ARG(arg_addr, __actv_local_var_arg_addr), __ARCH_ACTV_RPC_PASS_ARG(arg_size, __actv_local_var_arg_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_keycall_keymgr_fsverity_ctrl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_keycall_keymgr_fsverity_ctrl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_keycall_keymgr_fsverity_ctrl(unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	int __ret;
	int __maybe_unused __actv_local_var_operation = operation;
	unsigned long __maybe_unused __actv_local_var_arg_addr = arg_addr;
	unsigned long __maybe_unused __actv_local_var_arg_size = arg_size;
	__ret = ____actvcall_keycall_keymgr_fsverity_ctrl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(operation, __actv_local_var_operation), __ARCH_ACTV_RPC_PASS_ARG(arg_addr, __actv_local_var_arg_addr), __ARCH_ACTV_RPC_PASS_ARG(arg_size, __actv_local_var_arg_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_keycall_keymgr_fsverity_ctrl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	int __ret;
	int __maybe_unused __actv_local_var_operation = operation;
	unsigned long __maybe_unused __actv_local_var_arg_addr = arg_addr;
	unsigned long __maybe_unused __actv_local_var_arg_size = arg_size;
	__ret = ____actvcapcall_keycall_keymgr_fsverity_ctrl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(operation, __actv_local_var_operation), __ARCH_ACTV_RPC_PASS_ARG(arg_addr, __actv_local_var_arg_addr), __ARCH_ACTV_RPC_PASS_ARG(arg_size, __actv_local_var_arg_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_keycall_keymgr_procfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_keycall_keymgr_procfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_keycall_keymgr_procfs_read(unsigned long long __rpc_info, void *__buf, int file_id, char *ubuf, unsigned long len)
{
	int __ret;
	int __maybe_unused __actv_local_var_file_id = file_id;
	__maybe_unused char *__actv_local_var_ubuf = ubuf;
	unsigned long __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_keycall_keymgr_procfs_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_keycall_keymgr_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, int file_id, char *ubuf, unsigned long len)
{
	int __ret;
	int __maybe_unused __actv_local_var_file_id = file_id;
	__maybe_unused char *__actv_local_var_ubuf = ubuf;
	unsigned long __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_keycall_keymgr_procfs_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_keycall_keymgr_procfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_keycall_keymgr_procfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_keycall_keymgr_procfs_write(unsigned long long __rpc_info, void *__buf, int file_id, const char *ubuf, unsigned long len)
{
	int __ret;
	int __maybe_unused __actv_local_var_file_id = file_id;
	__maybe_unused const char *__actv_local_var_ubuf = ubuf;
	unsigned long __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_keycall_keymgr_procfs_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_keycall_keymgr_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, int file_id, const char *ubuf, unsigned long len)
{
	int __ret;
	int __maybe_unused __actv_local_var_file_id = file_id;
	__maybe_unused const char *__actv_local_var_ubuf = ubuf;
	unsigned long __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_keycall_keymgr_procfs_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __lsyscall_keymgr_add_key(long syscall_num, const char *type, const char *description, const void *payload, size_t plen, int keyring)
{
	long ret = lsyscall_syscall5(syscall_num, (long)type, (long)description, (long)payload, (long)plen, (long)keyring);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

int __lsyscall_keymgr_request_key(long syscall_num, const char *type, const char *description, const char *callout_info, int keyring)
{
	long ret = lsyscall_syscall4(syscall_num, (long)type, (long)description, (long)callout_info, (long)keyring);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

int __lsyscall_keymgr_keyctl(long syscall_num, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5)
{
	long ret = lsyscall_syscall5(syscall_num, (long)operation, (long)arg2, (long)arg3, (long)arg4, (long)arg5);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

#endif
#endif
