/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for keymgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/keymgr/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/keymgr/libsysif/keymgr/api.h --server-dest ../image/usr/include/generated/sysif_server/keymgr/libsysif/keymgr/server.h --dump-callno ../image/usr/include/generated/dump_info/keymgr/libsysif/keymgr/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:27:09 2025
*/
#ifndef LIBSYSIF_KEYMGR_SERVER_H
#define LIBSYSIF_KEYMGR_SERVER_H
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

struct __actvret_keycall_keymgr_krctl {
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
	struct __actvret_keycall_keymgr_krctl keymgr_krctl;
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
	struct __actvret_keycall_keymgr_krctl keymgr_krctl;
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
	struct __actvret_keycall_keymgr_krctl keymgr_krctl;
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
#define __keymgr_method_keymgr_krctl (__ACTV_HIGHTABLE_START + 8)
#define __keymgr_method_keymgr_rsvd_1 (__ACTV_HIGHTABLE_START + 9)
#define __keymgr_method_MAX (__ACTV_HIGHTABLE_START + 10)
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_fscrypt_ctrl) < 512,
            keycall_keymgr_fscrypt_ctrl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_fsverity_ctrl) < 512,
            keycall_keymgr_fsverity_ctrl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_procfs_read) < 512,
            keycall_keymgr_procfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_procfs_write) < 512,
            keycall_keymgr_procfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_keycall_keymgr_krctl) < 512,
            keycall_keymgr_krctl_too_many_actvret_data);
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

extern int __actvcall_keycall_keymgr_krctl(unsigned long long __rpc_info, void *__buf, int operation, void *keyring, unsigned long arg);


extern int __actvcapcall_keycall_keymgr_krctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int operation, void *keyring, unsigned long arg);


static inline int __attribute__((always_inline)) actvxactcall_keycall_keymgr_krctl(bool is_xact_fwd, bool is_xact_allow_refwd, int operation, void *keyring, unsigned long arg)
{
	const unsigned long __callno = __keymgr_method_keymgr_krctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_keycall_keymgr_krctl(rpc_info, NULL, operation, keyring, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_keycall_keymgr_krctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int operation, void *keyring, unsigned long arg)
{
	const unsigned long __callno = __keymgr_method_keymgr_krctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_keycall_keymgr_krctl(__rref, rpc_info, NULL, operation, keyring, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_keycall_keymgr_krctl(int operation, void *keyring, unsigned long arg)
{
	return actvxactcall_keycall_keymgr_krctl(false, false, operation, keyring, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_keycall_keymgr_krctl(rref_t __rref, int operation, void *keyring, unsigned long arg)
{
	return actvxactcapcall_keycall_keymgr_krctl(false, false, __rref, operation, keyring, arg);
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
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);


extern int actvhdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);


extern int actvhdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);


extern int actvhdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);


extern int actvhdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);


extern int actvhdlr_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);


extern int actvhdlr_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);


extern int actvhdlr_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);


#else /* aarch64 */
extern int actvhdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);


extern int actvhdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);


extern int actvhdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);


extern int actvhdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);


extern int actvhdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);


extern int __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);


extern int actvhdlr_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);


extern int actvhdlr_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);


extern int __actvshadowhdlr_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_keycall_keymgr_fscrypt_ctrl* actvhdlr_keyhandler_keymgr_fscrypt_ctrl_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_fscrypt_ctrl));
	return (struct __actvret_keycall_keymgr_fscrypt_ctrl*)__ret;
}

static inline struct __actvret_keycall_keymgr_fsverity_ctrl* actvhdlr_keyhandler_keymgr_fsverity_ctrl_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_fsverity_ctrl));
	return (struct __actvret_keycall_keymgr_fsverity_ctrl*)__ret;
}

static inline struct __actvret_keycall_keymgr_procfs_read* actvhdlr_keyhandler_keymgr_procfs_read_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_procfs_read));
	return (struct __actvret_keycall_keymgr_procfs_read*)__ret;
}

