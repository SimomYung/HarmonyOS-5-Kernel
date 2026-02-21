/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for crypto
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libhmsrv_crypt/sysif/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/crypt/libsysif/crypt/api.h --server-dest ../image/usr/include/generated/sysif_server/crypt/libsysif/crypt/server.h --dump-callno ../image/usr/include/generated/dump_info/crypt/libsysif/crypt/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:27:09 2025
*/
#ifndef LIBSYSIF_CRYPTO_SERVER_H
#define LIBSYSIF_CRYPTO_SERVER_H
#ifndef LIBSYSIF_CRYPTO_COMMON_H
#define LIBSYSIF_CRYPTO_COMMON_H
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


#include <stdint.h>

enum crypt_rng_type {
	CRYPT_RNG_DEFAULT = 0,
	CRYPT_RNG_SYSTEM,
	CRYPT_RNG_HW,

	CRYPT_RNG_MAX
};

#define CRYPT_RNG_RAND_DATA_MAX	256
struct crypt_rng_rand_data {
	unsigned int len;
	unsigned char buf[CRYPT_RNG_RAND_DATA_MAX];
};

enum crypt_rng_rand_val_type {
	CRYPT_RNG_RAND_VAL_U8 = 0,
	CRYPT_RNG_RAND_VAL_U16,
	CRYPT_RNG_RAND_VAL_U32,
	CRYPT_RNG_RAND_VAL_U64,

	CRYPT_RNG_RAND_VAL_MAX
};

union crypt_rng_rand_value {
	uint8_t data_u8;
	uint16_t data_u16;
	uint32_t data_u32;
	uint64_t data_u64;
};


raw_static_assert(sizeof(enum crypt_rng_type) <= 96,
                    cryptcall_rng_random_bytes_arg_rng_type_is_too_large);
raw_static_assert(sizeof(enum crypt_rng_type) <= 96,
                    cryptcall_rng_random_value_arg_rng_type_is_too_large);
raw_static_assert(sizeof(enum crypt_rng_rand_val_type) <= 96,
                    cryptcall_rng_random_value_arg_value_type_is_too_large);
raw_static_assert(sizeof(uref_t) <= 96,
                    cryptcall_crypto_register_arg_uref_is_too_large);
struct __actvret_cryptcall_rng_random_bytes {
};

struct __actvret_cryptcall_rng_random_value {
};

struct __actvret_cryptcall_rng_random_entropy_avail {
};

struct __actvret_cryptcall_rng_getrandom {
};

struct __actvret_cryptcall_procfs_read {
};

struct __actvret_cryptcall_procfs_write {
};

struct __actvret_cryptcall_crypto_register {
};

struct __actvret_cryptcall_pac_cfi {
};

#ifdef __aarch64__
union __actvret_cryptcall {
	struct __actvret_cryptcall_rng_random_bytes rng_random_bytes;
	struct __actvret_cryptcall_rng_random_value rng_random_value;
	struct __actvret_cryptcall_rng_random_entropy_avail rng_random_entropy_avail;
	struct __actvret_cryptcall_rng_getrandom rng_getrandom;
	struct __actvret_cryptcall_procfs_read procfs_read;
	struct __actvret_cryptcall_procfs_write procfs_write;
	struct __actvret_cryptcall_crypto_register crypto_register;
	struct __actvret_cryptcall_pac_cfi pac_cfi;
};

#elif defined(__arm__)
union __actvret_cryptcall {
	struct __actvret_cryptcall_rng_random_bytes rng_random_bytes;
	struct __actvret_cryptcall_rng_random_value rng_random_value;
	struct __actvret_cryptcall_rng_random_entropy_avail rng_random_entropy_avail;
	struct __actvret_cryptcall_rng_getrandom rng_getrandom;
	struct __actvret_cryptcall_procfs_read procfs_read;
	struct __actvret_cryptcall_procfs_write procfs_write;
	struct __actvret_cryptcall_crypto_register crypto_register;
	struct __actvret_cryptcall_pac_cfi pac_cfi;
};

#else
union __actvret_cryptcall {
	struct __actvret_cryptcall_rng_random_bytes rng_random_bytes;
	struct __actvret_cryptcall_rng_random_value rng_random_value;
	struct __actvret_cryptcall_rng_random_entropy_avail rng_random_entropy_avail;
	struct __actvret_cryptcall_rng_getrandom rng_getrandom;
	struct __actvret_cryptcall_procfs_read procfs_read;
	struct __actvret_cryptcall_procfs_write procfs_write;
	struct __actvret_cryptcall_crypto_register crypto_register;
	struct __actvret_cryptcall_pac_cfi pac_cfi;
};

