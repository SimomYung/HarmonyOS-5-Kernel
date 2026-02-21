/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Memory manager interfaces of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:35:13 2019
 */
#include <udk/mm.h>
#include <udk/iolib.h>

#include <limits.h>

#include <libmem/umem.h>
#include <libmem/utils.h>
#include <libalgo/rbtree.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hmkernel/mm/pgtable.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhwsecurec/securec.h>

#include <devhost/memory.h>

#include <udk/log.h>
#include <udk/cache.h>
#include <udk/mutex.h>
#include <udk/bitops.h>
#include <udk/rwlock.h>
#include <udk/mempool.h>
#include <internal/init.h>

#ifdef UDK_FUSION
#include <intapi/mem/paf.h>
#include <intapi/mem/sysmem.h>
raw_static_assert(UDK_PAF_NO_FAIL == PAF_NO_FAIL, sysmgr_PAF_NO_FAIL_NOTEQUAL_UDK_PAF_NO_FAIL);
raw_static_assert(UDK_PAF_NO_IO == PAF_NO_IO, sysmgr_PAF_NO_IO_NOTEQUAL_UDK_PAF_NO_IO);
#endif

int udk_copy_from_caller(void *dst, const void *src, unsigned long n)
{
	int ret;

	if (dst == NULL) {
		return -EINVAL;
	}
	mem_zero_b(dst, (size_t)n);

	ret = hm_copy_from_caller(dst, src, n);
	if (ret < 0) {
		udk_error("copy from caller failed, err=%s\n", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}

int udk_copy_to_caller(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = hm_copy_to_caller(dst, src, n);
	if (ret < 0) {
		udk_error("copy to caller failed, err=%s\n", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}

int udk_copy_from_user(void *dst, const void *src, unsigned long n)
{
	/*
	 * We have to touch the pages in dst to populate physical pages first,
	 * because kernel do not support page fault during hm_actv_read_xact_vm
	 * in current implementation. Remove this after page fault is supported
	 * in hm_actv_read_xact_vm.
	 */
	int ret;

	if (dst == NULL) {
		return -EINVAL;
	}
	mem_zero_b(dst, (size_t)n);

	ret = hm_copy_from_user(dst, src, n);
	if (ret < 0) {
		udk_error("copy from user failed, err=%s\n", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}

int udk_copy_to_user(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = hm_copy_to_user(dst, src, n);
	if (ret < 0) {
		udk_error("copy to user failed, err=%s\n", hmstrerror(ret));
	}

	return -hmerrno2posix(ret);
}

void *udk_malloc_ex(size_t size, uint32_t paf)
{
	if (size == 0) {
		return NULL;
	}
#ifdef UDK_FUSION
	return sysmgr_internal_malloc(size, paf);
#else
	return malloc(size);
#endif
}

void *udk_malloc(size_t size)
{
	if (size == 0) {
		return NULL;
	}

	return malloc(size);
}

void udk_free(void *ptr)
{
	free(ptr);
}