static inline struct __actvret_keycall_keymgr_procfs_write* actvhdlr_keyhandler_keymgr_procfs_write_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_procfs_write));
	return (struct __actvret_keycall_keymgr_procfs_write*)__ret;
}

static inline struct __actvret_keycall_keymgr_krctl* actvhdlr_keyhandler_keymgr_krctl_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_krctl));
	return (struct __actvret_keycall_keymgr_krctl*)__ret;
}

static inline struct __actvret_keycall_keymgr_add_key* actvhdlr_keyhandler_keymgr_add_key_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_add_key));
	return (struct __actvret_keycall_keymgr_add_key*)__ret;
}

static inline struct __actvret_keycall_keymgr_request_key* actvhdlr_keyhandler_keymgr_request_key_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_request_key));
	return (struct __actvret_keycall_keymgr_request_key*)__ret;
}

static inline struct __actvret_keycall_keymgr_keyctl* actvhdlr_keyhandler_keymgr_keyctl_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_keycall_keymgr_keyctl));
	return (struct __actvret_keycall_keymgr_keyctl*)__ret;
}

#ifdef __sysif_server_keymgr_no_compat32_handlers__
#ifdef __HOST_LLT__
union keymgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgehdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union keymgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgehdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union keymgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgehdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union keymgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgehdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union keymgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len);
	int (*p__actvbridgehdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg);
	int (*p__actvbridgehdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring);
	int (*p__actvbridgehdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_add_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_request_key)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_keyctl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_keymgr_no_compat32_handlers__) && defined(__sysif_server_keymgr_no_ilp32_handlers__)
#define __keymgr_method_TIMES 1
#elif !defined(__sysif_server_keymgr_no_compat32_handlers__) && !defined(__sysif_server_keymgr_no_ilp32_handlers__)
#define __keymgr_method_TIMES 3
#else
#define __keymgr_method_TIMES 2
#endif
extern union keymgr_hdlrs_union sysif_keymgr_actvhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES];

extern union keymgr_hdlrs_union sysif_keymgr_actvcapcallhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES];

extern void sysif_show_keymgr_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_procfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_procfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_procfs_write(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(const char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_procfs_write(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(const char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_krctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(void*, keyring), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_krctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(void*, keyring), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_add_key(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const char*, type), __ARCH_ACTV_RPC_RECV_ARG(const char*, description), __ARCH_ACTV_RPC_RECV_ARG(const void*, payload), __ARCH_ACTV_RPC_RECV_ARG(size_t, plen), __ARCH_ACTV_RPC_RECV_ARG(int, keyring));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_request_key(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const char*, type), __ARCH_ACTV_RPC_RECV_ARG(const char*, description), __ARCH_ACTV_RPC_RECV_ARG(const char*, callout_info), __ARCH_ACTV_RPC_RECV_ARG(int, keyring));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_keyctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg2), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg3), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg4), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg5));
}