#endif
enum __crypto_mgr {
	__crypto_INVAL_mgr=0,
	__crypto_rng_mgr,
	__crypto_procfs_mgr,
	__crypto_crypto_mgr,
	__crypto_MAX_mgr,
};

#define __crypto_method_MIN __ACTV_HIGHTABLE_START
#define __crypto_method_rng_getrandom (__ACTV_HIGHTABLE_START + 1)
#define __crypto_method_rng_random_bytes (__ACTV_HIGHTABLE_START + 2)
#define __crypto_method_rng_random_value (__ACTV_HIGHTABLE_START + 3)
#define __crypto_method_rng_random_entropy_avail (__ACTV_HIGHTABLE_START + 4)
#define __crypto_method_procfs_read (__ACTV_HIGHTABLE_START + 5)
#define __crypto_method_procfs_write (__ACTV_HIGHTABLE_START + 6)
#define __crypto_method_procfs_rsvd_2 (__ACTV_HIGHTABLE_START + 7)
#define __crypto_method_procfs_rsvd_1 (__ACTV_HIGHTABLE_START + 8)
#define __crypto_method_crypto_register (__ACTV_HIGHTABLE_START + 9)
#define __crypto_method_pac_cfi (__ACTV_HIGHTABLE_START + 10)
#define __crypto_method_MAX (__ACTV_HIGHTABLE_START + 11)
raw_static_assert(sizeof(struct __actvret_cryptcall_rng_random_bytes) < 512,
            cryptcall_rng_random_bytes_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_rng_random_value) < 512,
            cryptcall_rng_random_value_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_rng_random_entropy_avail) < 512,
            cryptcall_rng_random_entropy_avail_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_rng_getrandom) < 512,
            cryptcall_rng_getrandom_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_procfs_read) < 512,
            cryptcall_procfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_procfs_write) < 512,
            cryptcall_procfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_crypto_register) < 512,
            cryptcall_crypto_register_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_cryptcall_pac_cfi) < 512,
            cryptcall_pac_cfi_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_cryptcall) < 512,
        cryptcall_too_many_actvret_data);
extern int __actvcall_cryptcall_rng_random_bytes(unsigned long long __rpc_info, void *__buf, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);


