/**
 * @file allocator_ex.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator 源代码文件。
 * @details
 * @author anonymous
 * @date 2021-03-10
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-10  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "himalloc.h"
#include "allocator_inner.h"
#include "allocator_ex.h"
#include "allocator_ex_inner.h"
#include "mem_hm_memattr.h"
#include "mem_hm_log_inner.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define ALLOCATOR_RELEASE_DFX_SIZE          (2 * sizeof(uint32_t))
#define ALLOCATOR_RELEASE_DFX_HANDLE_MASK (0xFFFF)
#define ALLOCATOR_RELEASE_DFX_MAX_CHECK_SIZE (8 * 2 * sizeof(uint32_t))
#define MEMINFO_LOG_MAX_NUM_PER_LINE 8
#define MEMINFO_LOG_MAX_LENGTH (8 * 2 * 11 + 3) /* 11 is calculated from 0x00000000 + space.
													3 is sum of two \n and one \0 */
#define HIM_BYHNADLE_SECONDE_TO_NANOSECOND 1000000000U
#define HIM_BYHNADLE_UPDATE_PERIOD_SECOND 10U
#define ALLOC_ALIGN_HEAD_FLAG (size_t)0xADD00BEE
#define ALLOC_ALIGN_HEAD_MASK 0xfff00fff
#define ALLOC_ALIGN_HEAD_OFFSET 12
#define ALLOC_ALIGN_ADDR_OFFSET 3

static uint64_t g_him_updated_time = 0;

static void *malloc_addr_adjust_align(void *addr)
{
	uintptr_t align_head_tmp = *((uintptr_t *)addr - 1);
	void *tmp_addr = NULL;
	if (((align_head_tmp) & ALLOC_ALIGN_HEAD_MASK) == ALLOC_ALIGN_HEAD_FLAG) {
		size_t offset;
		/* offset save in middle of the alignhead */
		offset = ((align_head_tmp) & ~(ALLOC_ALIGN_HEAD_FLAG)) >> ALLOC_ALIGN_HEAD_OFFSET;
		if (offset == 0xff) {
			tmp_addr = (uint8_t *)(*((uintptr_t *)addr - ALLOC_ALIGN_ADDR_OFFSET));
		} else {
			tmp_addr = (uint8_t *)addr - offset;
		}
	} else {
		tmp_addr = addr;
	}
	return tmp_addr;
}

void *malloc_ex(uint16_t handle, const char *filename, uint16_t line, size_t size)
{
    (void)filename;
    (void)line;
    void *addr = NULL;
    size_t tmp_size = size + ALLOCATOR_RELEASE_DFX_SIZE;

    if (handle >= HANDLE_MAX) {
        return NULL;
    }

    if (tmp_size <= size) {
        errno = ENOMEM;
        return NULL;
    }

    addr = ALLOCATOR_ALLOC(tmp_size);
    if (addr != NULL) {
        *(uint32_t *)addr = ((handle & ALLOCATOR_RELEASE_DFX_HANDLE_MASK) << SHIFT_BIT) |
            ALLOCATOR_RELEASE_DFX_HEAD_MAGIC;
        addr = (void *)((uintptr_t)addr + sizeof(uint32_t));
        /* Setting reference count to 1. */
        *(uint32_t *)addr = 1;
        addr = (void *)((uintptr_t)addr + sizeof(uint32_t));
    }

    return addr;
}

void *calloc_ex(uint16_t handle, const char *filename, uint16_t line, size_t n, size_t size)
{
    size_t new_size;
    void *addr = NULL;

    if ((size != 0UL) && (n > ((size_t)-1) / size)) {
        return NULL;
    }

    new_size = size * n;
    addr = malloc_ex(handle, filename, line, new_size);
    if (addr != NULL) {
        (void)memset(addr, 0, new_size);
    }

    return addr;
}

size_t malloc_usable_size_ex(uint16_t handle, void *mem)
{
    (void)handle;
    if (mem == NULL) {
        return 0UL;
    }

    size_t size;
    void *addr = (void *)((uintptr_t)mem - ALLOCATOR_RELEASE_DFX_SIZE);
    if (ALLOCATOR_ALLOC_USABLE_SIZE_GET(addr, &size) != ALLOCATOR_OK) {
        return 0UL;
    }

    return (size_t)(size - ALLOCATOR_RELEASE_DFX_SIZE);
}

