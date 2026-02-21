/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Syscall of virtual memory in hmld
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 14:24:58 2019
 */

#include <sys/mman.h>
#include <hmasm/page.h>
#include <hongmeng/types.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <libsysif/sysmgr/api.h>

#include <hm_ldso.h>

void *ldso_mmap(const void *addr, size_t len, int prot, int flags, int _fd,
		off_t offset)
{
	int fd = _fd;
	unsigned long aligned_addr = ptr_to_ulong(addr);
	unsigned long aligned_len = (unsigned long)len;

	if (ULONG_MAX - aligned_addr < PAGE_SIZE) {
		return MAP_FAILED;
	}
	if (ULONG_MAX - aligned_len < PAGE_SIZE) {
		return MAP_FAILED;
	}

	if (!is_map_with_fixed((uint32_t)flags)) {
		aligned_addr = ALIGN_UP(aligned_addr, PAGE_SIZE);
	}

	aligned_len = ALIGN_UP(aligned_len, PAGE_SIZE);

#ifdef __arm__
	offset >>= PAGE_SHIFT;
#endif
	return (void *)lsyscall_mem_mmap_lsyscall(ulong_to_ptr(aligned_addr, void),
						  aligned_len,
						  (unsigned int)prot,
						  (unsigned int)flags,
						  (long)fd,
						  (unsigned long)offset);
}

int ldso_munmap(const void *addr, size_t _len)
{
	size_t len = _len;
	if (((uintptr_t)addr & PAGE_OFFMASK) != 0) {
		return -EINVAL;
	}

	if (ULONG_MAX - len < PAGE_SIZE) {
		return -EOVERFLOW;
	}
	len = PAGE_ALIGN_UP(len);

	return lsyscall_mem_munmap_lsyscall(addr, (unsigned long)len);
}