extern int __actvcapcall_cryptcall_rng_random_bytes(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_rng_random_bytes(bool is_xact_fwd, bool is_xact_allow_refwd, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	const unsigned long __callno = __crypto_method_rng_random_bytes;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_rng_random_bytes(rpc_info, NULL, rng_type, udata, bytes_len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_rng_random_bytes(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	const unsigned long __callno = __crypto_method_rng_random_bytes;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_rng_random_bytes(__rref, rpc_info, NULL, rng_type, udata, bytes_len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_rng_random_bytes(enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	return actvxactcall_cryptcall_rng_random_bytes(false, false, rng_type, udata, bytes_len, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_rng_random_bytes(rref_t __rref, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	return actvxactcapcall_cryptcall_rng_random_bytes(false, false, __rref, rng_type, udata, bytes_len, flags);
}

extern int __actvcall_cryptcall_rng_random_value(unsigned long long __rpc_info, void *__buf, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);


extern int __actvcapcall_cryptcall_rng_random_value(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_rng_random_value(bool is_xact_fwd, bool is_xact_allow_refwd, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	const unsigned long __callno = __crypto_method_rng_random_value;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_rng_random_value(rpc_info, NULL, rng_type, value_type, uvalue, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_rng_random_value(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	const unsigned long __callno = __crypto_method_rng_random_value;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_rng_random_value(__rref, rpc_info, NULL, rng_type, value_type, uvalue, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_rng_random_value(enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	return actvxactcall_cryptcall_rng_random_value(false, false, rng_type, value_type, uvalue, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_rng_random_value(rref_t __rref, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	return actvxactcapcall_cryptcall_rng_random_value(false, false, __rref, rng_type, value_type, uvalue, flags);
}

extern int __actvcall_cryptcall_rng_random_entropy_avail(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_cryptcall_rng_random_entropy_avail(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_rng_random_entropy_avail(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __crypto_method_rng_random_entropy_avail;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_rng_random_entropy_avail(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_rng_random_entropy_avail(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __crypto_method_rng_random_entropy_avail;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_rng_random_entropy_avail(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_rng_random_entropy_avail(void)
{
	return actvxactcall_cryptcall_rng_random_entropy_avail(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_rng_random_entropy_avail(rref_t __rref)
{
	return actvxactcapcall_cryptcall_rng_random_entropy_avail(false, false, __rref);
}

extern int __actvcall_cryptcall_procfs_read(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_cryptcall_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __crypto_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_procfs_read(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __crypto_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_procfs_read(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_procfs_read(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_cryptcall_procfs_read(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_procfs_read(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_cryptcall_procfs_read(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_cryptcall_procfs_write(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_cryptcall_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __crypto_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_procfs_write(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __crypto_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_procfs_write(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_procfs_write(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_cryptcall_procfs_write(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_procfs_write(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_cryptcall_procfs_write(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_cryptcall_crypto_register(unsigned long long __rpc_info, void *__buf, uref_t uref);


extern int __actvcapcall_cryptcall_crypto_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, uref_t uref);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_crypto_register(bool is_xact_fwd, bool is_xact_allow_refwd, uref_t uref)
{
	const unsigned long __callno = __crypto_method_crypto_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_crypto_register(rpc_info, NULL, uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_crypto_register(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uref_t uref)
{
	const unsigned long __callno = __crypto_method_crypto_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_crypto_register(__rref, rpc_info, NULL, uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_crypto_register(uref_t uref)
{
	return actvxactcall_cryptcall_crypto_register(false, false, uref);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_crypto_register(rref_t __rref, uref_t uref)
{
	return actvxactcapcall_cryptcall_crypto_register(false, false, __rref, uref);
}

extern int __actvcall_cryptcall_pac_cfi(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_cryptcall_pac_cfi(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_cryptcall_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __crypto_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_cryptcall_pac_cfi(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_cryptcall_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __crypto_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_cryptcall_pac_cfi(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_cryptcall_pac_cfi(void)
{
	return actvxactcall_cryptcall_pac_cfi(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_cryptcall_pac_cfi(rref_t __rref)
{
	return actvxactcapcall_cryptcall_pac_cfi(false, false, __rref);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);


extern int actvhdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);


extern int actvhdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential);


extern int actvhdlr_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);


extern int actvhdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential);


extern int actvhdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential);


extern int actvhdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, uref_t uref);


extern int actvhdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential);


#else /* aarch64 */
extern int actvhdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);


extern int actvhdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);


extern int actvhdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);


extern int __actvshadowhdlr_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential);


extern int actvhdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, uref_t uref);


extern int actvhdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_cryptcall_rng_random_bytes* actvhdlr_crypthandler_rng_random_bytes_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_rng_random_bytes));
	return (struct __actvret_cryptcall_rng_random_bytes*)__ret;
}

static inline struct __actvret_cryptcall_rng_random_value* actvhdlr_crypthandler_rng_random_value_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_rng_random_value));
	return (struct __actvret_cryptcall_rng_random_value*)__ret;
}

static inline struct __actvret_cryptcall_rng_random_entropy_avail* actvhdlr_crypthandler_rng_random_entropy_avail_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_rng_random_entropy_avail));
	return (struct __actvret_cryptcall_rng_random_entropy_avail*)__ret;
}

static inline struct __actvret_cryptcall_rng_getrandom* actvhdlr_crypthandler_rng_getrandom_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_rng_getrandom));
	return (struct __actvret_cryptcall_rng_getrandom*)__ret;
}

static inline struct __actvret_cryptcall_procfs_read* actvhdlr_crypthandler_procfs_read_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_procfs_read));
	return (struct __actvret_cryptcall_procfs_read*)__ret;
}

static inline struct __actvret_cryptcall_procfs_write* actvhdlr_crypthandler_procfs_write_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_procfs_write));
	return (struct __actvret_cryptcall_procfs_write*)__ret;
}

static inline struct __actvret_cryptcall_crypto_register* actvhdlr_crypthandler_crypto_register_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_crypto_register));
	return (struct __actvret_cryptcall_crypto_register*)__ret;
}

static inline struct __actvret_cryptcall_pac_cfi* actvhdlr_crypthandler_pac_cfi_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_cryptcall_pac_cfi));
	return (struct __actvret_cryptcall_pac_cfi*)__ret;
}

#ifdef __sysif_server_crypto_no_compat32_handlers__
#ifdef __HOST_LLT__
union crypto_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgehdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, uref_t uref);
	int (*p__actvbridgecapcallhdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref);
	int (*p__actvbridgehdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union crypto_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgehdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, uref_t uref);
	int (*p__actvbridgecapcallhdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref);
	int (*p__actvbridgehdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union crypto_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgehdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, uref_t uref);
	int (*p__actvbridgecapcallhdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref);
	int (*p__actvbridgehdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union crypto_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgehdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, uref_t uref);
	int (*p__actvbridgecapcallhdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref);
	int (*p__actvbridgehdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union crypto_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags);
	int (*p__actvbridgehdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgecapcallhdlr_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags);
	int (*p__actvbridgehdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, uref_t uref);
	int (*p__actvbridgecapcallhdlr_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref);
	int (*p__actvbridgehdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_value)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_rng_getrandom)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_procfs_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_procfs_write)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_crypto_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_crypthandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_crypto_no_compat32_handlers__) && defined(__sysif_server_crypto_no_ilp32_handlers__)
#define __crypto_method_TIMES 1
#elif !defined(__sysif_server_crypto_no_compat32_handlers__) && !defined(__sysif_server_crypto_no_ilp32_handlers__)
#define __crypto_method_TIMES 3
#else
#define __crypto_method_TIMES 2
#endif
extern union crypto_hdlrs_union sysif_crypto_actvhdlr_table[__crypto_method_MAX * __crypto_method_TIMES];

extern union crypto_hdlrs_union sysif_crypto_actvcapcallhdlr_table[__crypto_method_MAX * __crypto_method_TIMES];

extern void sysif_show_crypto_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_rng_random_bytes(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(void*, udata), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, bytes_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_rng_random_bytes(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(void*, udata), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, bytes_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_rng_random_value(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_rand_val_type, value_type), __ARCH_ACTV_RPC_RECV_ARG(void*, uvalue), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_rng_random_value(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_rand_val_type, value_type), __ARCH_ACTV_RPC_RECV_ARG(void*, uvalue), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags)
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
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_rng_getrandom(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buflen), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, uref_t uref)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_crypto_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uref_t, uref));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_crypthandler_crypto_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uref_t, uref));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_pac_cfi(sender, credential);
}

#ifndef __sysif_server_crypto_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	enum crypt_rng_type __maybe_unused _rng_type;
	__maybe_unused void *_udata;
	unsigned int __maybe_unused _bytes_len;
	int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum crypt_rng_type);
		type = __type_of(enum crypt_rng_type);
		__fetch_one_arg((void*)&_rng_type, data, 0, arg_sz, model, type, enum crypt_rng_type, _rng_type)
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
		__fetch_one_arg((void*)&_udata, data, 1, arg_sz, model, type, void*, _udata)
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
		__fetch_one_arg((void*)&_bytes_len, data, 2, arg_sz, model, type, unsigned int, _bytes_len)
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
		__fetch_one_arg((void*)&_flags, data, 3, arg_sz, model, type, int, _flags)
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
	return __actvshadowhdlr_crypthandler_rng_random_bytes(sender, credential, _rng_type, _udata, _bytes_len, _flags);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	enum crypt_rng_type __maybe_unused _rng_type;
	__maybe_unused void *_udata;
	unsigned int __maybe_unused _bytes_len;
	int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum crypt_rng_type);
		type = __type_of(enum crypt_rng_type);
		__fetch_one_arg((void*)&_rng_type, data, 0, arg_sz, model, type, enum crypt_rng_type, _rng_type)
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
		__fetch_one_arg((void*)&_udata, data, 1, arg_sz, model, type, void*, _udata)
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
		__fetch_one_arg((void*)&_bytes_len, data, 2, arg_sz, model, type, unsigned int, _bytes_len)
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
		__fetch_one_arg((void*)&_flags, data, 3, arg_sz, model, type, int, _flags)
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
	return __actvshadowhdlr_crypthandler_rng_random_bytes(sender, credential, _rng_type, _udata, _bytes_len, _flags);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	enum crypt_rng_type __maybe_unused _rng_type;
	enum crypt_rng_rand_val_type __maybe_unused _value_type;
	__maybe_unused void *_uvalue;
	int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum crypt_rng_type);
		type = __type_of(enum crypt_rng_type);
		__fetch_one_arg((void*)&_rng_type, data, 0, arg_sz, model, type, enum crypt_rng_type, _rng_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum crypt_rng_rand_val_type);
		type = __type_of(enum crypt_rng_rand_val_type);
		__fetch_one_arg((void*)&_value_type, data, 1, arg_sz, model, type, enum crypt_rng_rand_val_type, _value_type)
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
		__fetch_one_arg((void*)&_uvalue, data, 2, arg_sz, model, type, void*, _uvalue)
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
		__fetch_one_arg((void*)&_flags, data, 3, arg_sz, model, type, int, _flags)
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
	return __actvshadowhdlr_crypthandler_rng_random_value(sender, credential, _rng_type, _value_type, _uvalue, _flags);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	enum crypt_rng_type __maybe_unused _rng_type;
	enum crypt_rng_rand_val_type __maybe_unused _value_type;
	__maybe_unused void *_uvalue;
	int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum crypt_rng_type);
		type = __type_of(enum crypt_rng_type);
		__fetch_one_arg((void*)&_rng_type, data, 0, arg_sz, model, type, enum crypt_rng_type, _rng_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum crypt_rng_rand_val_type);
		type = __type_of(enum crypt_rng_rand_val_type);
		__fetch_one_arg((void*)&_value_type, data, 1, arg_sz, model, type, enum crypt_rng_rand_val_type, _value_type)
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
		__fetch_one_arg((void*)&_uvalue, data, 2, arg_sz, model, type, void*, _uvalue)
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
		__fetch_one_arg((void*)&_flags, data, 3, arg_sz, model, type, int, _flags)
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
	return __actvshadowhdlr_crypthandler_rng_random_value(sender, credential, _rng_type, _value_type, _uvalue, _flags);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused void *_buf;
	size_t __maybe_unused _buflen;
	unsigned int __maybe_unused _flags;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(void*);
		type = __type_of(void*);
		__fetch_one_arg((void*)&_buf, data, 0, arg_sz, model, type, void*, _buf)
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
		__fetch_one_arg((void*)&_buflen, data, 1, arg_sz, model, type, size_t, _buflen)
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
	return __actvshadowhdlr_crypthandler_rng_getrandom(sender, credential, _buf, _buflen, _flags);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_procfs_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_procfs_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_procfs_write(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_procfs_write(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uref_t __maybe_unused _uref;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uref_t);
		type = __type_of(uref_t);
		__fetch_one_arg((void*)&_uref, data, 0, arg_sz, model, type, uref_t, _uref)
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
	return __actvshadowhdlr_crypthandler_crypto_register(sender, credential, _uref);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uref_t __maybe_unused _uref;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uref_t);
		type = __type_of(uref_t);
		__fetch_one_arg((void*)&_uref, data, 0, arg_sz, model, type, uref_t, _uref)
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
	return __actvshadowhdlr_crypthandler_crypto_register(sender, credential, _uref);
}