#ifndef __sysif_server_keymgr_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	unsigned long __maybe_unused _arg_addr;
	unsigned long __maybe_unused _arg_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_arg_addr, data, 1, arg_sz, model, type, unsigned long, _arg_addr)
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
		__fetch_one_arg((void*)&_arg_size, data, 2, arg_sz, model, type, unsigned long, _arg_size)
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
	return __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, _operation, _arg_addr, _arg_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	unsigned long __maybe_unused _arg_addr;
	unsigned long __maybe_unused _arg_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_arg_addr, data, 1, arg_sz, model, type, unsigned long, _arg_addr)
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
		__fetch_one_arg((void*)&_arg_size, data, 2, arg_sz, model, type, unsigned long, _arg_size)
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
	return __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, _operation, _arg_addr, _arg_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	unsigned long __maybe_unused _arg_addr;
	unsigned long __maybe_unused _arg_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_arg_addr, data, 1, arg_sz, model, type, unsigned long, _arg_addr)
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
		__fetch_one_arg((void*)&_arg_size, data, 2, arg_sz, model, type, unsigned long, _arg_size)
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
	return __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, _operation, _arg_addr, _arg_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	unsigned long __maybe_unused _arg_addr;
	unsigned long __maybe_unused _arg_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_arg_addr, data, 1, arg_sz, model, type, unsigned long, _arg_addr)
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
		__fetch_one_arg((void*)&_arg_size, data, 2, arg_sz, model, type, unsigned long, _arg_size)
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
	return __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, _operation, _arg_addr, _arg_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _file_id;
	__maybe_unused char *_ubuf;
	unsigned long __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_file_id, data, 0, arg_sz, model, type, int, _file_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(char*);
		type = __type_of(char*);
		__fetch_one_arg((void*)&_ubuf, data, 1, arg_sz, model, type, char*, _ubuf)
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
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, unsigned long, _len)
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
	return __actvshadowhdlr_keyhandler_keymgr_procfs_read(sender, credential, _file_id, _ubuf, _len);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _file_id;
	__maybe_unused char *_ubuf;
	unsigned long __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_file_id, data, 0, arg_sz, model, type, int, _file_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(char*);
		type = __type_of(char*);
		__fetch_one_arg((void*)&_ubuf, data, 1, arg_sz, model, type, char*, _ubuf)
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
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, unsigned long, _len)
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
	return __actvshadowhdlr_keyhandler_keymgr_procfs_read(sender, credential, _file_id, _ubuf, _len);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _file_id;
	__maybe_unused const char *_ubuf;
	unsigned long __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_file_id, data, 0, arg_sz, model, type, int, _file_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_ubuf, data, 1, arg_sz, model, type, const char*, _ubuf)
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
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, unsigned long, _len)
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
	return __actvshadowhdlr_keyhandler_keymgr_procfs_write(sender, credential, _file_id, _ubuf, _len);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _file_id;
	__maybe_unused const char *_ubuf;
	unsigned long __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_file_id, data, 0, arg_sz, model, type, int, _file_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_ubuf, data, 1, arg_sz, model, type, const char*, _ubuf)
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
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, unsigned long, _len)
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
	return __actvshadowhdlr_keyhandler_keymgr_procfs_write(sender, credential, _file_id, _ubuf, _len);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	__maybe_unused void *_keyring;
	unsigned long __maybe_unused _arg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_keyring, data, 1, arg_sz, model, type, void*, _keyring)
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
		__fetch_one_arg((void*)&_arg, data, 2, arg_sz, model, type, unsigned long, _arg)
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
	return __actvshadowhdlr_keyhandler_keymgr_krctl(sender, credential, _operation, _keyring, _arg);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	__maybe_unused void *_keyring;
	unsigned long __maybe_unused _arg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_keyring, data, 1, arg_sz, model, type, void*, _keyring)
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
		__fetch_one_arg((void*)&_arg, data, 2, arg_sz, model, type, unsigned long, _arg)
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
	return __actvshadowhdlr_keyhandler_keymgr_krctl(sender, credential, _operation, _keyring, _arg);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused const char *_type;
	__maybe_unused const char *_description;
	__maybe_unused const void *_payload;
	size_t __maybe_unused _plen;
	int __maybe_unused _keyring;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, const char*, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_description, data, 1, arg_sz, model, type, const char*, _description)
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
		__fetch_one_arg((void*)&_payload, data, 2, arg_sz, model, type, const void*, _payload)
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
		__fetch_one_arg((void*)&_plen, data, 3, arg_sz, model, type, size_t, _plen)
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
		__fetch_one_arg((void*)&_keyring, data, 4, arg_sz, model, type, int, _keyring)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_add_key(sender, credential, _type, _description, _payload, _plen, _keyring);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused const char *_type;
	__maybe_unused const char *_description;
	__maybe_unused const char *_callout_info;
	int __maybe_unused _keyring;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, const char*, _type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_description, data, 1, arg_sz, model, type, const char*, _description)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const char*);
		type = __type_of(const char*);
		__fetch_one_arg((void*)&_callout_info, data, 2, arg_sz, model, type, const char*, _callout_info)
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
		__fetch_one_arg((void*)&_keyring, data, 3, arg_sz, model, type, int, _keyring)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_request_key(sender, credential, _type, _description, _callout_info, _keyring);
}

