/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Description: ABI definitions to support delayed delivery of signals
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 28 15:31:55 2020
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SIGNAL_ABI_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SIGNAL_ABI_H

#include <stddef.h>
#include <stdint.h>

#define HM_SIGNAL_ABI_NR_INTS 2UL
union hm_signal_abi_u {
	struct {
		int signal_disable_count;
		char signal_trigger_count;
		char signal_handled;
		char signal_restart;
		char _rsvd0;
	};
	int _data[HM_SIGNAL_ABI_NR_INTS];
};
_Static_assert(sizeof(union hm_signal_abi_u) == sizeof(int) * HM_SIGNAL_ABI_NR_INTS,
	       "Invalid size of hm_signal_abi_u");
_Static_assert(offsetof(union hm_signal_abi_u, signal_trigger_count) == sizeof(int),
	       "Invalid alignment of signal_trigger_count");

/*
 * These constants are offsets to the end of struct pthread.
 * Their values should be kept in sync with libc ABI.
 */
#define HM_SIGNAL_ABI_LIBC_USED_SIZE (2)
#define HM_SIGNAL_ABI_LIBC_POST_RESERVED_SLOT_SIZE (5)

#define HM_SIGNAL_ABI_BASE_OFFSET \
	(HM_SIGNAL_ABI_LIBC_USED_SIZE * sizeof(void *) + \
	 HM_SIGNAL_ABI_LIBC_POST_RESERVED_SLOT_SIZE * sizeof(void *) + \
	 sizeof(union hm_signal_abi_u))

#define HM_SIGNAL_ABI_BASE_OFFSET_COMPAT \
	(HM_SIGNAL_ABI_LIBC_USED_SIZE * sizeof(uint32_t) + \
	 HM_SIGNAL_ABI_LIBC_POST_RESERVED_SLOT_SIZE * sizeof(uint32_t) + \
	 sizeof(union hm_signal_abi_u))

/*
 * Accessing signal_{disable,trigger}_count directly via TLS register is
 * faster than marking them as __thread in a shared library.
 *
 * This can improve performance of all vsyscalls.
 */
static inline int *raw_signal_disable_count_addr_of(uintptr_t tls)
{
	char *tls_ptr = (char *)tls;
	tls_ptr -= HM_SIGNAL_ABI_BASE_OFFSET;
	tls_ptr += offsetof(union hm_signal_abi_u, signal_disable_count);
	return (int *)tls_ptr;
}

static inline char *raw_signal_trigger_count_addr_of(uintptr_t tls)
{
	char *tls_ptr = (char *)tls;
	tls_ptr -= HM_SIGNAL_ABI_BASE_OFFSET;
	tls_ptr += offsetof(union hm_signal_abi_u, signal_trigger_count);
	return tls_ptr;
}

static inline int *compat_signal_disable_count_addr_of(uintptr_t tls)
{
	char *tls_ptr = (char *)tls;
	tls_ptr -= HM_SIGNAL_ABI_BASE_OFFSET_COMPAT;
	tls_ptr += offsetof(union hm_signal_abi_u, signal_disable_count);
	return (int *)tls_ptr;
}

static inline char *compat_signal_trigger_count_addr_of(uintptr_t tls)
{
	char *tls_ptr = (char *)tls;
	tls_ptr -= HM_SIGNAL_ABI_BASE_OFFSET_COMPAT;
	tls_ptr += offsetof(union hm_signal_abi_u, signal_trigger_count);
	return tls_ptr;
}

#endif