static int __opt_nonoframe __actvbridgehdlr_compat_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_pac_cfi(sender, credential);
}

#endif /* compat handlers */
#ifndef __sysif_server_crypto_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum crypt_rng_type, rng_type)
	} while (0);
	do {
		__adjust_one_arg(void*, udata)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, bytes_len)
	} while (0);
	do {
		__adjust_one_arg(int, flags)
	} while (0);
	return __actvshadowhdlr_crypthandler_rng_random_bytes(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(void*, udata), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, bytes_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum crypt_rng_type, rng_type)
	} while (0);
	do {
		__adjust_one_arg(void*, udata)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, bytes_len)
	} while (0);
	do {
		__adjust_one_arg(int, flags)
	} while (0);
	return __actvshadowhdlr_crypthandler_rng_random_bytes(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(void*, udata), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, bytes_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum crypt_rng_type, rng_type)
	} while (0);
	do {
		__adjust_one_arg(enum crypt_rng_rand_val_type, value_type)
	} while (0);
	do {
		__adjust_one_arg(void*, uvalue)
	} while (0);
	do {
		__adjust_one_arg(int, flags)
	} while (0);
	return __actvshadowhdlr_crypthandler_rng_random_value(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_rand_val_type, value_type), __ARCH_ACTV_RPC_RECV_ARG(void*, uvalue), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum crypt_rng_type, rng_type)
	} while (0);
	do {
		__adjust_one_arg(enum crypt_rng_rand_val_type, value_type)
	} while (0);
	do {
		__adjust_one_arg(void*, uvalue)
	} while (0);
	do {
		__adjust_one_arg(int, flags)
	} while (0);
	return __actvshadowhdlr_crypthandler_rng_random_value(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_rand_val_type, value_type), __ARCH_ACTV_RPC_RECV_ARG(void*, uvalue), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags)
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
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(void*, buf)
	} while (0);
	do {
		__adjust_one_arg(size_t, buflen)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, flags)
	} while (0);
	return __actvshadowhdlr_crypthandler_rng_getrandom(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buflen), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_procfs_read(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_procfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_procfs_write(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_procfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, uref_t uref)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uref_t, uref)
	} while (0);
	return __actvshadowhdlr_crypthandler_crypto_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uref_t, uref));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uref_t, uref)
	} while (0);
	return __actvshadowhdlr_crypthandler_crypto_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uref_t, uref));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_crypthandler_pac_cfi(sender, credential);
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_rng_random_bytes(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(void*, udata), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, bytes_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_rng_random_bytes(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, void *udata, unsigned int bytes_len, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_rng_random_bytes(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(void*, udata), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, bytes_len), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_rng_random_value(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_rand_val_type, value_type), __ARCH_ACTV_RPC_RECV_ARG(void*, uvalue), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_rng_random_value(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum crypt_rng_type rng_type, enum crypt_rng_rand_val_type value_type, void *uvalue, int flags)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum crypt_rng_rand_val_type), __arch_actv_rpc_stksz_alignment(enum crypt_rng_rand_val_type));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_rng_random_value(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_type, rng_type), __ARCH_ACTV_RPC_RECV_ARG(enum crypt_rng_rand_val_type, value_type), __ARCH_ACTV_RPC_RECV_ARG(void*, uvalue), __ARCH_ACTV_RPC_RECV_ARG(int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_rng_random_entropy_avail(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_rng_getrandom(unsigned long long sender, unsigned long credential, void *buf, size_t buflen, unsigned int flags)
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
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_rng_getrandom(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(void*, buf), __ARCH_ACTV_RPC_RECV_ARG(size_t, buflen), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_procfs_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_procfs_read(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_procfs_write(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_procfs_write(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, uref_t uref)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_crypto_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uref_t, uref));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_crypto_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uref_t uref)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_crypthandler_crypto_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uref_t, uref));
}