void *realloc_ex(uint16_t handle, const char *filename, uint16_t line, void *old_mem, size_t new_size)
{
    size_t length;
    size_t old_size;
    void *new_mem = NULL;

    if (old_mem == NULL) {
        return malloc_ex(handle, filename, line, new_size);
    }

    if (new_size == 0UL) {
        free_ex(handle, old_mem);
        return NULL;
    }

    old_size = malloc_usable_size_ex(handle, old_mem);
    if (old_size == 0) {
        return NULL;
    }

    new_mem = malloc_ex(handle, filename, line, new_size);
    if (new_mem == NULL) {
        return NULL;
    }

    length = (new_size < old_size) ? new_size : old_size;
    if (memcpy(new_mem, old_mem, length) != new_mem) {
        free_ex(handle, new_mem);
        return NULL;
    }

    free_ex(handle, old_mem);

    return new_mem;
}

static void himalloc_byhandle_damage_info_show(uintptr_t addr, size_t len)
{
	size_t i;
	int32_t ret;
	int32_t cur_len;
	size_t temp_len = (len <= ALLOCATOR_RELEASE_DFX_MAX_CHECK_SIZE) ? len : ALLOCATOR_RELEASE_DFX_MAX_CHECK_SIZE;
	char buffer[MEMINFO_LOG_MAX_LENGTH];

	if (!MemHMMemAttrRdChk(addr, temp_len)) {
		return;
	}

	buffer[0] = '\0';
	cur_len = 0;
	MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_DAMAGE_HAPPEN,
		"[himalloc_byhandle-%s]:The meminfo length is %zu, it from [0x%x] to [0x%x] is:\n",
		__FUNCTION__, temp_len, addr, (uint32_t *)(addr + temp_len));

	uint32_t *temp_addr = (uint32_t *)addr;
	size_t cur_address_num = (temp_len / sizeof(uint32_t));
	for (i = 0; i < cur_address_num; i++) {
		if (i % MEMINFO_LOG_MAX_NUM_PER_LINE == 0) {
			ret = sprintf(buffer + cur_len, "\n0x%08x ", *temp_addr);
		} else {
			ret = sprintf(buffer + cur_len, "0x%08x ", *temp_addr);
		}
		if (ret < 0) {
			MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_DAMAGE_HAPPEN,
				"[himalloc_byhandle-%s]:sprintf failed!\n", __FUNCTION__);
			return;
		}
		cur_len += ret;
		temp_addr++;
	}

	if (temp_len % sizeof(uint32_t) != 0) {
		uint32_t reserve_num = temp_len % sizeof(uint32_t);
		uint8_t tmp_num[4] = {0};
    	for (i = 0; i < reserve_num; i++) {
    		tmp_num[i] = ((uint8_t *)temp_addr)[i];
    	}

    	if (cur_address_num % MEMINFO_LOG_MAX_NUM_PER_LINE == 0) {
			ret = sprintf(buffer + cur_len, "\n0x%08x ", *((uint32_t *)tmp_num));
		} else {
			ret = sprintf(buffer + cur_len, "0x%08x ", *((uint32_t *)tmp_num));
		}
		if (ret < 0) {
			MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_DAMAGE_HAPPEN,
				"[himalloc_byhandle-%s]:sprintf failed!\n", __FUNCTION__);
			return;
		}
	}

	MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_DAMAGE_HAPPEN,
		"[himalloc_byhandle-%s]:%s\n", __FUNCTION__, buffer);
}