static int __opt_nonoframe __actvbridgehdlr_compat_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _operation;
	unsigned long __maybe_unused _arg2;
	unsigned long __maybe_unused _arg3;
	unsigned long __maybe_unused _arg4;
	unsigned long __maybe_unused _arg5;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_operation, data, 0, arg_sz, model, type, int, _operation)
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
		__fetch_one_arg((void*)&_arg2, data, 1, arg_sz, model, type, unsigned long, _arg2)
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
		__fetch_one_arg((void*)&_arg3, data, 2, arg_sz, model, type, unsigned long, _arg3)
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
		__fetch_one_arg((void*)&_arg4, data, 3, arg_sz, model, type, unsigned long, _arg4)
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
		__fetch_one_arg((void*)&_arg5, data, 4, arg_sz, model, type, unsigned long, _arg5)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_keyhandler_keymgr_keyctl(sender, credential, _operation, _arg2, _arg3, _arg4, _arg5);
}

#endif /* compat handlers */
#ifndef __sysif_server_keymgr_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_addr)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_size)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_addr)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_size)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_addr)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_size)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_addr)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg_size)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, file_id)
	} while (0);
	do {
		__adjust_one_arg(char*, ubuf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, len)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_procfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, file_id)
	} while (0);
	do {
		__adjust_one_arg(char*, ubuf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, len)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_procfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, file_id)
	} while (0);
	do {
		__adjust_one_arg(const char*, ubuf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, len)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_procfs_write(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(const char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, file_id)
	} while (0);
	do {
		__adjust_one_arg(const char*, ubuf)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, len)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_procfs_write(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(const char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(void*, keyring)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_krctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(void*, keyring), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(void*, keyring)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_krctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(void*, keyring), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(const char*, type)
	} while (0);
	do {
		__adjust_one_arg(const char*, description)
	} while (0);
	do {
		__adjust_one_arg(const void*, payload)
	} while (0);
	do {
		__adjust_one_arg(size_t, plen)
	} while (0);
	do {
		__adjust_one_arg(int, keyring)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_add_key(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const char*, type), __ARCH_ACTV_RPC_RECV_ARG(const char*, description), __ARCH_ACTV_RPC_RECV_ARG(const void*, payload), __ARCH_ACTV_RPC_RECV_ARG(size_t, plen), __ARCH_ACTV_RPC_RECV_ARG(int, keyring));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(const char*, type)
	} while (0);
	do {
		__adjust_one_arg(const char*, description)
	} while (0);
	do {
		__adjust_one_arg(const char*, callout_info)
	} while (0);
	do {
		__adjust_one_arg(int, keyring)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_request_key(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const char*, type), __ARCH_ACTV_RPC_RECV_ARG(const char*, description), __ARCH_ACTV_RPC_RECV_ARG(const char*, callout_info), __ARCH_ACTV_RPC_RECV_ARG(int, keyring));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, operation)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg2)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg3)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg4)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, arg5)
	} while (0);
	return __actvshadowhdlr_keyhandler_keymgr_keyctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg2), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg3), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg4), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg5));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_fscrypt_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_fsverity_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_addr), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg_size));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, int file_id, char *ubuf, unsigned long len)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_procfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, char *ubuf, unsigned long len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_procfs_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, int file_id, const char *ubuf, unsigned long len)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_procfs_write(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(const char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int file_id, const char *ubuf, unsigned long len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_procfs_write(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, file_id), __ARCH_ACTV_RPC_RECV_ARG(const char*, ubuf), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, len));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, int operation, void *keyring, unsigned long arg)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_krctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(void*, keyring), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_keyhandler_keymgr_krctl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int operation, void *keyring, unsigned long arg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_krctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(void*, keyring), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_add_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const void *payload, size_t plen, int keyring)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_add_key(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const char*, type), __ARCH_ACTV_RPC_RECV_ARG(const char*, description), __ARCH_ACTV_RPC_RECV_ARG(const void*, payload), __ARCH_ACTV_RPC_RECV_ARG(size_t, plen), __ARCH_ACTV_RPC_RECV_ARG(int, keyring));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_request_key(unsigned long long sender, unsigned long credential, const char *type, const char *description, const char *callout_info, int keyring)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_request_key(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const char*, type), __ARCH_ACTV_RPC_RECV_ARG(const char*, description), __ARCH_ACTV_RPC_RECV_ARG(const char*, callout_info), __ARCH_ACTV_RPC_RECV_ARG(int, keyring));
}