static int __opt_nonoframe __actvbridgehdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_crypthandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_crypthandler_pac_cfi(sender, credential);
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_crypto_no_compat32_handlers__) && defined(__sysif_server_crypto_no_ilp32_handlers__)
__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_crypthandler_rng_random_bytes = (&__actvbridgehdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_crypthandler_rng_random_value = (&__actvbridgehdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_crypthandler_procfs_read = (&__actvbridgehdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_crypthandler_procfs_write = (&__actvbridgehdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_crypthandler_crypto_register = (&__actvbridgehdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_crypthandler_pac_cfi = (&__actvbridgehdlr_crypthandler_pac_cfi)
	}
};

__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvcapcallhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_crypthandler_pac_cfi)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_crypto_no_compat32_handlers__) && defined(__sysif_server_crypto_no_ilp32_handlers__)
__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_crypthandler_rng_random_bytes = (&__actvbridgehdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_crypthandler_rng_random_value = (&__actvbridgehdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_crypthandler_procfs_read = (&__actvbridgehdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_crypthandler_procfs_write = (&__actvbridgehdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_crypthandler_crypto_register = (&__actvbridgehdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_crypthandler_pac_cfi = (&__actvbridgehdlr_crypthandler_pac_cfi)
	},
	[__crypto_method_MAX ... __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_random_bytes = (&__actvbridgehdlr_compat_crypthandler_rng_random_bytes)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_random_value = (&__actvbridgehdlr_compat_crypthandler_rng_random_value)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail)
	},
	[__crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_getrandom = (&__actvbridgehdlr_compat_crypthandler_rng_getrandom)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_compat_crypthandler_procfs_read = (&__actvbridgehdlr_compat_crypthandler_procfs_read)
	},
	[__crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_compat_crypthandler_procfs_write = (&__actvbridgehdlr_compat_crypthandler_procfs_write)
	},
	[__crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_compat_crypthandler_crypto_register = (&__actvbridgehdlr_compat_crypthandler_crypto_register)
	},
	[__crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_compat_crypthandler_pac_cfi = (&__actvbridgehdlr_compat_crypthandler_pac_cfi)
	}
};

__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvcapcallhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_crypthandler_pac_cfi)
	},
	[__crypto_method_MAX ... __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_compat_crypthandler_rng_random_value)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail)
	},
	[__crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_getrandom = (&__actvbridgehdlr_compat_crypthandler_rng_getrandom)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_compat_crypthandler_procfs_read)
	},
	[__crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_compat_crypthandler_procfs_write)
	},
	[__crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_compat_crypthandler_crypto_register)
	},
	[__crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_compat_crypthandler_pac_cfi)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_crypto_no_compat32_handlers__) && !defined(__sysif_server_crypto_no_ilp32_handlers__)