void free_ex(uint16_t handle, void *mem)
{
    size_t cur_size;
    if (mem == NULL) {
        return;
    }

    void *addr = malloc_addr_adjust_align(mem);

	addr = (void *)((uintptr_t)addr - ALLOCATOR_RELEASE_DFX_SIZE);

    /* magic number check */
	uint32_t temp_head_flag = *(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK;
	if (temp_head_flag != ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
		if (VosHimallocAllocUsableSizeGet(addr, &cur_size) == ALLOCATOR_OK) {
			himalloc_byhandle_damage_info_show((uintptr_t)addr, cur_size);
		}
		if (g_himalloc_enable_mcheck) {
			abort(); /* The memory is damaged. The address may be invalid. Abort is performed when MALOC_CHECK_ is
				enabled. */
		}
		ALLOCATOR_FREE(addr);
		return;
	}

	/* handle check */
	uint16_t temp_handle = (uint16_t)((*(uint32_t *)addr & ALLOCATOR_RELEASE_DFX_GET_HANDLE_MASK) >> SHIFT_BIT);
	if (temp_handle != handle) {
		MemHMLogWrite(MEM_HM_LOG_LEVEL_ERROR, ERRNO_DAMAGE_HAPPEN,
			"[himalloc_byhandle-%s]:The handle check failed. handle = %hu\n", __FUNCTION__, handle);
	}

    addr = (void *)((uintptr_t)addr + sizeof(uint32_t));
	/* Setting reference count to 0. */
	*(uint32_t *)addr = 0;
	addr = (void *)((uintptr_t)addr - sizeof(uint32_t));


    ALLOCATOR_FREE(addr);
}

uint32_t mem_ref_get_ex(void *addr, uint32_t *ref_count)
{
	if ((addr == NULL) || (ref_count == NULL)) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	void *head_addr = (void *)((uintptr_t)addr - ALLOCATOR_RELEASE_DFX_SIZE);

	/* magic number check */
	uint32_t temp_head_flag = *(uint32_t *)head_addr & ALLOCATOR_DFX_HEAD_MASK;
	if (temp_head_flag != ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	head_addr = (void *)((uintptr_t)head_addr + sizeof(uint32_t));
	*ref_count = *(uint32_t *)head_addr;
	return (uint32_t)ALLOCATOR_OK;
}

uint32_t mem_ref_inc_ex(void *addr)
{
	if (addr == NULL) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	void *head_addr = (void *)((uintptr_t)addr - ALLOCATOR_RELEASE_DFX_SIZE);

	/* magic number check */
	uint32_t temp_head_flag = *(uint32_t *)head_addr & ALLOCATOR_DFX_HEAD_MASK;
	if (temp_head_flag != ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	head_addr = (void *)((uintptr_t)head_addr + sizeof(uint32_t));
	*(uint32_t *)head_addr += 1;
	return (uint32_t)ALLOCATOR_OK;
}

void *memalign_ex(uint16_t handle, size_t align, size_t size)
{
	size_t   align_byte_size;
	size_t   new_size;
	size_t   offset;
	size_t   align_head;
	size_t   remainder;
	void	 *addr = NULL;
	void	 *user_addr = NULL;

	if ((align != 8) && (align != 16) && (align != 32) && /* align byte: 8 16 32 64 128 */
		(align != 64) && (align != 128)) { /* align byte: 8 16 32 64 128 */
		return NULL;
	}
	align_byte_size = (size_t)align;

	if (size == 0) {
		return NULL;
	}

	/* at most,the offset is ulAlignBye - 1 */
	new_size = size + align_byte_size;
	if (new_size < size) {
		return NULL;
	}

	addr = malloc_ex(handle, NULL, 0, new_size);
	if (addr == NULL) {
		return NULL;
	}

	remainder = (uintptr_t)addr & (align_byte_size - 1);
	if (remainder != 0) {
		/* not aligned */
		offset = align_byte_size - remainder;
		user_addr = (void *)((uintptr_t)addr + offset);

		/* 12: the bits to shift, to set align address offset */
		align_head = ALLOC_ALIGN_HEAD_FLAG | (offset << 12);
		*((uintptr_t *)user_addr - 1) = align_head;
	} else {
		user_addr = addr;
	}
	return user_addr;
}

static uint64_t get_cur_time_ns_ex(void)
{
	struct timespec tp;
	uint64_t time_ns;

	(void)clock_gettime(CLOCK_MONOTONIC, &tp);
	time_ns = (uint64_t)tp.tv_sec * (uint64_t)HIM_BYHNADLE_SECONDE_TO_NANOSECOND + (uint64_t)tp.tv_nsec;

	return time_ns;
}

uint32_t mem_user_size_get_ex(uint16_t handle, size_t *size)
{
	if (size == NULL) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	uint64_t cur_time = get_cur_time_ns_ex();
	if ((cur_time - g_him_updated_time) >
		((uint64_t)(HIM_BYHNADLE_UPDATE_PERIOD_SECOND * HIM_BYHNADLE_SECONDE_TO_NANOSECOND))) {
		g_him_updated_time = cur_time;
		uint32_t ret = update_handle_size_ex();
		if (ret != ALLOCATOR_OK) {
			return (uint32_t)ALLOCATOR_ERR;
		}
	}

	mem_handle_use_info *handle_size_array = get_handle_size_ex();
	if (handle_size_array == NULL) {
		return (uint32_t)ALLOCATOR_ERR;
	}
	*size = handle_size_array[handle].use_size;
	return (uint32_t)ALLOCATOR_OK;
}

void mem_stat_sort_ex(uint32_t user_handle_num, uint32_t handle_num, mem_handle_use_info *handle_info)
{
	size_t tmp_size;
	uint16_t tmp_handle;
	uint32_t i;
	uint32_t j;
	uint32_t max_loop_num = (user_handle_num > handle_num) ? handle_num : user_handle_num;

	for (i = 0; i < max_loop_num; i++) {
		for (j = 0; j < handle_num - i - 1; j++) {
			if (handle_info[j].use_size > handle_info[j + 1].use_size) {
				tmp_size = handle_info[j].use_size;
				tmp_handle = handle_info[j].handle;

				handle_info[j].use_size = handle_info[j + 1].use_size;
				handle_info[j].handle = handle_info[j + 1].handle;

				handle_info[j + 1].use_size = tmp_size;
				handle_info[j + 1].handle = tmp_handle;
			}
		}
	}
}

uint32_t mem_topn_size_get_ex(uint32_t *handle_num, mem_handle_use_info handle_info[])
{
	if ((handle_num == NULL) || (handle_num == 0)) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	if (handle_info == NULL) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	uint64_t cur_time = get_cur_time_ns_ex();
	if ((cur_time - g_him_updated_time) >
		((uint64_t)(HIM_BYHNADLE_UPDATE_PERIOD_SECOND * HIM_BYHNADLE_SECONDE_TO_NANOSECOND))) {
		g_him_updated_time = cur_time;
		uint32_t ret = update_handle_size_ex();
		if (ret != ALLOCATOR_OK) {
			return (uint32_t)ALLOCATOR_ERR;
		}
	}

	mem_handle_use_info *handle_size_array = get_handle_size_ex();
	if (handle_size_array == NULL) {
		return (uint32_t)ALLOCATOR_ERR;
	}

	uint32_t tmp_user_handle_num = *handle_num;
	uint32_t tmp_handle_num = max_handle_num_get_ex();
	mem_stat_sort_ex(tmp_user_handle_num, tmp_handle_num, handle_size_array);
	(void)memset(handle_info, 0, tmp_user_handle_num * sizeof(mem_handle_use_info));
	uint32_t max_loop_num = (tmp_user_handle_num > tmp_handle_num) ? tmp_handle_num : tmp_user_handle_num;

	uint32_t i;
	for (i = 0; i < max_loop_num; i++) {
		handle_info[i].handle = handle_size_array[tmp_handle_num - 1 - i].handle;
		handle_info[i].use_size = handle_size_array[tmp_handle_num - 1 - i].use_size;
	}

	uint32_t handle_cnt = 0;
	for (i = 0; i < tmp_user_handle_num; i++) {
		if (handle_info[i].use_size > 0) {
			handle_cnt++;
		} else {
			break;
		}
	}
	*handle_num = handle_cnt;

	return (uint32_t)ALLOCATOR_OK;
}

uint16_t mem_handle_get_by_addr(void *mem)
{
	uint16_t temp_handle = HANDLE_MAX;
	if (mem == NULL) {
		return temp_handle;
	}

	void *addr = malloc_addr_adjust_align(mem);

	addr = (void *)((uintptr_t)addr - ALLOCATOR_RELEASE_DFX_SIZE);

	/* magic number check */
	uint32_t temp_head_flag = *(uint32_t *)addr & ALLOCATOR_DFX_HEAD_MASK;
	if (temp_head_flag != ALLOCATOR_RELEASE_DFX_HEAD_MAGIC) {
		return temp_handle;
	}

	/* handle check */
	temp_handle = (uint16_t)((*(uint32_t *)addr & ALLOCATOR_RELEASE_DFX_GET_HANDLE_MASK) >> SHIFT_BIT);
	return temp_handle;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