static int __opt_nonoframe __actvbridgehdlr_keyhandler_keymgr_keyctl(unsigned long long sender, unsigned long credential, int operation, unsigned long arg2, unsigned long arg3, unsigned long arg4, unsigned long arg5)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__u32 src_cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	__u32 fwd_cnode_idx = __RPC_SENDER_DECODE_FWD_CIDX(sender);
	if (unlikely(fwd_cnode_idx != CNODE_IDX_VALID_UNUSED)) {
		return E_HM_NOSYS;
	} else  {
		sender = ((__u64)src_cnode_idx << 32) + (__u64)src_cnode_idx;
	}
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_keyhandler_keymgr_keyctl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, operation), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg2), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg3), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg4), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, arg5));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_keymgr_no_compat32_handlers__) && defined(__sysif_server_keymgr_no_ilp32_handlers__)
__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_krctl = (&__actvbridgehdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	}
};

__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvcapcallhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_keymgr_no_compat32_handlers__) && defined(__sysif_server_keymgr_no_ilp32_handlers__)
__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_krctl = (&__actvbridgehdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	},
	[__keymgr_method_MAX ... __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_compat_keyhandler_keymgr_procfs_read)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_compat_keyhandler_keymgr_procfs_write)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_krctl = (&__actvbridgehdlr_compat_keyhandler_keymgr_krctl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_add_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_add_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_request_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_request_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_compat_keyhandler_keymgr_keyctl)
	}
};

__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvcapcallhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	},
	[__keymgr_method_MAX ... __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_add_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_add_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_request_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_request_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_compat_keyhandler_keymgr_keyctl)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_keymgr_no_compat32_handlers__) && !defined(__sysif_server_keymgr_no_ilp32_handlers__)
__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_krctl = (&__actvbridgehdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	},
	[__keymgr_method_MAX ... __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_fscrypt_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_fsverity_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_procfs_read)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_procfs_write)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_krctl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_krctl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_add_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_request_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_keyctl)
	}
};

__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvcapcallhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	},
	[__keymgr_method_MAX ... __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_fscrypt_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_fsverity_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_procfs_read)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_procfs_write)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_krctl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_add_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_request_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_keyctl)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_keymgr_no_compat32_handlers__) && !defined(__sysif_server_keymgr_no_ilp32_handlers__)
__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_krctl = (&__actvbridgehdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	},
	[__keymgr_method_MAX ... __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_compat_keyhandler_keymgr_fscrypt_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_compat_keyhandler_keymgr_fsverity_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_compat_keyhandler_keymgr_procfs_read)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_compat_keyhandler_keymgr_procfs_write)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_krctl = (&__actvbridgehdlr_compat_keyhandler_keymgr_krctl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_add_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_add_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_request_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_request_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_compat_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_compat_keyhandler_keymgr_keyctl)
	},
	[2 * __keymgr_method_MAX ... 2 * __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_fscrypt_ctrl)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_fsverity_ctrl)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_read = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_procfs_read)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgehdlr_keyhandler_keymgr_procfs_write = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_procfs_write)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_krctl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_krctl)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_add_key)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgehdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_request_key)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgehdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_keyctl)
	}
};