__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_crypthandler_rng_random_bytes = (&__actvbridgehdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_crypthandler_rng_random_value = (&__actvbridgehdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_crypthandler_procfs_read = (&__actvbridgehdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_crypthandler_procfs_write = (&__actvbridgehdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_crypthandler_crypto_register = (&__actvbridgehdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_crypthandler_pac_cfi = (&__actvbridgehdlr_crypthandler_pac_cfi)
	},
	[__crypto_method_MAX ... __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_crypthandler_rng_random_bytes = (&__actvbridgehdlr_ilp32_crypthandler_rng_random_bytes)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_crypthandler_rng_random_value = (&__actvbridgehdlr_ilp32_crypthandler_rng_random_value)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_ilp32_crypthandler_rng_random_entropy_avail)
	},
	[__crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_ilp32_crypthandler_rng_getrandom)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_crypthandler_procfs_read = (&__actvbridgehdlr_ilp32_crypthandler_procfs_read)
	},
	[__crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_crypthandler_procfs_write = (&__actvbridgehdlr_ilp32_crypthandler_procfs_write)
	},
	[__crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_crypthandler_crypto_register = (&__actvbridgehdlr_ilp32_crypthandler_crypto_register)
	},
	[__crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_crypthandler_pac_cfi = (&__actvbridgehdlr_ilp32_crypthandler_pac_cfi)
	}
};

__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvcapcallhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_crypthandler_pac_cfi)
	},
	[__crypto_method_MAX ... __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_bytes)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_value)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_entropy_avail)
	},
	[__crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_ilp32_crypthandler_rng_getrandom)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_ilp32_crypthandler_procfs_read)
	},
	[__crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_ilp32_crypthandler_procfs_write)
	},
	[__crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_ilp32_crypthandler_crypto_register)
	},
	[__crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_ilp32_crypthandler_pac_cfi)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_crypto_no_compat32_handlers__) && !defined(__sysif_server_crypto_no_ilp32_handlers__)
