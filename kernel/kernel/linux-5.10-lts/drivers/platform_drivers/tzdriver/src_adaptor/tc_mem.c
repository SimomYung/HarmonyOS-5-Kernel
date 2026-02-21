/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include "tc_mem.h"
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include "mem.h"
#include "mailbox_mempool.h"
#include "reserved_mempool.h"
#include "tc_ns_log.h"
#include "tc_current_info.h"

void *tz_alloc(size_t size)
{
	return kzalloc(size, GFP_KERNEL);
}

void tz_free(void *addr)
{
	if (addr)
		kfree(addr);
}

int tz_alloc_sharemem(void **kaddr, size_t len)
{
	void *addr = NULL;

	if (!kaddr)
		return -EINVAL;

	if (len == 0 || len > MAILBOX_POOL_SIZE) {
		tloge("alloc sharemem size %zu is not allowed\n", len);
		return -EINVAL;
	}

	addr = vmalloc_user(len);
	if (!addr) {
		tloge("alloc mailbox failed\n");
		return -ENOMEM;
	}
	*kaddr = addr;
	return 0;
}

void tc_mem_free(struct tc_ns_shared_mem *shared_mem)
{
	if (!shared_mem)
		return;

	if (shared_mem->mem_type == RESERVED_TYPE) {
		reserved_mem_free(shared_mem->kernel_addr);
		tz_free(shared_mem);
		return;
	}

	if (shared_mem->kernel_addr) {
		vfree(shared_mem->kernel_addr);
		shared_mem->kernel_addr = NULL;
	}
	tz_free(shared_mem);
}

#ifdef CONFIG_LIBLINUX
unsigned long agent_buffer_map(unsigned long buffer, uint32_t size)
{
	unsigned long user_addr = 0;
	int ret;

	void *priv = NULL;

	size = PAGE_ALIGN(size);
	if (!size)
		return -ENOMEM;

	user_addr = liblinux_pal_usermap_prepare(user_addr, size, PROT_READ | PROT_WRITE,
			MAP_SHARED | MAP_ANONYMOUS, &priv);
	if (IS_ERR_OR_NULL((const void *)user_addr)) {
		tloge("agent usermap prepare failed\n");
		return user_addr;
	}

	ret = liblinux_pal_remap_pfn_range(user_addr, buffer >> PAGE_SHIFT, size,
			PROT_READ | PROT_WRITE, LIBLINUX_PAL_REMAP_CACHE);
	if (ret) {
		tloge("remap agent buffer failed, err=%d", ret);
		liblinux_pal_usermap_finish((const void *)priv, 0);
		return -EFAULT;
	}

	liblinux_pal_usermap_finish((const void *)priv, 1);
	return user_addr;
}
#else
unsigned long agent_buffer_map(unsigned long buffer, uint32_t size)
{
	struct vm_area_struct *vma = NULL;
	unsigned long user_addr;
	int ret;

	user_addr = vm_mmap(NULL, 0, size, PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS, 0);
	if (IS_ERR_VALUE((uintptr_t)user_addr)) {
		tloge("vm mmap failed\n");
		return user_addr;
	}

	down_read(&mm_sem_lock(CURRENT_MM));
	vma = find_vma(CURRENT_MM, user_addr);
	if (!vma) {
		tloge("user_addr is not valid in vma");
		goto err_out;
	}

	ret = remap_pfn_range(vma, user_addr, buffer >> PAGE_SHIFT, size,
		vma->vm_page_prot);
	if (ret != 0) {
		tloge("remap agent buffer failed, err=%d", ret);
		goto err_out;
	}

	up_read(&mm_sem_lock(CURRENT_MM));
	return user_addr;
err_out:
	up_read(&mm_sem_lock(CURRENT_MM));
	if (vm_munmap(user_addr, size) != 0)
		tloge("munmap failed\n");
	return -EFAULT;
}
#endif