__ro_after_init_hdlr union keymgr_hdlrs_union sysif_keymgr_actvcapcallhdlr_table[__keymgr_method_MAX * __keymgr_method_TIMES] = {
	[0 ... __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl)
	},
	[ __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl)
	},
	[ __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read)
	},
	[ __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write)
	},
	[ __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_keyhandler_keymgr_krctl)
	},
	[ __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_keyhandler_keymgr_add_key)
	},
	[ __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_keyhandler_keymgr_request_key)
	},
	[ __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_keyhandler_keymgr_keyctl)
	},
	[__keymgr_method_MAX ... __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fscrypt_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_fsverity_ctrl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_read)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_procfs_write)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_compat_keyhandler_keymgr_krctl)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_add_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_add_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_request_key = (&__actvbridgehdlr_compat_keyhandler_keymgr_request_key)
	},
	[__keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_compat_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_compat_keyhandler_keymgr_keyctl)
	},
	[2 * __keymgr_method_MAX ... 2 * __keymgr_method_MAX + __keymgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_fscrypt_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fscrypt_ctrl = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_fscrypt_ctrl)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_fsverity_ctrl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_fsverity_ctrl = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_fsverity_ctrl)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_procfs_read] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_read = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_procfs_read)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_procfs_write] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_procfs_write = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_procfs_write)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_krctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_krctl = (&__actvbridgecapcallhdlr_ilp32_keyhandler_keymgr_krctl)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_add_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_add_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_add_key)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_request_key] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_request_key = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_request_key)
	},
	[2 * __keymgr_method_MAX + __keymgr_method_keymgr_keyctl] = {
		.p__actvbridgecapcallhdlr_keyhandler_keymgr_keyctl = (&__actvbridgehdlr_ilp32_keyhandler_keymgr_keyctl)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_keymgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "fscrypt_ctrl", __keymgr_method_keymgr_fscrypt_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "fscrypt_ctrl", __keymgr_method_keymgr_fscrypt_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "fsverity_ctrl", __keymgr_method_keymgr_fsverity_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "fsverity_ctrl", __keymgr_method_keymgr_fsverity_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "procfs_read", __keymgr_method_keymgr_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "procfs_read", __keymgr_method_keymgr_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "procfs_write", __keymgr_method_keymgr_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "procfs_write", __keymgr_method_keymgr_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "krctl", __keymgr_method_keymgr_krctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "krctl", __keymgr_method_keymgr_krctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "add_key", __keymgr_method_keymgr_add_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "add_key", __keymgr_method_keymgr_add_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "request_key", __keymgr_method_keymgr_request_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "request_key", __keymgr_method_keymgr_request_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "keyctl", __keymgr_method_keymgr_keyctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "keyctl", __keymgr_method_keymgr_keyctl);
	hm_info("%s_%s, max method num: %d\n", "keymgr", "method", __keymgr_method_MAX);
}

#elif defined(__arm__)
void sysif_show_keymgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "fscrypt_ctrl", __keymgr_method_keymgr_fscrypt_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "fscrypt_ctrl", __keymgr_method_keymgr_fscrypt_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "fsverity_ctrl", __keymgr_method_keymgr_fsverity_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "fsverity_ctrl", __keymgr_method_keymgr_fsverity_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "procfs_read", __keymgr_method_keymgr_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "procfs_read", __keymgr_method_keymgr_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "procfs_write", __keymgr_method_keymgr_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "procfs_write", __keymgr_method_keymgr_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "krctl", __keymgr_method_keymgr_krctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "krctl", __keymgr_method_keymgr_krctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "add_key", __keymgr_method_keymgr_add_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "add_key", __keymgr_method_keymgr_add_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "request_key", __keymgr_method_keymgr_request_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "request_key", __keymgr_method_keymgr_request_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "keyctl", __keymgr_method_keymgr_keyctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "keyctl", __keymgr_method_keymgr_keyctl);
	hm_info("%s_%s, max method num: %d\n", "keymgr", "method", __keymgr_method_MAX);
}