__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_crypthandler_rng_random_bytes = (&__actvbridgehdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_crypthandler_rng_random_value = (&__actvbridgehdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_crypthandler_procfs_read = (&__actvbridgehdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_crypthandler_procfs_write = (&__actvbridgehdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_crypthandler_crypto_register = (&__actvbridgehdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_crypthandler_pac_cfi = (&__actvbridgehdlr_crypthandler_pac_cfi)
	},
	[__crypto_method_MAX ... __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_random_bytes = (&__actvbridgehdlr_compat_crypthandler_rng_random_bytes)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_random_value = (&__actvbridgehdlr_compat_crypthandler_rng_random_value)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_compat_crypthandler_rng_random_entropy_avail)
	},
	[__crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_compat_crypthandler_rng_getrandom = (&__actvbridgehdlr_compat_crypthandler_rng_getrandom)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_compat_crypthandler_procfs_read = (&__actvbridgehdlr_compat_crypthandler_procfs_read)
	},
	[__crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_compat_crypthandler_procfs_write = (&__actvbridgehdlr_compat_crypthandler_procfs_write)
	},
	[__crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_compat_crypthandler_crypto_register = (&__actvbridgehdlr_compat_crypthandler_crypto_register)
	},
	[__crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_compat_crypthandler_pac_cfi = (&__actvbridgehdlr_compat_crypthandler_pac_cfi)
	},
	[2 * __crypto_method_MAX ... 2 * __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgehdlr_crypthandler_rng_random_bytes = (&__actvbridgehdlr_ilp32_crypthandler_rng_random_bytes)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgehdlr_crypthandler_rng_random_value = (&__actvbridgehdlr_ilp32_crypthandler_rng_random_value)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgehdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgehdlr_ilp32_crypthandler_rng_random_entropy_avail)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgehdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_ilp32_crypthandler_rng_getrandom)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgehdlr_crypthandler_procfs_read = (&__actvbridgehdlr_ilp32_crypthandler_procfs_read)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgehdlr_crypthandler_procfs_write = (&__actvbridgehdlr_ilp32_crypthandler_procfs_write)
	},
	[2 * __crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgehdlr_crypthandler_crypto_register = (&__actvbridgehdlr_ilp32_crypthandler_crypto_register)
	},
	[2 * __crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgehdlr_crypthandler_pac_cfi = (&__actvbridgehdlr_ilp32_crypthandler_pac_cfi)
	}
};

__ro_after_init_hdlr union crypto_hdlrs_union sysif_crypto_actvcapcallhdlr_table[__crypto_method_MAX * __crypto_method_TIMES] = {
	[0 ... __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_crypthandler_rng_random_bytes)
	},
	[ __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_crypthandler_rng_random_value)
	},
	[ __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail)
	},
	[ __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_crypthandler_rng_getrandom)
	},
	[ __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_crypthandler_procfs_read)
	},
	[ __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_crypthandler_procfs_write)
	},
	[ __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_crypthandler_crypto_register)
	},
	[ __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_crypthandler_pac_cfi)
	},
	[__crypto_method_MAX ... __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_compat_crypthandler_rng_random_bytes)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_compat_crypthandler_rng_random_value)
	},
	[__crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_compat_crypthandler_rng_random_entropy_avail)
	},
	[__crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_rng_getrandom = (&__actvbridgehdlr_compat_crypthandler_rng_getrandom)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_compat_crypthandler_procfs_read)
	},
	[__crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_compat_crypthandler_procfs_write)
	},
	[__crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_compat_crypthandler_crypto_register)
	},
	[__crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_compat_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_compat_crypthandler_pac_cfi)
	},
	[2 * __crypto_method_MAX ... 2 * __crypto_method_MAX + __crypto_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_random_bytes] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_bytes = (&__actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_bytes)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_random_value] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_value = (&__actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_value)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_random_entropy_avail] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_random_entropy_avail = (&__actvbridgecapcallhdlr_ilp32_crypthandler_rng_random_entropy_avail)
	},
	[2 * __crypto_method_MAX + __crypto_method_rng_getrandom] = {
		.p__actvbridgecapcallhdlr_crypthandler_rng_getrandom = (&__actvbridgehdlr_ilp32_crypthandler_rng_getrandom)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_read] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_read = (&__actvbridgecapcallhdlr_ilp32_crypthandler_procfs_read)
	},
	[2 * __crypto_method_MAX + __crypto_method_procfs_write] = {
		.p__actvbridgecapcallhdlr_crypthandler_procfs_write = (&__actvbridgecapcallhdlr_ilp32_crypthandler_procfs_write)
	},
	[2 * __crypto_method_MAX + __crypto_method_crypto_register] = {
		.p__actvbridgecapcallhdlr_crypthandler_crypto_register = (&__actvbridgecapcallhdlr_ilp32_crypthandler_crypto_register)
	},
	[2 * __crypto_method_MAX + __crypto_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_crypthandler_pac_cfi = (&__actvbridgecapcallhdlr_ilp32_crypthandler_pac_cfi)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_crypto_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_bytes", __crypto_method_rng_random_bytes);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_bytes", __crypto_method_rng_random_bytes);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_value", __crypto_method_rng_random_value);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_value", __crypto_method_rng_random_value);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_entropy_avail", __crypto_method_rng_random_entropy_avail);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_entropy_avail", __crypto_method_rng_random_entropy_avail);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "getrandom", __crypto_method_rng_getrandom);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "getrandom", __crypto_method_rng_getrandom);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "procfs", "read", __crypto_method_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "procfs", "read", __crypto_method_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "procfs", "write", __crypto_method_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "procfs", "write", __crypto_method_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "crypto", "register", __crypto_method_crypto_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "crypto", "register", __crypto_method_crypto_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "pac", "cfi", __crypto_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "pac", "cfi", __crypto_method_pac_cfi);
	hm_info("%s_%s, max method num: %d\n", "crypto", "method", __crypto_method_MAX);
}

#elif defined(__arm__)
void sysif_show_crypto_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_bytes", __crypto_method_rng_random_bytes);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_bytes", __crypto_method_rng_random_bytes);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_value", __crypto_method_rng_random_value);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_value", __crypto_method_rng_random_value);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_entropy_avail", __crypto_method_rng_random_entropy_avail);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_entropy_avail", __crypto_method_rng_random_entropy_avail);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "getrandom", __crypto_method_rng_getrandom);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "getrandom", __crypto_method_rng_getrandom);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "procfs", "read", __crypto_method_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "procfs", "read", __crypto_method_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "procfs", "write", __crypto_method_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "procfs", "write", __crypto_method_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "crypto", "register", __crypto_method_crypto_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "crypto", "register", __crypto_method_crypto_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "pac", "cfi", __crypto_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "pac", "cfi", __crypto_method_pac_cfi);
	hm_info("%s_%s, max method num: %d\n", "crypto", "method", __crypto_method_MAX);
}

#else
void sysif_show_crypto_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_bytes", __crypto_method_rng_random_bytes);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_bytes", __crypto_method_rng_random_bytes);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_value", __crypto_method_rng_random_value);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_value", __crypto_method_rng_random_value);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "random_entropy_avail", __crypto_method_rng_random_entropy_avail);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "random_entropy_avail", __crypto_method_rng_random_entropy_avail);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "rng", "getrandom", __crypto_method_rng_getrandom);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "rng", "getrandom", __crypto_method_rng_getrandom);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "procfs", "read", __crypto_method_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "procfs", "read", __crypto_method_procfs_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "procfs", "write", __crypto_method_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "procfs", "write", __crypto_method_procfs_write);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "crypto", "register", __crypto_method_crypto_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "crypto", "register", __crypto_method_crypto_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "cryptcall", "pac", "cfi", __crypto_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "crypthandler", "pac", "cfi", __crypto_method_pac_cfi);
	hm_info("%s_%s, max method num: %d\n", "crypto", "method", __crypto_method_MAX);
}

#endif
#endif
#endif