#else
void sysif_show_keymgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "fscrypt_ctrl", __keymgr_method_keymgr_fscrypt_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "fscrypt_ctrl", __keymgr_method_keymgr_fscrypt_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "fsverity_ctrl", __keymgr_method_keymgr_fsverity_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "fsverity_ctrl", __keymgr_method_keymgr_fsverity_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "procfs_read", __keymgr_method_keymgr_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "procfs_read", __keymgr_method_keymgr_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "procfs_write", __keymgr_method_keymgr_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "procfs_write", __keymgr_method_keymgr_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "krctl", __keymgr_method_keymgr_krctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "krctl", __keymgr_method_keymgr_krctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "add_key", __keymgr_method_keymgr_add_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "add_key", __keymgr_method_keymgr_add_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "request_key", __keymgr_method_keymgr_request_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "request_key", __keymgr_method_keymgr_request_key);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "keycall", "keymgr", "keyctl", __keymgr_method_keymgr_keyctl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "keyhandler", "keymgr", "keyctl", __keymgr_method_keymgr_keyctl);
	hm_info("%s_%s, max method num: %d\n", "keymgr", "method", __keymgr_method_MAX);
}

#endif
int __actvcall_keycall_keymgr_fscrypt_ctrl(unsigned long long __rpc_info, __maybe_unused void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_fscrypt_ctrl((unsigned long long)(uintptr_t)&selfcall, credential, operation, arg_addr, arg_size);
}

int __actvcapcall_keycall_keymgr_fscrypt_ctrl(rref_t __rref, unsigned long long __rpc_info, __maybe_unused void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_fscrypt_ctrl((unsigned long long)(uintptr_t)&selfcall, credential, operation, arg_addr, arg_size);
}

int __actvcall_keycall_keymgr_fsverity_ctrl(unsigned long long __rpc_info, __maybe_unused void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_fsverity_ctrl((unsigned long long)(uintptr_t)&selfcall, credential, operation, arg_addr, arg_size);
}

int __actvcapcall_keycall_keymgr_fsverity_ctrl(rref_t __rref, unsigned long long __rpc_info, __maybe_unused void *__buf, int operation, unsigned long arg_addr, unsigned long arg_size)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_fsverity_ctrl((unsigned long long)(uintptr_t)&selfcall, credential, operation, arg_addr, arg_size);
}

int __actvcall_keycall_keymgr_procfs_read(unsigned long long __rpc_info, __maybe_unused void *__buf, int file_id, char *ubuf, unsigned long len)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_procfs_read((unsigned long long)(uintptr_t)&selfcall, credential, file_id, ubuf, len);
}

int __actvcapcall_keycall_keymgr_procfs_read(rref_t __rref, unsigned long long __rpc_info, __maybe_unused void *__buf, int file_id, char *ubuf, unsigned long len)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_procfs_read((unsigned long long)(uintptr_t)&selfcall, credential, file_id, ubuf, len);
}

int __actvcall_keycall_keymgr_procfs_write(unsigned long long __rpc_info, __maybe_unused void *__buf, int file_id, const char *ubuf, unsigned long len)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_procfs_write((unsigned long long)(uintptr_t)&selfcall, credential, file_id, ubuf, len);
}

int __actvcapcall_keycall_keymgr_procfs_write(rref_t __rref, unsigned long long __rpc_info, __maybe_unused void *__buf, int file_id, const char *ubuf, unsigned long len)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_procfs_write((unsigned long long)(uintptr_t)&selfcall, credential, file_id, ubuf, len);
}

int __actvcall_keycall_keymgr_krctl(unsigned long long __rpc_info, __maybe_unused void *__buf, int operation, void *keyring, unsigned long arg)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_krctl((unsigned long long)(uintptr_t)&selfcall, credential, operation, keyring, arg);
}

int __actvcapcall_keycall_keymgr_krctl(rref_t __rref, unsigned long long __rpc_info, __maybe_unused void *__buf, int operation, void *keyring, unsigned long arg)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_keyhandler_keymgr_krctl((unsigned long long)(uintptr_t)&selfcall, credential, operation, keyring, arg);
}

#endif
#endif
